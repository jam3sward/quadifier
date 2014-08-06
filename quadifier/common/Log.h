#ifndef quadifier_log_h
#define quadifier_log_h

//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------------

/// Logging class
class Log {
private:
	/// Constructor
	Log();

public:
	/// Destructor
	virtual ~Log();

	/// Returns the single instance of the log
	static Log & get();
	
	/// Open log file. If append is false, a new log file is created (any
	/// existing log file is replaced). If append is true, new log messages
	/// are appended to the existing file. When the class is constructed,
	/// the default log file name is 'log.txt' and append is false.
	/// Returns true if the log is successfully opened, false otherwise.
	bool open( const std::string & fileName, bool append = false );
	
	/// Close log file
	void close();
	
	/// Returns the current filename
	const std::string getFileName() const;
	
	/// Returns true if the log is open
	bool isOpen() const;

	/// Returns true if the specified logging level is enabled
	static bool level( int value );

	/// Returns the stream used for output
	static std::ostream & stream();

	/// Returns true if logging is disabled
	static bool disabled() { return level(Log::Disabled); }

	/// Returns true if error level logging is enabled
	static bool error() { return level(Log::Error); }

	/// Returns true if warning level logging is enabled
	static bool warning() { return level(Log::Warning); }

	/// Returns true if detailed logging is enabled
	static bool detailed() { return level(Log::Detailed); }

	/// Returns true if verbose logging is enabled
	static bool verbose() { return level(Log::Verbose); }

	/// Returns the current logging level
	int getLevel() const;

	/// Sets the current logging level
	Log & setLevel( int level );

	/// Returns the stream used for output
	std::ostream & getStream();

	enum {
		Disabled = 0,	///< No logging at all
		Error	 = 1,	///< Errors only
		Warning  = 2,	///< Warning also
		Detailed = 3,	///< Detailed/informational logging
		Verbose  = 4	///< Very detailed, verbose logging
	};

private:
	std::ofstream m_fileStream;	///< Log file stream
	std::string	m_fileName;		///< Log file name
	bool m_append;				///< Append or replace log
	int m_level;				///< Log message level
};

//-----------------------------------------------------------------------------

Log::Log() :
	m_fileName( "log.txt" ),
	m_append( false ),
	m_level( Log::Verbose )
{
}

//-----------------------------------------------------------------------------

Log::~Log()
{
	close();
}

//-----------------------------------------------------------------------------

inline Log & Log::get()
{
	static Log instance;
	return instance;
}

//-----------------------------------------------------------------------------

inline bool Log::open( const std::string & fileName, bool append )
{
	// close existing log
	close();
	
	// set filename and append flag
	m_fileName = fileName;
	m_append = append;
	
	// get the stream to cause file to be lazily opened
	getStream();
	
	// returns true if the log was successfully opened
	return isOpen();
}

//-----------------------------------------------------------------------------

inline void Log::close()
{
	if ( m_fileStream.is_open() )
		m_fileStream.close();
}

//-----------------------------------------------------------------------------

inline const std::string Log::getFileName() const
{
	return m_fileName;
}

//-----------------------------------------------------------------------------

inline bool Log::isOpen() const
{
	return m_fileStream.is_open();
}

//-----------------------------------------------------------------------------

inline bool Log::level( int value )
{
	return get().getLevel() >= value;
}

//-----------------------------------------------------------------------------

inline std::ostream & Log::stream()
{
	return get().getStream();
}

//-----------------------------------------------------------------------------

inline int Log::getLevel() const
{
	return m_level;
}

//-----------------------------------------------------------------------------

inline Log & Log::setLevel( int level )
{
	m_level = level;
	return *this;
}

//-----------------------------------------------------------------------------

inline std::ostream & Log::getStream()
{
	// if file is not already open
	if ( !m_fileStream.is_open() ) {
		// attempt to open file
		// i.e. the file is only opened the first time the caller tries to
		// make use of logging
		m_fileStream.open(
			m_fileName.c_str(),
			std::ofstream::out |
			( m_append ? std::ofstream::app : std::ofstream::trunc )
		);
	}
	
	// return stream to caller
	return m_fileStream;
}

//-----------------------------------------------------------------------------

#endif//quadifier_log_h

