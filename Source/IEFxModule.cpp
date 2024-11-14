// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEFxModule.h"

IEFxModule::IEFxModule(const juce::String& Name)
{
    m_Name = Name;
    m_ProgramNames.push_back(juce::String("Default"));
}

void IEFxModule::Draw()
{
    ImGui::SetSmartCursorPosYRelative(0.5f);
    ImGui::CenteredText(m_Name.toRawUTF8());
    ImGui::CenteredText(m_ProgramNames.back().toRawUTF8());
}

void IEFxModule::LinkModule(const std::shared_ptr<IEFxModule>& NextModule)
{
    UnlinkModule();
    if (NextModule)
    {
        m_NextModule = NextModule;
        m_NextModule->setPlayConfigDetails(getTotalNumInputChannels(), getTotalNumOutputChannels(), getSampleRate(), getBlockSize());
    }
}

void IEFxModule::UnlinkModule()
{
    if (m_NextModule)
    {
        m_NextModule->releaseResources();
    }
    m_NextModule.reset();
}

const juce::String IEFxModule::getName() const
{
    return m_Name;
}

double IEFxModule::getTailLengthSeconds() const
{
    return 0.0f;
}

bool IEFxModule::acceptsMidi() const
{
    return false;
}

bool IEFxModule::producesMidi() const
{
   return false;
}

juce::AudioProcessorEditor* IEFxModule::createEditor()
{
    return nullptr;
}

bool IEFxModule::hasEditor() const
{
    return false;
}

int IEFxModule::getNumPrograms()
{
    return m_ProgramNames.size();
}

int IEFxModule::getCurrentProgram()
{
    return m_CurrentProgramIndex;
}

void IEFxModule::setCurrentProgram(int Index)
{
    const int AvailablePrograms = m_ProgramNames.size();
    if (IEAssert(Index >= 0 && Index < AvailablePrograms))
    {
        m_CurrentProgramIndex = Index;
    }
    else
    {
        IELOG_WARNING("Program index %d is out of bounds, max programs available are %d", Index, AvailablePrograms);
    }
}

const juce::String IEFxModule::getProgramName(int Index)
{
    const int AvailablePrograms = m_ProgramNames.size();
    if (IEAssert(Index >= 0 && Index < AvailablePrograms))
    {
        return m_ProgramNames[Index];
    }
    return juce::String();
}

void IEFxModule::changeProgramName(int Index, const juce::String& NewName)
{
    const int AvailablePrograms = m_ProgramNames.size();
    if (IEAssert(Index >= 0 && Index < AvailablePrograms))
    {
        m_ProgramNames[Index] = NewName;
    }
}