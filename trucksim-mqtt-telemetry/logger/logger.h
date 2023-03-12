#pragma once

namespace trucksim_mqtt {

	enum LogLevel{
		kDebug = 10,
		kInfo = 20,
		kWarning = 30,
		kError = 40
	};
	
	/// <summary>
	/// Class defining basic logging interface.
	/// </summary>
	class Logger {
	public:
		// we have other constructors, so we have to have a default one
		Logger() = default;

		// forbbid accidental move/copying
		// See ISOCPP Guidelines C67
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(const Logger&&) = delete;

		virtual ~Logger() = default;

		virtual void log(const char* msg, LogLevel level) = 0;

		void debug(const char* msg) { log(msg, kDebug); }
		void info(const char* msg) { log(msg, kInfo); }
		void warning(const char* msg) { log(msg, kWarning); }
		void error(const char* msg) { log(msg, kError); }
	};
}