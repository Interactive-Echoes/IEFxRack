// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxModule_Reverb.h"

void IEFxModule_Reverb::SetReverbParams(const juce::Reverb::Parameters& Params)
{
    m_ReverbParams.Write(Params);
}

void IEFxModule_Reverb::Draw()
{
    static juce::Reverb::Parameters ReverbParams;
    bool bHasChanged = false;

    static float MixLevel = 0.5f;
    if (ImGui::SliderFloat("Mix Level", &MixLevel, 0.0f, 1.0f, "%.2f"))
    {
        ReverbParams.wetLevel = MixLevel;
        ReverbParams.dryLevel = 1 - MixLevel;
        bHasChanged = true;
    }
    if (ImGui::SliderFloat("Room Size", &ReverbParams.roomSize, 0.0f, 1.0f, "%.2f"))
    {
        bHasChanged = true;
    }
    if (ImGui::SliderFloat("Damping", &ReverbParams.damping, 0.0f, 1.0f, "%.2f"))
    {
        bHasChanged = true;
    }
    if (ImGui::SliderFloat("Width", &ReverbParams.width, 0.0f, 1.0f, "%.2f"))
    {
       bHasChanged = true;
    }

    if (bHasChanged)
    {
        SetReverbParams(ReverbParams);
    }
}

void IEFxModule_Reverb::prepareToPlay(double SampleRate, int SamplesPerBlock)
{
    m_Reverb.setSampleRate(SampleRate);
    m_Reverb.setParameters(m_ReverbParams.LockForRead().Value);

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
    m_Reverb.setParameters(m_ReverbParams.LockForRead().Value);

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
