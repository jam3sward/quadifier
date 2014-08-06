#include "Event.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <time.h>
#include <pthread.h>
#endif

#include <assert.h>

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

struct Event::Context {
#if defined(_WIN32)

    HANDLE eventHandle;

#elif defined(__linux__)

    pthread_mutex_t mutex;      ///< the mutex
    pthread_cond_t condition;   ///< condition variable to wait on
    bool signalled;             ///< is the event signalled?
    unsigned waiting;           ///< number of waiting threads

#endif
};

//-----------------------------------------------------------------------------

Event::Event() :
    self( new Event::Context )
{
#if defined(_WIN32)

    // create an event object
    self->eventHandle = CreateEvent(
        NULL,   // default security attributes
        FALSE,  // auto-reset event object
        FALSE,  // initial state is not signalled
        NULL    // object is not named
    );

    // ensure that event object was created successfully (in debug builds)
    assert( self->eventHandle != 0 );

#elif defined(__linux__)

    self->signalled = false;
    self->waiting   = 0;
    
    // initialise mutex
    pthread_mutex_init( &self->mutex, 0 );

    // initialise condition variable
    pthread_cond_init( &self->condition, 0 );

#endif
}//Event

//-----------------------------------------------------------------------------

Event::~Event()
{
#if defined(_WIN32)

    // destroy the event object
    CloseHandle( self->eventHandle );

#elif defined(__linux__)

    pthread_mutex_destroy( &self->mutex );
    pthread_cond_destroy( &self->condition );

#endif
}

//-----------------------------------------------------------------------------

void Event::signal()
{
#if defined(_WIN32)

    // set the event to the signalled state
    SetEvent( self->eventHandle );

#elif defined(__linux__)

    // lock the mutex first
    pthread_mutex_lock( &self->mutex );

    if ( self->waiting > 0 ) {
        // threads waiting: wake up waiting thread
        pthread_cond_signal( &self->condition );
    } else {
        // no threads waiting: signal the event
        self->signalled = true;
    }
    
    // release the mutex
    pthread_mutex_unlock( &self->mutex );

#endif
}

//-----------------------------------------------------------------------------

bool Event::wait( unsigned milliseconds )
{
#if defined(_WIN32)

    // wait for the event to be signalled, return true if signalled,
    // return false if the wait timed out or was abandoned
    return ( WaitForSingleObject(
        self->eventHandle,
        static_cast<DWORD>( milliseconds )
    ) == WAIT_OBJECT_0 );

#elif defined(__linux__)

    // lock the mutex first
    pthread_mutex_lock( &self->mutex );

    // is the event signalled?
    if ( self->signalled ) {
        // automatically reset
        self->signalled = false;
    } else {
        // not signalled

        // increment number of waiting threads
        self->waiting++;
        
        // has a timeout been specified?
        if ( milliseconds == 0 ) {
            // no timeout specified: wait indefinitely
            pthread_cond_wait( &self->condition, &self->mutex );
        } else {
            // timeout specified: we will use a timed wait

            // get current time, to calculate absolute expiry time
            struct timespec expiry;
            clock_gettime( CLOCK_REALTIME, &expiry );
            
            // increment nanoseconds by user specified timeout
            expiry.tv_nsec += 1000L * static_cast<long>( milliseconds );

            // carry into seconds
            if ( expiry.tv_nsec >= 1000000000 ) {
                expiry.tv_nsec -= 1000000000;
                expiry.tv_sec++;
            }

            // wait with timeout
            pthread_cond_timedwait( &self->condition, &self->mutex, &expiry );
        }

        // decrement number of waiting threads
        self->waiting--;
    }
    
    // release the mutex
    pthread_mutex_unlock( &self->mutex );

#endif
}//wait

//-----------------------------------------------------------------------------
