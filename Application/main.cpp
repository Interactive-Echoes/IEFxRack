// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxRack.h"

#include "juce_events/juce_events.h"

int main()
{
    juce::MessageManager::getInstance()->setCurrentThreadAsMessageThread();
    IEFxRack IEFxRackApp;
    IERenderer& Renderer = IEFxRackApp.GetRenderer();
    if (Renderer.Initialize(std::string("IEFxRack")))
    {
        if (ImGuiContext* const CreatedImGuiContext = ImGui::CreateContext())
        {
            ImGuiIO& IO = ImGui::GetIO();
            IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_IsSRGB;
            if (Renderer.PostImGuiContextCreated())
            {
                ImGui::IEStyle::StyleIE();
                IO.IniFilename = nullptr;
                IO.LogFilename = nullptr;
                
                IEClock::time_point StartFrameTime = IEClock::now();
                IEDurationMs CapturedDeltaTime = IEDurationMs::zero();

                while (Renderer.IsAppRunning())
                {
                    StartFrameTime = IEClock::now();

                    Renderer.CheckAndResizeSwapChain();
                    Renderer.NewFrame();
                    ImGui::NewFrame();

                    // On Pre Frame Render
                    // Pre-Frame App Code Goes Here
                    IEFxRackApp.OnPreFrameRender();
                    Renderer.DrawTelemetry();
                    // On Pre Frame Render
                    
                    ImGui::Render();
                    Renderer.RenderFrame(*ImGui::GetDrawData());
                    Renderer.PresentFrame();

                    // On Post Frame Render
                    // Post-Frame App Code Goes Here
                    IEFxRackApp.OnPostFrameRender();
                    // On Post Frame Render

                    CapturedDeltaTime = std::chrono::duration_cast<IEDurationMs>(IEClock::now() - StartFrameTime);
                    // Captured delta time can be used to enforce a fixed frame rate

                    if (Renderer.IsAppWindowOpen())
                    {
                        Renderer.WaitEventsTimeout(0.1f);
                    }
                    else
                    {
                        Renderer.WaitEvents();
                    }
                }
            }
        }

        Renderer.Deinitialize();
    }

    return 0;
}