/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperSlowAudioProcessorEditor::SuperSlowAudioProcessorEditor (SuperSlowAudioProcessor& p)
    : AudioProcessorEditor (&p)
	, processor (p)
	, mButtonLoadFile("Select File")
	, mToggleFast("Fast")
	, mToggleNorm("Normal")
	, mToggleSlow("Slow")
	, mButtonExportFile("Export")
{
    setSize (400, 300);

	addAndMakeVisible(mButtonLoadFile);
	mButtonLoadFile.onClick = [this]
	{
		FileChooser fileChooser("Select audio file");
                 
		bool ok = fileChooser.browseForFileToOpen();
		if(ok)
		{
			File result = fileChooser.getResult();
			processor.setFile(result);
		}
	};

	addAndMakeVisible(mSliderDelta);
	mSliderDelta.setSliderStyle(Slider::SliderStyle::IncDecButtons);
	mSliderDelta.setRange(1, 16, 1);
	mSliderDelta.onValueChange = [this]
	{
		processor.setDelta(mSliderDelta.getValue());
	};

	addAndMakeVisible(mToggleFast);
	mToggleFast.onClick = [this]
	{
		processor.setMode(SuperSlowAudioProcessor::Mode::Fast);
		mToggleNorm.setToggleState(false, dontSendNotification);
		mToggleSlow.setToggleState(false, dontSendNotification);
		mToggleFast.setToggleState(true, dontSendNotification);
	};
	addAndMakeVisible(mToggleNorm);
	mToggleNorm.onClick = [this]
	{
		processor.setMode(SuperSlowAudioProcessor::Mode::Norm);
		mToggleNorm.setToggleState(true, dontSendNotification);
		mToggleSlow.setToggleState(false, dontSendNotification);
		mToggleFast.setToggleState(false, dontSendNotification);
	};
	addAndMakeVisible(mToggleSlow);
	mToggleSlow.onClick = [this]
	{
		processor.setMode(SuperSlowAudioProcessor::Mode::Slow);
		mToggleNorm.setToggleState(false, dontSendNotification);
		mToggleSlow.setToggleState(true, dontSendNotification);
		mToggleFast.setToggleState(false, dontSendNotification);
	};

	addAndMakeVisible(mButtonExportFile);
	mButtonExportFile.onClick = [this]
	{
		processor.exportFile();
	};
}

SuperSlowAudioProcessorEditor::~SuperSlowAudioProcessorEditor()
{
}

//==============================================================================
void SuperSlowAudioProcessorEditor::paint (Graphics& g)
{
}

void SuperSlowAudioProcessorEditor::resized()
{
	auto bounds = getLocalBounds();

	auto height = 50;
	mButtonLoadFile.setBounds(bounds.removeFromTop(height).reduced(10));
	mSliderDelta.setBounds(bounds.removeFromTop(height).reduced(10));
	mToggleFast.setBounds(bounds.removeFromTop(height).reduced(10));
	mToggleNorm.setBounds(bounds.removeFromTop(height).reduced(10));
	mToggleSlow.setBounds(bounds.removeFromTop(height).reduced(10));
	mButtonExportFile.setBounds(bounds.removeFromTop(height).reduced(10));
}
