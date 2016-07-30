#ifndef hive_CriticalSection_h
#define hive_CriticalSection_h

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

#include <memory>

//-----------------------------------------------------------------------------

/**
 * This class implements a Critical Section
 */
class CriticalSection {
public:
    /// Default Constructor
    CriticalSection();

    /// Non-virtual destructor: do not inherit from this object
    ~CriticalSection();

    /**
     * Lock the Critical Section. This function waits until it
     * acquires ownership of the Critical Section.
     */
    void lock();

    /**
     * Try to lock the Critical Section, returning true if successful,
     * false otherwise. If the Critical Section is already in use, this
     * function will return immediately.
     */
    bool tryLock();

    /**
     * Unlock the Critical Section. Relinquishes ownership of the
     * Critical Section.
     */
    void unlock();

    class Scope {
    public:
        Scope( CriticalSection & section ) : m_section( section ) {
            m_section.lock();
        }

        ~Scope() {
            m_section.unlock();
        }

    private:
        Scope( const Scope & );

        Scope & operator = ( const Scope & );

        CriticalSection & m_section;
    };

private:
    /// Copy construction is not supported
    CriticalSection( const CriticalSection & );

    /// Assignment is not supported
    CriticalSection & operator = ( const CriticalSection & );

    /// Private implementation
    struct Context;

    /// Private implementation
    std::shared_ptr<Context> self;
};

//-----------------------------------------------------------------------------

#endif//hive_CriticalSection_h
