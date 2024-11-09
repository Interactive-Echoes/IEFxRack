// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxRack.h"

#include "IEFxModule_Delay.h"
#include "IEFxModule_Oscillator.h"
#include "IEFxModule_Reverb.h"

IEFxRack::IEFxRack() 
    :   m_Renderer(std::make_unique<IERenderer_Vulkan>()),
        m_AudioDeviceManager(std::make_unique<juce::AudioDeviceManager>())
{
    if (m_AudioDeviceManager)
    {
        m_AudioDeviceManager->initialiseWithDefaultDevices(2, 2);
        m_AudioDeviceManager->addAudioCallback(this);

        m_Modules.resize(8);
        audioDeviceAboutToStart(m_AudioDeviceManager->getCurrentAudioDevice());
    }
}

IEFxRack::~IEFxRack()
{
    setProcessor(nullptr);
}

void IEFxRack::OnPreFrameRender()
{
    static constexpr uint32_t WindowFlags = ImGuiWindowFlags_NoResize |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoCollapse |
                                            ImGuiWindowFlags_NoTitleBar |
                                            ImGuiWindowFlags_NoScrollbar |
                                            ImGuiWindowFlags_NoScrollWithMouse;

    ImGuiViewport& MainViewport = *ImGui::GetMainViewport();
    const float WindowWidth = MainViewport.Size.x * 0.975f;
    const float WindowHeight = MainViewport.Size.y * 0.95f;
    const float WindowPosX = (MainViewport.Pos.x + (MainViewport.Size.x - WindowWidth) * 0.5f);
    const float WindowPosY = (MainViewport.Pos.y + (MainViewport.Size.y - WindowHeight) * 0.5f);

    ImGui::SetNextWindowSize(ImVec2(WindowWidth, WindowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(WindowPosX, WindowPosY));
    ImGui::Begin("Main Rack Window", nullptr, WindowFlags);
    Draw();
    ImGui::End();

    // if (ImGui::Button("Play Test Sound"))
    // {
    //     if (m_AudioDeviceManager)
    //     {
    //         m_AudioDeviceManager->playTestSound();
    //     }
    // }
}

void IEFxRack::OnPostFrameRender()
{
    // Any post frame logic
}

void IEFxRack::Draw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    static constexpr uint32_t WindowFlags = ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoCollapse |
                                            ImGuiWindowFlags_NoTitleBar;

    static constexpr uint32_t ChildWindowFlags = ImGuiChildFlags_Borders;

    int ModulesPerLine = 0;
    const int ModuleSize = m_Modules.size();
    for (int i = 0; i < ModuleSize; i++)
    {
        ImGui::PushID(i);
        std::string UniqueWindowName = std::format("Module{}", i);

        const ImVec2 ParentWindowSize = ImGui::GetWindowSize();
        const ImVec2 ParentWindowPadding = ImGui::GetStyle().WindowPadding;
        ImVec2 ChildWindowSize = ImVec2((ParentWindowSize.x / 4.0f) - (ImGui::GetStyle().WindowPadding.x * 1.25f),
                                        (ParentWindowSize.y / 2.0f) - (ImGui::GetStyle().WindowPadding.y * 1.25f));
        
        ImGui::BeginChild(UniqueWindowName.c_str(), ChildWindowSize, ChildWindowFlags, WindowFlags);
        std::shared_ptr<IEFxModule>& Module = m_Modules[i];
        if (Module)
        {
            // Draw at position i
            Module->Draw();
        }
        else
        {
            ImGui::SetSmartCursorPosRelative(ImVec2(0.5f, 0.5f));

            static const char* ModuleTypes[]{ "None", "Oscillator", "Input", "Reverb", "Delay", "Chorus" };
            int SelectedTypeInt = 0;
            if (ImGui::Combo("+", &SelectedTypeInt, ModuleTypes, IM_ARRAYSIZE(ModuleTypes)))
            {
                IEFxModuleType SelectedModuleType = static_cast<IEFxModuleType>(SelectedTypeInt);
                switch (SelectedModuleType)
                {
                    case IEFxModuleType::Oscillator:
                    {
                        Module = IEFxModule::CreateFxModule<IEFxModule_Oscillator>(juce::String("Oscillator"));
                        break;
                    }
                    case IEFxModuleType::Input:
                    {
                        break;
                    }
                    case IEFxModuleType::Reverb:
                    {
                        Module = IEFxModule::CreateFxModule<IEFxModule_Reverb>(juce::String("Reverb"));
                        break;
                    }
                    case IEFxModuleType::Delay:
                    {
                        Module = IEFxModule::CreateFxModule<IEFxModule_Delay>(juce::String("Delay"));
                        break;
                    }
                    case IEFxModuleType::Chorus:
                    {
                        break;
                    }
                    case IEFxModuleType::None:
                    case IEFxModuleType::Count:
                    {
                        break;
                    }
                }

                if (i == 0)
                {
                    setProcessor(Module.get());
                }
                else if (const std::shared_ptr<IEFxModule>& PrevModule = m_Modules[i - 1])
                {
                    PrevModule->LinkModule(Module);
                    if (m_AudioDeviceManager)
                    {
                        audioDeviceAboutToStart(m_AudioDeviceManager->getCurrentAudioDevice());
                    }
                }
            }
        }
        ImGui::EndChild();

        if (ModulesPerLine < (ModuleSize / 2) - 1)
        {
            ImGui::SameLine();
            ModulesPerLine++;
        }
        else
        {
            ModulesPerLine = 0;
        }
        ImGui::PopID();
    }

    ImGui::PopStyleVar(1);
}
