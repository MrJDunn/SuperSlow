/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Style.h"
#include "Components/Header.h"
#include "Components/BufferView.h"

//==============================================================================
/**
*/
class SuperSlowAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SuperSlowAudioProcessorEditor (SuperSlowAudioProcessor&);
    ~SuperSlowAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SuperSlowAudioProcessor& processor;

	Header _header;
	BufferView _bufferView;

private:

	Style style;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperSlowAudioProcessorEditor)
};
