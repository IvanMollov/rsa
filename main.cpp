#include "./utils/utils.hpp"
#include "./utils/log.hpp"
#include "./app/app.hpp"

#include <iostream>


int main(int argc, const char* argv[])
{
	using namespace std;
	
	auto start = Utils::TimeUtils::now();

	vector<string> arguments;
	arguments.assign(argv, argv + argc);

	Log output(cout), log(clog), error_log(cerr);

	App* app = App::get_instance(arguments, output, log, error_log);
	if (!app) {
		error_log << "Errors occured! Terminating... T.T" << "\n";
		return -1;
	}

	app->execute();

	output << "Total threads count: " << app->get_threads_count() << "\n";
	//cout << "Bytes processed count: " << ft[256] << endl;
	streampos file_size = Utils::FileUtils::get_file_size(app->get_file_path());
	output << "File size: " << file_size << "\n";

	auto elapsed = Utils::TimeUtils::since(start);
	output << "Total execution time: " << elapsed.count() << "s" << "\n";

	return 0;
}