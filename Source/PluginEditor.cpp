/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperSlowAudioProcessorEditor::SuperSlowAudioProcessorEditor(SuperSlowAudioProcessor& p)
	: AudioProcessorEditor(&p)
	, processor(p)
	, _header(p)
	, _bufferView(p)
{
	setLookAndFeel(&style);

    setSize (480, 270);

	addAndMakeVisible(_header);
	addAndMakeVisible(_bufferView);
}

SuperSlowAudioProcessorEditor::~SuperSlowAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

//==============================================================================
void SuperSlowAudioProcessorEditor::paint (Graphics& g)
{
	g.setGradientFill(ColourGradient(
		Colour::fromRGB( 33,  33,  33), float(getWidth()/2), 0,
		Colour::fromRGB(198, 113,   0), float(getWidth()/2), float(getHeight()), false));
	g.fillAll();
}

void SuperSlowAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds();

	auto headerArea = bounds.removeFromTop(58);
	_header.setBounds(headerArea);
	_bufferView.setBounds(bounds);

}
