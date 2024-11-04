// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_core/juce_core.h"

class IEFxModule : public juce::AudioProcessor
{
public:
    IEFxModule(const juce::String& Name);
    ~IEFxModule() override {}

public:
    virtual void Draw() const;

public:
    void LinkModule(const std::shared_ptr<IEFxModule>& NextModule);
    void UnlinkModule();

public:
    const juce::String getName() const override;
    double getTailLengthSeconds() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override {}
    void setStateInformation (const void* data, int sizeInBytes) override {}

protected:
    std::shared_ptr<IEFxModule> m_NextModule;

private:
    juce::String m_Name;
    std::vector<juce::String> m_ProgramNames;
    int m_CurrentProgramIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IEFxModule)
};