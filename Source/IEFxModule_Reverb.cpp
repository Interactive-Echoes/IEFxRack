// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxModule_Reverb.h"

void IEFxModule_Reverb::SetReverbParams(const juce::Reverb::Parameters& Params)
{
    m_Reverb.setParameters(Params); // Not thread safe
}

void IEFxModule_Reverb::prepareToPlay(double SampleRate, int SamplesPerBlock)
{
    juce::Reverb::Parameters Params;
    Params.roomSize = 1.0f;
    Params.dryLevel = 1.0f;

    m_Reverb.setSampleRate(SampleRate);
    m_Reverb.setParameters(Params);

    if (m_NextModule)
    {
        m_NextModule->prepareToPlay(SampleRate, SamplesPerBlock);
    }
}

void IEFxModule_Reverb::releaseResources()
{
    m_Reverb.reset();

    if (m_NextModule)
    {
        m_NextModule->releaseResources();
    }
}

void IEFxModule_Reverb::processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer)
{
    if (AudioBuffer.getNumChannels() >= 2)
    {
        m_Reverb.processStereo(AudioBuffer.getWritePointer(0), AudioBuffer.getWritePointer(1), AudioBuffer.getNumSamples());
    }
    else
    {
        m_Reverb.processStereo(AudioBuffer.getWritePointer(0), AudioBuffer.getWritePointer(0), AudioBuffer.getNumSamples());
    }

    if (m_NextModule)
    {
        m_NextModule->processBlock(AudioBuffer, MidiBuffer);
    }
}
