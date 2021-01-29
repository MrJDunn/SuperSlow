/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <deque>

//==============================================================================
/**
*/
class SuperSlowAudioProcessor  : public AudioProcessor
{
public:

	enum Mode 
	{
		Norm = 0,
		Fast,
		Slow
	};
    //==============================================================================
    SuperSlowAudioProcessor();
    ~SuperSlowAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void setDelta(int delta);
	void setFile(const File&);

	Mode getMode() const;
	void setMode(const Mode& mode);

	void exportFile();
private:
	void playNorm(AudioBuffer<float>&, int);
	void playFast(AudioBuffer<float>&, int);
	void playSlow(AudioBuffer<float>&, int);

private:
	juce::AudioFormatManager mFormatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> mReaderSource;
	juce::AudioTransportSource mTransportSource;

	std::deque<float> mReadQueueL;
	std::deque<float> mReadQueueR;

	Mode mMode;

	File mFile;
	CriticalSection mCriticalSection;
	int mDelta;

	double mSampleRate = 44100.0;
	int mSamplesPerBlock = 512;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperSlowAudioProcessor)
};
