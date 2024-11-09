// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxModule_Delay.h"

void IEFxModule_Delay::prepareToPlay(double SampleRate, int SamplesPerBlock)
{
    m_SampleRate = SampleRate;
    const int InputChannelNum = getTotalNumInputChannels();
    m_DelayBufferSize = SampleRate* m_MaxDelayTime;
    m_DelayBuffer = juce::AudioBuffer<float>(InputChannelNum, m_DelayBufferSize);
    m_ChannelDelayWritePositionTracker.resize(InputChannelNum);
    m_DelayBuffer.clear();
    
    if (m_NextModule)
    {
        m_NextModule->prepareToPlay(SampleRate, SamplesPerBlock);
    }
}

void IEFxModule_Delay::releaseResources()
{
    if (m_NextModule)
    {
        m_NextModule->releaseResources();
    }
}

void IEFxModule_Delay::processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer)
{
    const int ChannelNum = AudioBuffer.getNumChannels();
    const int AudioBufferSize = AudioBuffer.getNumSamples();
    
    const int DelayReadOffset = static_cast<int>(m_DelayTime * m_SampleRate);
    const int NormalizedReadPositionOffset = DelayReadOffset + m_DelayBufferSize;

    for (int Channel = 0; Channel < ChannelNum; Channel++)
    {
        float* ChannelBuffer = AudioBuffer.getWritePointer(Channel);
        float* DelayBuffer = m_DelayBuffer.getWritePointer(Channel);
        int& ChannelDelayWritePosition = m_ChannelDelayWritePositionTracker[Channel];

        for (int i = 0; i < AudioBufferSize; i++)
        {
            const float CapturedSample = ChannelBuffer[i];
            const int DelayReadPosition = (ChannelDelayWritePosition - DelayReadOffset + m_DelayBufferSize) % m_DelayBufferSize;

            const float CapturedDelayedSample = DelayBuffer[DelayReadPosition];
            DelayBuffer[ChannelDelayWritePosition] = CapturedSample + CapturedDelayedSample * m_Feedback;
            ChannelBuffer[i] = CapturedSample * (1.0f - m_Mix) + CapturedDelayedSample * m_Mix;
            ChannelDelayWritePosition = (ChannelDelayWritePosition + 1) % m_DelayBufferSize;
        }
    }

    if (m_NextModule)
    {
        m_NextModule->processBlock(AudioBuffer, MidiBuffer);
    }
}
