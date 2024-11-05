// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxModule_Oscillator.h"

void IEFxModule_Oscillator::SetFrequency(float Frequency)
{
    m_Osc.setFrequency(Frequency);
}

void IEFxModule_Oscillator::prepareToPlay(double SampleRate, int SamplesPerBlock)
{
    juce::dsp::ProcessSpec Spec;
    Spec.maximumBlockSize = SamplesPerBlock;
    Spec.sampleRate = SampleRate;
    Spec.numChannels = getTotalNumOutputChannels();
    
    m_Osc.prepare(Spec);
    m_Gain.prepare(Spec);
    
    m_Osc.setFrequency(440.0f);
    m_Gain.setGainLinear(0.5f);

    if (m_NextModule)
    {
        m_NextModule->prepareToPlay(SampleRate, SamplesPerBlock);
    }
}

void IEFxModule_Oscillator::releaseResources()
{
    if (m_NextModule)
    {
        m_NextModule->releaseResources();
    }
}

void IEFxModule_Oscillator::processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer)
{
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); i++)
    {
       AudioBuffer.clear(i, 0, AudioBuffer.getNumSamples()); 
    }
    
    juce::dsp::AudioBlock<float> AudioBlock{ AudioBuffer };
    m_Osc.process(juce::dsp::ProcessContextReplacing<float>(AudioBlock));
    m_Gain.process(juce::dsp::ProcessContextReplacing<float>(AudioBlock));

    if (m_NextModule)
    {
        m_NextModule->processBlock(AudioBuffer, MidiBuffer);
    }
}
