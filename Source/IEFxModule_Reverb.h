// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"
#include "IEConcurrency.h"

#include "IEFxModule.h"

#include "juce_audio_basics/juce_audio_basics.h"

class IEFxModule_Reverb : public IEFxModule 
{
public:
    IEFxModule_Reverb(const juce::String& Name) : IEFxModule(Name) {}

public:
    void SetReverbParams(const juce::Reverb::Parameters& Params);

public:
    void Draw() override;

public:
    void prepareToPlay(double SampleRate, int SamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer) override;

private:
    /* Real Time Modifiable Variables */
    IESpinOnWriteObject<juce::Reverb::Parameters> m_ReverbParams;

private:
    juce::Reverb m_Reverb;
};