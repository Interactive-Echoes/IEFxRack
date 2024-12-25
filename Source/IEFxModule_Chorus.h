// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"

#include "IEFxModule.h"

class IEFxModule_Chorus : public IEFxModule 
{
public:
    IEFxModule_Chorus(const juce::String& Name) : IEFxModule(Name) {}

public:
    void prepareToPlay(double SampleRate, int SamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer) override;
};