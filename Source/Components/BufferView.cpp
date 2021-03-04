/*
  ==============================================================================

    BufferView.cpp
    Created: 3 Mar 2021 6:18:26pm
    Author:  Jeff

  ==============================================================================
*/

#include "BufferView.h"

BufferView::BufferView(SuperSlowAudioProcessor & p)
	: _processor(p)
{
	_history.resize(p.getBlockSize());
	startTimerHz(60);
}

BufferView::~BufferView()
{
}

void BufferView::resized()
{
}

void BufferView::paint(Graphics & g)
{
	int i = 0;
	float width = (float)getWidth();
	float height = (float)getHeight();

	float barWidth = 0.5f;

	auto sampleGradient = ColourGradient(
		Colour::fromRGB(33, 33, 33), width / 2.f, -58,
		Colour::fromRGB(198, 113, 0), width / 2.f, height, false);
	
	auto interpolatedGradient = ColourGradient(
		Colour::fromRGB(33, 33, 33), width / 2.f, -58,
		Colour::fromRGB(255, 160, 0), width / 2.f, height, false);

	for(auto it : _history)
	{
		g.setGradientFill(interpolatedGradient);
		g.fillRect((float)i, height - it * height, barWidth, height);
		i += 1;
	}

}

void BufferView::timerCallback()
{
	//TODO get values from _processor.mReadQueueL and _processor.mReadQueueR;
	//for now just push a random value

	static Random rand;

	auto buf = _processor.getBuffer();

	for(auto v : buf)
	{
		_history.push_back(abs(v));
	}
//	_history.push_back(rand.nextFloat());

	while(_history.size() > 512)
	{
		_history.pop_front();
	}
	repaint();

	_rate = 2; // replace with something like "getRate()"
}
