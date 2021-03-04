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
	, mButtonLoadFile("Select File")
	, mToggleFast("Fast")
	, mToggleNorm("Normal")
	, mToggleSlow("Slow")
	, mButtonExportFile("Export")
{
	setLookAndFeel(&style);

    setSize (480, 270);

	addAndMakeVisible(_header);
	addAndMakeVisible(_bufferView);

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
	mSliderDelta.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	mSliderDelta.setRange(1, 16, 1);
	mSliderDelta.onValueChange = [this]
	{
		processor.setDelta(mSliderDelta.getValue());
	};


	// Speed
	addAndMakeVisible(mToggleFast);
	mToggleFast.onClick = [this]
	{
		setPlayMode(SuperSlowAudioProcessor::Mode::Fast);
	};
	addAndMakeVisible(mToggleNorm);
	mToggleNorm.onClick = [this]
	{
		setPlayMode(SuperSlowAudioProcessor::Mode::Norm);
	};
	addAndMakeVisible(mToggleSlow);
	mToggleSlow.onClick = [this]
	{
		setPlayMode(SuperSlowAudioProcessor::Mode::Slow);
	};

	addAndMakeVisible(mButtonExportFile);
	mButtonExportFile.onClick = [this]
	{
		processor.exportFile();
	};

	// Initialise buttons
	setPlayState(processor.getPlayState());
	setPlayMode(processor.getMode());

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

//	auto height = 50;
//	mButtonLoadFile.setBounds(bounds.removeFromTop(height).reduced(10));
//	mSliderDelta.setBounds(bounds.removeFromTop(height).reduced(10));
//	//mToggleFast.setBounds(bounds.removeFromTop(height).reduced(10));
//	mToggleNorm.setBounds(bounds.removeFromTop(height).reduced(10));
//	mToggleSlow.setBounds(bounds.removeFromTop(height).reduced(10));
//	mButtonExportFile.setBounds(bounds.removeFromTop(height).reduced(10));
}

void SuperSlowAudioProcessorEditor::setPlayMode(const SuperSlowAudioProcessor::Mode& mode)
{
	processor.setMode(mode);
	switch (mode)
	{
	case SuperSlowAudioProcessor::Mode::Fast:
		
		mToggleNorm.setToggleState(false, dontSendNotification);
		mToggleSlow.setToggleState(false, dontSendNotification);
		mToggleFast.setToggleState(true, dontSendNotification);
		break;
	case SuperSlowAudioProcessor::Mode::Norm:
		mToggleNorm.setToggleState(true, dontSendNotification);
		mToggleSlow.setToggleState(false, dontSendNotification);
		mToggleFast.setToggleState(false, dontSendNotification);

		break;
	case SuperSlowAudioProcessor::Mode::Slow:
		mToggleNorm.setToggleState(false, dontSendNotification);
		mToggleSlow.setToggleState(true, dontSendNotification);
		mToggleFast.setToggleState(false, dontSendNotification);

		break;
	}
}

void SuperSlowAudioProcessorEditor::setPlayState(const SuperSlowAudioProcessor::PlayState& playState)
{
	switch(playState)
	{
	case SuperSlowAudioProcessor::PlayState::Paused:
		break;
	case SuperSlowAudioProcessor::PlayState::Playing:
		break;
	}
}
