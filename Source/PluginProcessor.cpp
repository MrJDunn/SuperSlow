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
	, mMode(Mode::Norm)
	, mDelta(2)
	, mPlayState(PlayState::Paused)
	, mState(*this, nullptr, Identifier("SuperSlow"), 
		{
			std::make_unique<AudioParameterInt>("delta","Speed",1,16,2),
			std::make_unique<AudioParameterFloat>("wet","Wet",0.0,1.0,1.0),
			std::make_unique<AudioParameterChoice>("interpolation","Interpolation",StringArray("none","linear","random"),0),
			std::make_unique<AudioParameterChoice>("mode","Enabled",StringArray("none","fast","slow"),0)
		})
#endif
{
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

	mSampleRate = sampleRate;
	mSamplesPerBlock = samplesPerBlock;
	
	mHistoryBuffer.resize(mSamplesPerBlock);
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
	const ScopedLock myScopedLock(mCriticalSection);

	AudioBuffer<float> dry = buffer;

	mAudioPlayHead = getPlayHead();

	if(mAudioPlayHead)
	{
		mAudioPlayHead->getCurrentPosition(positionInfo);

		if (!positionInfo.isPlaying)
		{
			clearQueues();
		}

	}

	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	ScopedNoDenormals noDenormals;
	
	handleStateChange();

	//TODO: Replace with something that will keep timing better (qunatised to beats/bars)
	if(mReadQueueL.size() > 10000000)
	{
		clearQueues();
	}

	switch (mMode)
	{
	case SuperSlowAudioProcessor::Norm:
		playNorm(buffer, totalNumInputChannels);
		break;
	case SuperSlowAudioProcessor::Fast:
		playFast(buffer, totalNumInputChannels);
		break;
	case SuperSlowAudioProcessor::Slow:
		playSlow(buffer, totalNumInputChannels);
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
		for (int i = 0; i < mSamplesPerBlock; ++i)
		{
			if (channel == 0 && !mReadQueueL.empty())
			{
				channelData[i] = mReadQueueL.front();
				mHistoryBuffer.push_back(channelData[i]);
				mHistoryBuffer.pop_front();
				mReadQueueL.pop_front();
			}
			if (channel == 1 && !mReadQueueR.empty())
			{
				channelData[i] = mReadQueueR.front();
				mReadQueueR.pop_front();
			}
		}
	}

	// convolve dry and wet signals
	/*
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		auto* dryData = dry.getWritePointer(channel);
		for (int i = 0; i < mSamplesPerBlock; ++i)
		{
			channelData[i] = channelData[i] / getWet() + dryData[i] * getWet();
		}
	}
	*/
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
	std::unique_ptr<juce::XmlElement> xml(mState.copyState().createXml());
	copyXmlToBinary(*xml, destData);

	DBG(mState.copyState().toXmlString());
}

void SuperSlowAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mState.state.getType()))
			mState.replaceState(juce::ValueTree::fromXml(*xmlState));

	handleStateChange();

	DBG(mState.copyState().toXmlString());
}

int SuperSlowAudioProcessor::getDelta()
{
	const ScopedLock myScopedLock(mCriticalSection);

	return mDelta;
}

void SuperSlowAudioProcessor::setDelta(int delta)
{
	const ScopedLock myScopedLock(mCriticalSection);

	mDelta = delta;	

	clearQueues();
	mState.getParameterAsValue("delta").setValue(mDelta);

}

void SuperSlowAudioProcessor::setMode(const Mode& mode)
{
	const ScopedLock myScopedLock(mCriticalSection);

	mMode = mode;

	clearQueues();
	mState.getParameterAsValue("mode").setValue(mMode);

}

SuperSlowAudioProcessor::PlayState SuperSlowAudioProcessor::getPlayState() const
{
	return mPlayState;
}

void SuperSlowAudioProcessor::setPlayState(const PlayState & playState)
{
	const ScopedLock myScopedLock(mCriticalSection);

	mPlayState = playState;

	clearQueues();
}

SuperSlowAudioProcessor::Interpolation SuperSlowAudioProcessor::getInterpolation() const
{
	return mInterpolation;
}

void SuperSlowAudioProcessor::setInterpolation(const Interpolation & interpolation)
{
	const ScopedLock myScopedLock(mCriticalSection);

	mInterpolation = interpolation;
		
	clearQueues();
	mState.getParameterAsValue("interpolation").setValue(mInterpolation);

}

float SuperSlowAudioProcessor::getWet()
{
	return mWet;
}

void SuperSlowAudioProcessor::setWet(float wet)
{
	if(wet >= 0.0 && wet <= 1.0)
	{
		mWet = wet;
		
		mState.getParameterAsValue("wet").setValue(mWet);
	
	}
	else 
	{
		// Wet values are normalized (0 <= wet <= 1)
		jassertfalse;
	}
}

void SuperSlowAudioProcessor::playNorm(AudioBuffer<float>& buffer, int totalNumInputChannels)
{
		while(mReadQueueL.size() < mSamplesPerBlock || mReadQueueR.size() < mSamplesPerBlock )
		{
			//mTransportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
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
}

void SuperSlowAudioProcessor::playFast(AudioBuffer<float>& buffer, int totalNumInputChannels)
{
	while (mReadQueueL.size() < mSamplesPerBlock || mReadQueueR.size() < mSamplesPerBlock)
	{
		mTransportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));
		int numSamples = buffer.getNumSamples();

		for (int channel = 0; channel < totalNumInputChannels; ++channel)
		{
			auto* channelData = buffer.getReadPointer(channel);

			for (int i = 0; i < numSamples; i += mDelta)
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
}

void SuperSlowAudioProcessor::playSlow(AudioBuffer<float>& buffer, int totalNumInputChannels)
{
	static juce::Random r;
	//while (mReadQueueL.size() < mSamplesPerBlock || mReadQueueR.size() < mSamplesPerBlock)
	{
		//mTransportSource.getNextAudioBlock(AudioSourceChannelInfo(buffer));

		int numSamples = buffer.getNumSamples();

		for (int channel = 0; channel < totalNumInputChannels; ++channel)
		{
			auto* channelData = buffer.getReadPointer(channel);

			float stepSize = 0.f;

			for (int i = 0; i < numSamples; ++i)
			{
				for (int j = 0; j < mDelta; ++j)
				{
					if (i + 1 < mSamplesPerBlock)
					{
						float lerpTarget = mInterpolation == Interpolation::None ? channelData[i] : channelData[i + 1];
						stepSize = (lerpTarget - channelData[i]) / (float)mDelta;
					}

					float interpolatedAmplitude = channelData[i] + stepSize * j + (mInterpolation == Interpolation::Random ? r.nextFloat() / 750.0f : 0.0f);

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
}

void SuperSlowAudioProcessor::clearQueues()
{
	if(!mReadQueueL.empty())
	{
		mReadQueueL.clear();
	}
	if(!mReadQueueR.empty())
	{
		mReadQueueR.clear();
	}
}

void SuperSlowAudioProcessor::handleStateChange()
{
	float delta = (float)mState.getParameterAsValue("delta").getValue();
	float wet = (float)mState.getParameterAsValue("wet").getValue();
	float interpolation = (float)mState.getParameterAsValue("interpolation").getValue();
	float mode = (float)mState.getParameterAsValue("mode").getValue();

	if (delta != mDelta)
	{
		setDelta(delta);
	}
	if (wet != mWet)
	{
		setWet(wet);
	}
	if (interpolation != mInterpolation)
	{
		setInterpolation(Interpolation(int(interpolation)));
	}
	if (mode != mMode)
	{
		setMode(Mode(int(mode)));
	}

}

std::vector<float> SuperSlowAudioProcessor::getBuffer()
{
	const ScopedLock lock(mCriticalSection);

	std::vector<float> buf(mSamplesPerBlock);

	std::copy(mHistoryBuffer.begin(), mHistoryBuffer.end(), buf.begin());

	return buf;
}

SuperSlowAudioProcessor::Mode SuperSlowAudioProcessor::getMode() const
{
	const ScopedLock lock(mCriticalSection);
	return mMode;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SuperSlowAudioProcessor();
}
