#include "CriticalSection.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <Multiprocessing.h>
#elif defined(__linux__)
#include <pthread.h>
#endif

//-----------------------------------------------------------------------------
//
// Copyright (C) 2011-14 James Ward, University of Hull
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//    1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software.
//
//    2. If you use this software in a product, an acknowledgment in the
//    product documentation is required.
//
//    3. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
//    4. This notice may not be removed or altered from any source
//    distribution.
//
//-----------------------------------------------------------------------------

struct CriticalSection::Context {
#if defined(_WIN32)
    CRITICAL_SECTION criticalSection;
#elif defined(__APPLE__)
    MPCriticalRegionID criticalSection;
#elif defined(__linux__)
    pthread_mutex_t criticalSection;
#endif
};

//-----------------------------------------------------------------------------

CriticalSection::CriticalSection() :
    self( new CriticalSection::Context )
{
#if defined(_WIN32)
    InitializeCriticalSectionAndSpinCount( &self->criticalSection, 0x400 );
#elif defined(__APPLE__)
    MPCreateCriticalRegion( &self->criticalSection );
#elif defined(__linux__)
    pthread_mutex_init( &self->criticalSection, 0 );
#endif
}

//-----------------------------------------------------------------------------

CriticalSection::~CriticalSection() {
#if defined(_WIN32)
    DeleteCriticalSection( &self->criticalSection );
#elif defined(__APPLE__)
    MPDeleteCriticalRegion( self->criticalSection );
#elif defined(__linux__)
    pthread_mutex_destroy( &self->criticalSection );
#endif
}

//-----------------------------------------------------------------------------

void CriticalSection::lock() {
#if defined(_WIN32)
    EnterCriticalSection( &self->criticalSection );
#elif defined(__APPLE__)
    MPEnterCriticalRegion( self->criticalSection, kDurationForever );
#elif defined(__linux__)
    pthread_mutex_lock( &self->criticalSection );
#endif
}

//-----------------------------------------------------------------------------

bool CriticalSection::tryLock() {
#if defined(_WIN32)
    return ( TryEnterCriticalSection( &self->criticalSection ) != 0 );
#elif defined(__APPLE__)
    return ( MPEnterCriticalRegion(
        self->criticalSection, kDurationImmediate
    ) == noErr );
#elif defined(__linux__)
    return ( pthread_mutex_trylock( &self->criticalSection ) == 0 );
#endif
}

//-----------------------------------------------------------------------------

void CriticalSection::unlock() {
#if defined(_WIN32)
    LeaveCriticalSection( &self->criticalSection );
#elif defined(__APPLE__)
    MPExitCriticalRegion( self->criticalSection );
#elif defined(__linux__)
    pthread_mutex_unlock( &self->criticalSection );
#endif
}

//-----------------------------------------------------------------------------
