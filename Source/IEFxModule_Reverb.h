// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"

#include "IEFxModule.h"

#include "juce_dsp/juce_dsp.h"

class IEFxModule_Reverb : public IEFxModule 
{
public:
    IEFxModule_Reverb(const juce::String& Name) : IEFxModule(Name)
    {
        juce::Reverb::Parameters Params;
        Params.roomSize = 1.0f;
        Params.dryLevel = 1.0f;
        m_Reverb.setParameters(Params);
    }

public:
    void prepareToPlay(double SampleRate, int SamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer) override;

private:
    juce::Reverb m_Reverb;
};