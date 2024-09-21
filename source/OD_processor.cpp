//------------------------------------------------------------------------
// Copyright(c) 2024 ArtificialVoid1.
//------------------------------------------------------------------------

#include "OD_processor.h"
#include "OD_cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"

using namespace Steinberg;

namespace MyCompanyName {
//------------------------------------------------------------------------
// OD_1Processor
//------------------------------------------------------------------------
OD_1Processor::OD_1Processor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kOD_1ControllerUID);
}

//------------------------------------------------------------------------
OD_1Processor::~OD_1Processor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API OD_1Processor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API OD_1Processor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API OD_1Processor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------

float max(float a, float b) {
	if (a >= b) { return a; }
	else { return b; }
}
float min(float a, float b) {
	if (a <= b) { return a; }
	else { return b; }
}

//Clipping Function
float ClipFunc(float Drive, float Sample) {
	return max(min(1, (Drive + 0.2f) * Sample), -1);
}
tresult PLUGIN_API OD_1Processor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
				case params::kGain:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mGain = value;
					break;
				case params::kDistortion:
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						mDist = value;
					break;
				}
			}
		}
	}
	
	//--- Here you have to implement your processing

	Vst::Sample32* InL = data.inputs[0].channelBuffers32[0];
	Vst::Sample32* InR = data.inputs[0].channelBuffers32[1];

	Vst::Sample32* OutL = data.outputs[0].channelBuffers32[0];
	Vst::Sample32* OutR = data.outputs[0].channelBuffers32[1];

	for (int i = 0; i < data.numSamples; i++)
	{
		OutL[i] = ClipFunc(((mDist * 9) + 1) * 1.3, InL[i]);
		OutL[i] *= (((mGain * 9) + 1) * 2)* 0.03;
		
		OutR[i] = ClipFunc(((mDist * 9) + 1) * 1.3, InR[i]);
		OutR[i] *= (((mGain * 9) + 1) * 2) * 0.03;
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API OD_1Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API OD_1Processor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API OD_1Processor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);

	float fval;
	if (streamer.readFloat(fval) == false) { return kResultFalse; }
	mGain = fval;
	if (streamer.readFloat(fval) == false) { return kResultFalse; }
	mDist = fval;
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API OD_1Processor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	streamer.writeFloat(mGain);
	streamer.writeFloat(mDist);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace MyCompanyName
