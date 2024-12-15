// SPDX-License-Identifier: GPL-2.0-only
// Copyright © 2024 Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IECore.h"

template<typename T>
class IESpinOnWriteObject
{
public:
    IESpinOnWriteObject() : 
        m_ObjectStorage(std::make_unique<T>(T())),
        m_Object(m_ObjectStorage.get())
    {}
    
    const T* LockForRead()
    {
        return m_Object.exchange(nullptr);
    }

    void Unlock(const T& Object)
    {
        m_Object.store(&Object);
    }

    void Write(const T& NewObject)
    {
        std::unique_ptr<const T> NewObjectStorage = std::make_unique<T>(NewObject);
        for (const T* Expected = m_ObjectStorage.get(); !m_Object.compare_exchange_weak(Expected, NewObjectStorage.get());)
        {
            Expected = m_ObjectStorage.get();
        }
        m_ObjectStorage = std::move(NewObjectStorage);
    }

private:
    std::unique_ptr<const T> m_ObjectStorage;
    std::atomic<const T*> m_Object;
};