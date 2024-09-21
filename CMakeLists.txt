cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "E:/Programming/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(OD_1
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "OD_1 VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(OD_1
    source/version.h
    source/OD_cids.h
    source/OD_processor.h
    source/OD_processor.cpp
    source/OD_controller.h
    source/OD_controller.cpp
    source/OD_entry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(OD_1
        PRIVATE
            resource/OD_editor.uidesc
    )
    target_link_libraries(OD_1
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(OD_1
        RESOURCES
            "resource/OD_editor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (OD_1
    RESOURCES
        resource/2DC6A63CB5D35712A50700B8828786FD_snapshot.png
        resource/2DC6A63CB5D35712A50700B8828786FD_snapshot_2.0x.png
)

target_link_libraries(OD_1
    PRIVATE
        sdk
)

smtg_target_configure_version_file(OD_1)

if(SMTG_MAC)
    smtg_target_set_bundle(OD_1
        BUNDLE_IDENTIFIER Void
        COMPANY_NAME "ArtificialVoid1"
    )
    smtg_target_set_debug_executable(OD_1
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(OD_1 PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OD_1)

        smtg_target_set_debug_executable(OD_1
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)