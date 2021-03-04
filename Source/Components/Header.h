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

class Header : public Component 
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

	Label mLabelWet{ "WET/DRY", "WET/DRY" };
	Slider mSliderWet;

	Label mLabelInterpolation{ "INTERPOLATION", "INTERPOLATION" };
	Label mLabelNone{ "NONE", "NONE" };
	Label mLabelLinear{ "LINEAR", "LINEAR" };
	Label mLabelRandom{ "RANDOM", "RANDOM" };

	ToggleButton mToggleInterpolationNone;
	ToggleButton mToggleInterpolationLinear;
	ToggleButton mToggleInterpolationRandom;

	Label mLabelEnabled{ "ENABLED", "ENABLED" };
	ToggleButton mOnOff;

private:
	void setPlayMode(const SuperSlowAudioProcessor::Mode& mode);
	void setInterpolation(const SuperSlowAudioProcessor::Interpolation& interpolation);

}; 
