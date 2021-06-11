/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2020 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#if JUCE_ENABLE_ALLOCATION_HOOKS

namespace juce
{

static AllocationHooks& getAllocationHooksForThread()
{
    thread_local AllocationHooks hooks;
    return hooks;
}

void notifyAllocationHooksForThread()
{
    getAllocationHooksForThread().listenerList.call ([] (AllocationHooks::Listener& l)
    {
        l.newOrDeleteCalled();
    });
}

}

void* operator new (size_t s)
{
    juce::notifyAllocationHooksForThread();
    return std::malloc (s);
}

void* operator new[] (size_t s)
{
    juce::notifyAllocationHooksForThread();
    return std::malloc (s);
}

void operator delete (void* p) noexcept
{
    juce::notifyAllocationHooksForThread();
    std::free (p);
}

void operator delete[] (void* p) noexcept
{
    juce::notifyAllocationHooksForThread();
    std::free (p);
}

#if JUCE_CXX14_IS_AVAILABLE

void operator delete (void* p, size_t) noexcept
{
    juce::notifyAllocationHooksForThread();
    std::free (p);
}

void operator delete[] (void* p, size_t) noexcept
{
    juce::notifyAllocationHooksForThread();
    std::free (p);
}

#endif

namespace juce
{

//==============================================================================
UnitTestAllocationChecker::UnitTestAllocationChecker (UnitTest& test)
    : unitTest (test)
{
    getAllocationHooksForThread().addListener (this);
}

UnitTestAllocationChecker::~UnitTestAllocationChecker() noexcept
{
    getAllocationHooksForThread().removeListener (this);
    unitTest.expectEquals ((int) calls, 0, "new or delete was incorrectly called while allocation checker was active");
}

void UnitTestAllocationChecker::newOrDeleteCalled() noexcept { ++calls; }

}

#endif
