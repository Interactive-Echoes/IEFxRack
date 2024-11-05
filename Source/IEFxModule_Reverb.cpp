// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxModule_Reverb.h"

void IEFxModule_Reverb::prepareToPlay(double SampleRate, int SamplesPerBlock)
{
    m_Reverb.setSampleRate(SampleRate);
}

void IEFxModule_Reverb::releaseResources()
{
}

void IEFxModule_Reverb::processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer)
{
    m_Reverb.processStereo(AudioBuffer.getWritePointer(0), AudioBuffer.getWritePointer(1), AudioBuffer.getNumSamples());

    if (m_NextModule)
    {
        m_NextModule->processBlock(AudioBuffer, MidiBuffer);
    }
}
