// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"

#include "IEFxModule.h"

#include "juce_dsp/juce_dsp.h"

class IEFxModule_Oscillator : public IEFxModule
{
public:
    IEFxModule_Oscillator(const juce::String& Name) : IEFxModule(Name) {}

public:
    void SetFrequency(int Frequency);
    void SetGain(float Gain);

public:
    void Draw() override;

public:
    void prepareToPlay(double SampleRate, int SamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer) override;

private:
    juce::dsp::Oscillator<float> m_Osc{ [](float x)
    {
        return static_cast<int>(2 * (x / juce::MathConstants<float>::twoPi) - 1);
    }};
    juce::dsp::Gain<float> m_Gain;

private:
    /* Real Time Modifiable Variables */
    std::atomic<int> m_Frequency{440};
    std::atomic<float> m_GainValue{ 0.2f };

private:
    int m_SampleCount = 0; // Test
    bool m_bGainOn = false;
};