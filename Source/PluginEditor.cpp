/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuxInputAudioProcessorEditor::AuxInputAudioProcessorEditor (AuxInputAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
#if JUCE_WINDOWS
	String typeFaceName = "Meiryo UI";
	getLookAndFeel().setDefaultSansSerifTypefaceName(typeFaceName);
#endif

	selectorComponet.reset(new AudioDeviceSelectorComponent(processor.getDeviceManager(), 0, 2, 0, 0, false, false, false, false));
	addAndMakeVisible(selectorComponet.get());

	scopeComponent.reset(new ScopeComponent<float>(processor.getScopeDataQueue()));
	addAndMakeVisible(scopeComponent.get());

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 640);

}

AuxInputAudioProcessorEditor::~AuxInputAudioProcessorEditor()
{
}

//==============================================================================
void AuxInputAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void AuxInputAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	auto area = getLocalBounds();

	scopeComponent->setBounds(area.removeFromLeft(area.getWidth() * 0.5));
	selectorComponet->setBounds(area);

}
