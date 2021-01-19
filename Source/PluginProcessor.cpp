/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperSlowAudioProcessor::SuperSlowAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
	, mMode(Mode::Slow)
#endif
{
	File file("C:\\Users\\Jeff\\Music\\Diceworld-sinister_v1.wav");

	mFormatManager.registerBasicFormats();

	auto reader = mFormatManager.createReaderFor(file);
	auto source(new juce::AudioFormatReaderSource(reader, true));

	mTransportSource.setSource(source, 0, nullptr, reader->sampleRate);
	mReaderSource.reset(source);
}

SuperSlowAudioProcessor::~SuperSlowAudioProcessor()
{
}

//==============================================================================
const String SuperSlowAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool SuperSlowAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool SuperSlowAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool SuperSlowAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double SuperSlowAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int SuperSlowAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int SuperSlowAudioProcessor::getCurrentProgram()
{
	return 0;
}

void SuperSlowAudioProcessor::setCurrentProgram(int index)
{
}

const String SuperSlowAudioProcessor::getProgramName(int index)
{
	return {};
}

void SuperSlowAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void SuperSlowAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	mTransportSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void SuperSlowAudioProcessor::releaseResources()
{
	mTransportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SuperSlowAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void SuperSlowAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{

	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	int slowdown = 16; // max 100
	int speedUp = 16; // max 16

	ScopedNoDenormals noDenormals;

	if (mReaderSource.get() == nullptr)
	{
		buffer.clear();
	}

	if (!mTransportSource.isPlaying())
	{
		mTransportSource.setPosition(0.0);
		mTransportSource.start();
	}

	switch (mMode)
	{
	case SuperSlowAudioProcessor::Normal:

		while(mReadQueueL.size() < getBlockSize() || mReadQueueR.size() < getBlockSize())
		{
			mTransportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
			int numSamples = buffer.getNumSamples();
			
			for (int channel = 0; channel < totalNumInputChannels; ++channel)
			{
				auto* channelData = buffer.getReadPointer(channel);

				for (int i = 0; i < numSamples; ++i)
				{
					if (channel == 0)
					{
						mReadQueueL.push_back(channelData[i]);
					}
					if (channel == 1)
					{
						mReadQueueR.push_back(channelData[i]);
					}
				}
			}

			buffer.clear();

		}
		break;
	case SuperSlowAudioProcessor::Speed:
		while(mReadQueueL.size() < getBlockSize() || mReadQueueR.size() < getBlockSize())
		{
			mTransportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
			int numSamples = buffer.getNumSamples();

			for (int channel = 0; channel < totalNumInputChannels; ++channel)
			{
				auto* channelData = buffer.getReadPointer(channel);

				for (int i = 0; i < numSamples; i += speedUp)
				{
					if (channel == 0)
					{
						mReadQueueL.push_back(channelData[i]);
					}
					if (channel == 1)
					{
						mReadQueueR.push_back(channelData[i]);
					}
				}
			}
			buffer.clear();
		}
		break;
	case SuperSlowAudioProcessor::Slow:
		while(mReadQueueL.size() < getBlockSize() || mReadQueueR.size() < getBlockSize())
		{
			mTransportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));

			int numSamples = buffer.getNumSamples();

			for (int channel = 0; channel < totalNumInputChannels; ++channel)
			{
				auto* channelData = buffer.getReadPointer(channel);

				float stepSize = 0.f;

				for (int i = 0; i < numSamples; ++i)
				{
					for (int j = 0; j < slowdown; ++j)
					{
						if (i + 1 < getBlockSize())
						{
							float lerpTarget = channelData[i + 1];
							stepSize = (lerpTarget - channelData[i]) / (float)slowdown;
						}
						
						float interpolatedAmplitude = channelData[i] + stepSize * j;

						if (channel == 0)
						{
							mReadQueueL.push_back(interpolatedAmplitude);
						}
						if (channel == 1)
						{
							mReadQueueR.push_back(interpolatedAmplitude);
						}
					}
				}

			}

			buffer.clear();
		}
		break;
	default:
		break;
	}

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		for (int i = 0; i < getBlockSize(); ++i)
		{
			if (channel == 0)
			{
				channelData[i] = mReadQueueL.front();
				mReadQueueL.pop_front();
			}
			if (channel == 1)
			{
				channelData[i] = mReadQueueR.front();
				mReadQueueR.pop_front();
			}
		}
	}
}

//==============================================================================
bool SuperSlowAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SuperSlowAudioProcessor::createEditor()
{
	return new SuperSlowAudioProcessorEditor(*this);
}

//==============================================================================
void SuperSlowAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void SuperSlowAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

void SuperSlowAudioProcessor::setMode(const Mode & mode)
{
	mMode = mode;
}

SuperSlowAudioProcessor::Mode SuperSlowAudioProcessor::getMode() const
{
	return mMode;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SuperSlowAudioProcessor();
}
