#pragma once

#include <mutex>
#include <string>
#include <iostream>
#include <cassert>

class Log
{
public:
	Log(bool quiet = false) noexcept;
	Log(std::ostream& log, bool quiet = false, bool is_raii = false);
	Log(std::string& file_path, bool quiet = false);

	Log(const Log&) = delete;
	Log(Log&& source) = delete;

	~Log();

public:
	Log& operator=(const Log&) = delete;
	Log& operator=(Log&& source) = delete;

	template <typename MessegeType>
	Log& operator<<(const MessegeType& messege) 
	{
		std::lock_guard<std::mutex> lock(_mutex);

		assert(_log);
		std::ostream& log = *_log;
		
		if (!_is_quiet) {
			log << messege;
		}

		return *this;
	}

public:
	bool is_quiet() const      { return _is_quiet; }
	void set_quiet(bool quiet) { _is_quiet = quiet; }

private:
	std::ostream* _log;
	mutable std::mutex _mutex;

	bool _is_raii;
	bool _is_quiet;
};