//------------------------------------------------------------------------
// Copyright(c) 2024 ArtificialVoid1.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace MyCompanyName {
//------------------------------------------------------------------------
static const Steinberg::FUID kOD_1ProcessorUID (0x2DC6A63C, 0xB5D35712, 0xA50700B8, 0x828786FD);
static const Steinberg::FUID kOD_1ControllerUID (0x07F64196, 0xBF1C50F0, 0xABF6F9D6, 0x5DD31256);

#define OD_1VST3Category "Fx"

enum params {
	kGain = 100,
	kDistortion = 101,
};

//------------------------------------------------------------------------
} // namespace MyCompanyName
