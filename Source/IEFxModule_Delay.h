// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"

#include "IEFxModule.h"

#include "juce_audio_basics/juce_audio_basics.h"

class IEFxModule_Delay : public IEFxModule
{
public:
    IEFxModule_Delay(const juce::String& Name) : IEFxModule(Name) {}

public:
    void SetDelayTime(float DelayTime);
    void SetFeedback(float Feedback);
    void SetMix(float Mix);

public:
    void Draw() override;

public:
    void prepareToPlay(double SampleRate, int SamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>& AudioBuffer, juce::MidiBuffer& MidiBuffer) override;

private:
    /* Real Time Modifiable Variables */
    std::atomic<float> m_DelayTime{0.5f};
    std::atomic<float> m_Feedback{0.5f};
    std::atomic<float> m_Mix{0.5f};

private:
    float m_SampleRate = 0.0f;
    float m_MaxDelayTime = 2.0f;
    int m_DelayBufferSize = 0;
    juce::AudioBuffer<float> m_DelayBuffer;
    std::vector<int> m_ChannelDelayWritePositionTracker;
};