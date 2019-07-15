/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
	CriticalSection globalLock;

	AudioBuffer<float> internalBuffer;
}

class InternalCallback : public AudioIODeviceCallback
{
public:
	virtual void audioDeviceIOCallback(const float** inputChannelData, int numInputChannels, float** outputChannelData, int numOutputChannels, int numSamples) override
	{
		ScopedLock lock(globalLock);

		//DBG("InternalCallback::audioDeviceIOCallback");
		
		for (int ch_index = 0; ch_index < internalBuffer.getNumChannels(); ++ch_index)
		{
			if (ch_index < numInputChannels)
			{
				internalBuffer.copyFrom(ch_index, 0, inputChannelData[ch_index], jmin(internalBuffer.getNumSamples(), numSamples));
			}
		}
	}

	virtual void audioDeviceAboutToStart(AudioIODevice* device) override
	{
		ScopedLock lock(globalLock);

		DBG("InternalCallback::audioDeviceAboutToStart");

		auto num_channels = jmax<int>(device->getActiveInputChannels().countNumberOfSetBits(), device->getActiveOutputChannels().countNumberOfSetBits());
		auto num_samples = device->getDefaultBufferSize();
		internalBuffer.setSize(num_channels, num_samples, false, false, false);
	}

	virtual void audioDeviceStopped() override
	{
		ScopedLock lock(globalLock);

		DBG("InternalCallback::audioDeviceStopped");
		internalBuffer.clear();
	}

	virtual void audioDeviceError(const String& errorMessage) override
	{
		ScopedLock lock(globalLock);

		DBG("InternalCallback::audioDeviceError");
	}
};

namespace
{
	InternalCallback iocallback;
}

//==============================================================================
AuxInputAudioProcessor::AuxInputAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
	, scopeDataCollector(scopeDataQueue)
{
	deviceManager.addAudioCallback(&iocallback);
}

AuxInputAudioProcessor::~AuxInputAudioProcessor()
{
}

//==============================================================================
const String AuxInputAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AuxInputAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AuxInputAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AuxInputAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AuxInputAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AuxInputAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AuxInputAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AuxInputAudioProcessor::setCurrentProgram (int index)
{
}

const String AuxInputAudioProcessor::getProgramName (int index)
{
    return {};
}

void AuxInputAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AuxInputAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AuxInputAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AuxInputAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AuxInputAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedLock lock(globalLock);

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

	buffer.clear();

	for (int ch_index = 0; ch_index < internalBuffer.getNumChannels(); ++ch_index)
	{
		buffer.copyFrom(ch_index, 0, internalBuffer.getReadPointer(ch_index), buffer.getNumSamples());
	}

	scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());

}

//==============================================================================
bool AuxInputAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AuxInputAudioProcessor::createEditor()
{
    return new AuxInputAudioProcessorEditor (*this);
}

//==============================================================================
void AuxInputAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AuxInputAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AuxInputAudioProcessor();
}
