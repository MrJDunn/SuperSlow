/*
  ==============================================================================

    InterpolationBase.h
    Created: 4 Mar 2021 9:51:40am
    Author:  Jeff

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class InterpolationBase 
{
public: 
	InterpolationBase() {};
	virtual ~InterpolationBase() {};

	void prepareToPlay(double sampleRate, int blockSize)
	{

	};
	void processBlock(AudioBuffer<float>& buffer)
	{
	};

protected:
	double _sampleRate;
	int _blockSize;

protected:
	virtual void interpolate()
	{
	};
};
