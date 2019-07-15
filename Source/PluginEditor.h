/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "ScopeComponent.h"

//==============================================================================
/**
*/
class AuxInputAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    AuxInputAudioProcessorEditor (AuxInputAudioProcessor&);
    ~AuxInputAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AuxInputAudioProcessor& processor;

	std::unique_ptr<AudioDeviceSelectorComponent> selectorComponet;
	std::unique_ptr<ScopeComponent<float>> scopeComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxInputAudioProcessorEditor)
};
