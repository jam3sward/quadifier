#ifndef hive_Log_h
#define hive_Log_h

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

#include <string>
#include <fstream>

//-----------------------------------------------------------------------------

namespace hive {

/**
 * Simple logging singleton class.
 */
class Log {
public:
    /// Virtual Destructor
    virtual ~Log();

    /// Provides access to the single instance
    static Log & get();

    /// Open the specified log file for output
    static bool open( const std::string & fileName = "log.txt" );

    /// Close the log file
    static void close();

    /// Print a message to the log
    static std::ostream & print( const std::string & text = "" );

    /// Provides access to the log output stream
    static std::ostream & out();

private:
    /// Default Constructor
    Log();

    std::ofstream m_stream; ///< the log output stream
};

} // namespace hive

//-----------------------------------------------------------------------------

#endif//hive_Log_h