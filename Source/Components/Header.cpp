/*
  ==============================================================================

    Header.cpp
    Created: 3 Mar 2021 6:18:01pm
    Author:  Jeff

  ==============================================================================
*/

#include "Header.h"

Header::Header(SuperSlowAudioProcessor& p)
	: _processor(p)
{
	setOpaque(false);

	addAndMakeVisible(mSliderDelta);
	mSliderDelta.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	mSliderDelta.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 30, 10);
	mSliderDelta.setRange(1, 16, 1);
	mSliderDelta.onValueChange = [this]
	{
		_processor.setDelta(mSliderDelta.getValue());
	};


	addAndMakeVisible(mSliderWet);
	mSliderWet.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	mSliderWet.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 30, 10);
	mSliderWet.setRange(1, 100, 1);
	mSliderWet.onValueChange = [this]
	{
		_processor.setWet((float)mSliderWet.getValue() / 100.f);
		resized();
	};

	// Interpolation mode
	addAndMakeVisible(mToggleInterpolationNone);
	mToggleInterpolationNone.onClick = [this]
	{
		setInterpolation(SuperSlowAudioProcessor::Interpolation::None);
	};
	addAndMakeVisible(mToggleInterpolationLinear);
	mToggleInterpolationLinear.onClick = [this]
	{
		setInterpolation(SuperSlowAudioProcessor::Interpolation::Linear);
	};
	addAndMakeVisible(mToggleInterpolationRandom);
	mToggleInterpolationRandom.onClick = [this]
	{
		setInterpolation(SuperSlowAudioProcessor::Interpolation::Random);
	};

	addAndMakeVisible(mOnOff);
	mOnOff.onClick = [this]
	{
		if(mOnOff.getToggleState())
		{
			setPlayMode(SuperSlowAudioProcessor::Mode::Slow);
		}
		else
		{
			setPlayMode(SuperSlowAudioProcessor::Mode::Norm);
		}
	};

	auto boldFont = Font(8.f, 1);
	auto notBoldFont = Font(8.f, 0);

	addAndMakeVisible(mLabelEnabled);
	mLabelEnabled.setFont(boldFont);
	mLabelEnabled.setJustificationType(Justification::centred);

	addAndMakeVisible(mLabelDelta);
	mLabelDelta.setFont(boldFont);
	mLabelDelta.setJustificationType(Justification::centred);

	addAndMakeVisible(mLabelInterpolation);
	mLabelInterpolation.setFont(boldFont);
	mLabelInterpolation.setJustificationType(Justification::centred);

	addAndMakeVisible(mLabelLinear);
	mLabelLinear.setFont(notBoldFont);
	mLabelLinear.setJustificationType(Justification::left);

	addAndMakeVisible(mLabelNone);
	mLabelNone.setFont(notBoldFont);
	mLabelNone.setJustificationType(Justification::left);

	addAndMakeVisible(mLabelRandom);
	mLabelRandom.setFont(notBoldFont);
	mLabelRandom.setJustificationType(Justification::left);

	addAndMakeVisible(mLabelWet);
	mLabelWet.setFont(boldFont);
	mLabelWet.setJustificationType(Justification::centred);

	// Initialise buttons
	setPlayMode(_processor.getMode());
	setInterpolation(_processor.getInterpolation());
	mSliderDelta.setValue(_processor.getDelta());
	mSliderWet.setValue(_processor.getWet());
}

Header::~Header()
{
}

void Header::resized()
{
	auto bounds = getLocalBounds();

	bounds.removeFromLeft(getWidth() / 2 - 246 / 2);

	auto labelHeight = 10;
	auto width = 52;

	auto deltaBounds = bounds.removeFromLeft(width);
	mLabelDelta.setBounds(deltaBounds.removeFromTop(labelHeight));
	mSliderDelta.setBounds(deltaBounds);

	auto wetBounds = bounds.removeFromLeft(width);
	mLabelWet.setBounds(wetBounds.removeFromTop(labelHeight));
	mSliderWet.setBounds(wetBounds);

	auto toggleBounds = bounds.removeFromLeft((float)width * 1.5f);

	mLabelInterpolation.setBounds(toggleBounds.removeFromTop(labelHeight));

	auto noneBounds = toggleBounds.removeFromTop(labelHeight + 4);
	mLabelNone.setBounds(noneBounds.removeFromLeft(noneBounds.getWidth() / 2));
	mToggleInterpolationNone.setBounds(noneBounds);

	auto linearBounds = toggleBounds.removeFromTop(labelHeight + 4);
	mLabelLinear.setBounds(linearBounds.removeFromLeft(linearBounds.getWidth() / 2));
	mToggleInterpolationLinear.setBounds(linearBounds);

	auto randomBounds = toggleBounds.removeFromTop(labelHeight + 4);
	mLabelRandom.setBounds(randomBounds.removeFromLeft(randomBounds.getWidth() / 2));
	mToggleInterpolationRandom.setBounds(randomBounds);

	auto enabledBounds = bounds.removeFromLeft(width);
	mLabelEnabled.setBounds(enabledBounds.removeFromTop(labelHeight));
	mOnOff.setBounds(enabledBounds.removeFromRight(41));
}

void Header::paint(Graphics& g)
{
	auto bounds = getLocalBounds();

	g.setColour(Colour::fromRGB(33, 33, 33));
	// top bar
	g.fillRect(0,0,getWidth(),24);

	// center rounded rectangle
	g.fillRoundedRectangle(getWidth() / 2 - 246 / 2, 0, 246, getHeight(), 24.0f);

	g.setColour(Colours::white);
	g.setFont(Font(12.5f, 1));
	g.drawFittedText("SUPERSLOW", { 8,4,getWidth(),16 }, Justification::left, 1);
}

void Header::setPlayMode(const SuperSlowAudioProcessor::Mode& mode)
{
	_processor.setMode(mode);
	switch (mode)
	{
	case SuperSlowAudioProcessor::Mode::Fast:
		// Not available in plugin mode
		jassertfalse;
		break;
	case SuperSlowAudioProcessor::Mode::Norm:
		mOnOff.setToggleState(false, dontSendNotification);
		break;
	case SuperSlowAudioProcessor::Mode::Slow:
		mOnOff.setToggleState(true, dontSendNotification);
		break;
	}
}

void Header::setInterpolation(const SuperSlowAudioProcessor::Interpolation & interpolation)
{
	_processor.setInterpolation(interpolation);
	switch (interpolation)
	{
	case SuperSlowAudioProcessor::Interpolation::None:
		
		mToggleInterpolationNone.setToggleState(true, dontSendNotification);
		mToggleInterpolationLinear.setToggleState(false, dontSendNotification);
		mToggleInterpolationRandom.setToggleState(false, dontSendNotification);
		break;
	case SuperSlowAudioProcessor::Interpolation::Linear:
		mToggleInterpolationNone.setToggleState(false, dontSendNotification);
		mToggleInterpolationLinear.setToggleState(true, dontSendNotification);
		mToggleInterpolationRandom.setToggleState(false, dontSendNotification);

		break;
	case SuperSlowAudioProcessor::Interpolation::Random:
		mToggleInterpolationNone.setToggleState(false, dontSendNotification);
		mToggleInterpolationLinear.setToggleState(false, dontSendNotification);
		mToggleInterpolationRandom.setToggleState(true, dontSendNotification);

		break;
	}
}

