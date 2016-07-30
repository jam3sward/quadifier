#ifndef hive_Event_h
#define hive_Event_h

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

#if defined(__GNUC__)
#include <tr1/memory>
#else
#include <memory>
#endif

//-----------------------------------------------------------------------------

/**
 * This class implements an Event object. Clients can wait for an event, and
 * the event can be signalled.
 */
class Event {
public:
    /// Default Constructor
    Event();

    /// Non-virtual destructor: do not inherit from this object
    ~Event();

    /**
     * Signal the event. This function returns immediately.
     */
    void signal();

    /**
     * Wait for the event to be signalled. This function blocks until the
     * event is signalled. If the event was signalled, it returns true.
     * An optional timeout parameter can be provided in milliseconds. If
     * the timeout expires without the event being signalled, the function
     * will return false. The default timeout value of 0 indicates that
     * the function should block indefinitely, until the event is signalled.
     */
    bool wait( unsigned milliseconds = 0 );

private:
    /// Copy construction is not supported
    Event( const Event & );

    /// Assignment is not supported
    Event & operator = ( const Event & );

    /// Private implementation
    struct Context;

    /// Private implementation
    std::shared_ptr<Context> self;
};

//-----------------------------------------------------------------------------

#endif//hive_Event_h
