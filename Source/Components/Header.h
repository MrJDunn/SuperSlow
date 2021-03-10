/*
  ==============================================================================

    Header.h
    Created: 3 Mar 2021 6:18:01pm
    Author:  Jeff

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

class Header : public Component, public Timer
{
public: 
	Header(SuperSlowAudioProcessor& p);
	~Header();

	void resized() override;
	void paint(Graphics& g) override;
private:
	SuperSlowAudioProcessor& _processor;
		

	Label mLabelDelta{ "MULTIPLIER", "MULTIPLIER" };
	Slider mSliderDelta;
	Label mLabelDeltaDisplay;

	Label mLabelWet{ "WET/DRY", "WET/DRY" };
	Slider mSliderWet;
	Label mLabelWetDisplay;

	Label mLabelInterpolation{ "INTERPOLATION", "INTERPOLATION" };
	Label mLabelNone{ "NONE", "NONE" };
	Label mLabelLinear{ "LINEAR", "LINEAR" };
	Label mLabelRandom{ "RANDOM", "RANDOM" };

	ToggleButton mToggleInterpolationNone;
	ToggleButton mToggleInterpolationLinear;
	ToggleButton mToggleInterpolationRandom;

	Label mLabelEnabled{ "ENABLED", "ENABLED" };
	ToggleButton mToggleEnabled;
	Label mLabelEnabledDisplay;

private:

	float getMode();
	void setMode(const SuperSlowAudioProcessor::Mode& mode);

	float getInterpolation();
	void setInterpolation(const SuperSlowAudioProcessor::Interpolation& interpolation);

	float getWet();
	void setWet(float wet);

	float getDelta();
	void setDelta(float delta);

	void handleStateChange();

	// Inherited via Timer
	virtual void timerCallback() override;
};
