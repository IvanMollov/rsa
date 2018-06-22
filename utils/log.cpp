#include "log.hpp"

#include <fstream>

Log::Log(bool quiet) noexcept :
	Log(std::clog, quiet)
{}
Log::Log(std::ostream& log, bool quiet, bool is_raii) :
	_log(&log), _is_quiet(quiet), _is_raii(is_raii)
{}
Log::Log(std::string& file_path, bool quiet) :
	Log(*(new std::ofstream(file_path, std::ofstream::app | std::ofstream::out)), quiet, true)
{}

Log::~Log()
{
	std::ofstream* file_log = dynamic_cast<std::ofstream*>(_log);

	if (file_log)
		file_log->close();
	if (_is_raii)
		delete _log;
}