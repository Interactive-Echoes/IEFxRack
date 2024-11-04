// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"

#include "IEFxModule.h"

#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_audio_devices/juce_audio_devices.h"
#include "juce_core/juce_core.h"

class IEFxRack : public juce::AudioProcessorPlayer
{
public:
    IEFxRack();
    ~IEFxRack();
    IERenderer& GetRenderer() { return *m_Renderer; }
    juce::AudioDeviceManager& GetAudioDeviceManager() { return *m_AudioDeviceManager; }

public:
    void OnPreFrameRender();
    void OnPostFrameRender();

private:
    void Draw();

private:
    std::unique_ptr<IERenderer> m_Renderer;
    std::unique_ptr<juce::AudioDeviceManager> m_AudioDeviceManager;
    std::vector<std::shared_ptr<IEFxModule>> m_Modules;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IEFxRack)
};