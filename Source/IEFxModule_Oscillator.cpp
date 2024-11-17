// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxModule_Oscillator.h"

void IEFxModule_Oscillator::SetFrequency(int Frequency)
{
    m_Frequency.store(Frequency, std::memory_order_relaxed);
}

void IEFxModule_Oscillator::SetGain(float Gain)
{
    m_GainValue.store(Gain, std::memory_order_relaxed);
}

void IEFxModule_Oscillator::Draw()
{
    static int Frequency = m_Frequency.load(std::memory_order_relaxed);
    if (ImGui::SliderInt("Frequency", &Frequency, 20, 20000))
    {
        SetFrequency(Frequency);
    }

    static float GainValue = m_GainValue.load(std::memory_order_relaxed);
    if (ImGui::SliderFloat("Gain", &GainValue, 0.0f, 1.0f, "%.2f"))
    {
        SetGain(GainValue);
    }
}

void IEFxModule_Oscillator::prepareToPlay(double SampleRate, int SamplesPerBlock)
{
    juce::dsp::ProcessSpec Spec;
    Spec.maximumBlockSize = SamplesPerBlock;
    Spec.sampleRate = SampleRate;
    Spec.numChannels = getTotalNumOutputChannels();
    
    m_Osc.prepare(Spec);
    m_Gain.prepare(Spec);
    
    m_Osc.setFrequency(m_Frequency.load(std::memory_order_relaxed));
    m_Gain.setGainLinear(m_GainValue.load(std::memory_order_relaxed));

    if (m_NextModule)
    {
        m_NextModule->prepareToPlay(SampleRate, SamplesPerBlock);
    }
}

void IEFxModule_Oscillator::releaseResources()
{
    m_Osc.reset();
    m_Gain.reset();
    
    if (m_NextModule)
    {
        m_NextModule->releaseResources();
    }
}

void IEFxModule_Oscillator::processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer)
{
    const int SelectedFrequency = m_Frequency.load(std::memory_order_relaxed);
    if (m_Osc.getFrequency() != SelectedFrequency)
    {
        m_Osc.setFrequency(SelectedFrequency);
    }

    const float SelectedGainValue = m_GainValue.load(std::memory_order_relaxed);
    if (std::abs(SelectedGainValue - m_Gain.getGainLinear()) > 0.01f)
    {
        m_Gain.setGainLinear(SelectedGainValue);
    }

    AudioBuffer.clear();
    
    m_SampleCount += AudioBuffer.getNumSamples();
    if (m_SampleCount > getSampleRate())
    {
        m_SampleCount = 0;
        m_bGainOn = !m_bGainOn;
    }

    if (m_bGainOn)
    {
        juce::dsp::AudioBlock<float> AudioBlock{ AudioBuffer };
        m_Osc.process(juce::dsp::ProcessContextReplacing<float>(AudioBlock));
        m_Gain.process(juce::dsp::ProcessContextReplacing<float>(AudioBlock));
    }

    if (m_NextModule)
    {
        m_NextModule->processBlock(AudioBuffer, MidiBuffer);
    }
}
