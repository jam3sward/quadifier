#include "Log.h"
#include <thread>
#include <mutex>
#include <time.h>

using namespace hive;

//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-14 James Ward, University of Hull
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

Log & Log::get()
{
    static Log instance;
    return instance;
}

//-----------------------------------------------------------------------------

Log::~Log()
{
    close();
}

//-----------------------------------------------------------------------------

Log::Log() :
    m_level( Level::Info )
{
}

//-----------------------------------------------------------------------------

bool Log::open( const std::string & fileName )
{
    // close existing
    close();

    // open stream for writing
    get().m_stream.open( fileName.c_str() );
    if ( !get().m_stream ) return false;

    return true;
}

//-----------------------------------------------------------------------------

void Log::close()
{
    if ( get().m_stream.is_open() ) get().m_stream.close();
}

//-----------------------------------------------------------------------------

bool Log::info()
{
    return get().getLevel() >= Level::Info;
}

//-----------------------------------------------------------------------------

bool Log::warnings()
{
    return get().getLevel() >= Level::Warning;
}

//-----------------------------------------------------------------------------

bool Log::errors()
{
    return get().getLevel() >= Level::Error;
}

//-----------------------------------------------------------------------------

bool Log::verbose()
{
    return get().getLevel() >= Level::Verbose;
}

//-----------------------------------------------------------------------------

std::ostream & Log::print( const std::string & text )
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock( mutex );

    unsigned timeStamp = static_cast<unsigned>(
        1000.0 *
        static_cast<double>( clock() ) / static_cast<double>( CLOCKS_PER_SEC )
        + 0.5
    );

    get().m_stream << timeStamp << ": " << text;

    get().m_stream.flush();

    return get().m_stream;
}

//-----------------------------------------------------------------------------

std::ostream & Log::out()
{
    return get().m_stream;
}

//-----------------------------------------------------------------------------

Log & Log::setLevel( Log::Level level )
{
    m_level = level;
    return *this;
}

//-----------------------------------------------------------------------------

Log::Level Log::getLevel() const
{
    return m_level;
}

//-----------------------------------------------------------------------------
