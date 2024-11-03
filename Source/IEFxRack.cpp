// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxRack.h"

#include "IEFxModule_Oscillator.h"

IEFxRack::IEFxRack() 
    :   m_Renderer(std::make_unique<IERenderer_Vulkan>()),
        m_AudioDeviceManager(std::make_unique<juce::AudioDeviceManager>())
{
    m_AudioDeviceManager->initialiseWithDefaultDevices(2, 2);
    m_AudioDeviceManager->addAudioCallback(this);
    
    // Test
    m_Modules.push_back(std::make_shared<IEFxModule_Oscillator>(juce::String("Default")));
    audioDeviceAboutToStart(m_AudioDeviceManager->getCurrentAudioDevice());
    setProcessor(m_Modules.front().get());
}

void IEFxRack::OnPreFrameRender()
{
    if (ImGui::Button("Play Test Sound"))
    {
        if (m_AudioDeviceManager)
        {
            m_AudioDeviceManager->playTestSound();
        }
    }
}

void IEFxRack::OnPostFrameRender()
{
    // Any post frame logic
}

void IEFxRack::Draw()
{
    for (int i = 0; i < m_Modules.size(); i++)
    {
        const std::shared_ptr<IEFxModule>& Module = m_Modules[i];
        if (Module)
        {
            // Draw at position i
            Module->Draw();
        }
    }
}
