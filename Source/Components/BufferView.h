/*
  ==============================================================================

    BufferView.h
    Created: 3 Mar 2021 6:18:26pm
    Author:  Jeff

  ==============================================================================
*/

#pragma once
#include "../PluginProcessor.h"

#include <deque>

class BufferView : public Component, public Timer
{
public: 
	BufferView(SuperSlowAudioProcessor& p);
	~BufferView();

	void resized() override;
	void paint(Graphics& g) override;

private:
	SuperSlowAudioProcessor& _processor;

	std::deque<float> _history;

	int _rate;

private:
	// Inherited via Timer
	virtual void timerCallback() override;
};

