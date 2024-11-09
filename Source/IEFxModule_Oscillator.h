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
    void SetFrequency(float Frequency);
    void SetGain(float Gain);

public:
    void prepareToPlay(double SampleRate, int SamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer) override;

private:
    juce::dsp::Oscillator<float> m_Osc{ [](float x)
    {return 2.0f * (x / juce::MathConstants<float>::twoPi) - 1.0f;} };
    juce::dsp::Gain<float> m_Gain;

private:
    int m_SampleCount = 0; // Test
    bool m_bGainOn = false;
};