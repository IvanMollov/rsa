#include "frequency-table.hpp"
#include "utils.hpp"

#include <exception>

// experimental
#include <iostream>
#include <chrono>

#include <mutex>

using namespace std;

int main(int argc, const char* argv[])
{
	std::mutex m;

	auto start = chrono::high_resolution_clock::now();

	std::vector<std::string> arguments;
	arguments.assign(argv, argv + argc);

	if (!Utils::Options::has_option("-f", arguments)) {
		std::lock_guard<std::mutex> lock(m);

		std::cerr << "No file specified!" << endl;
		return -1;
	}
	
	string file_path = Utils::Options::get_option_value("-f", arguments);
	streampos file_size = Utils::File::get_file_size(file_path);
	if (file_size == std::streampos(-1)) {
		std::lock_guard<std::mutex> lock(m);

		std::cerr << "File " << file_path << " can not be opened for reading!" << endl;
		return -2;
	}

	unsigned int threads_count;

	if (!Utils::Options::has_option("-t", arguments)) {
		std::lock_guard<std::mutex> lock(m);

		std::cerr << "Number of threads not specified!" << endl;
		std::cerr << "Starting program with maximum threads available!" << endl;

		threads_count = std::thread::hardware_concurrency();
	}

	try {
		threads_count = std::stoi(Utils::Options::get_option_value("-t", arguments));
	}
	catch (const std::exception& e) {
		std::lock_guard<std::mutex> lock(m);

		cerr << "Can not parse thread count argument!" << endl;
		return -3;
	}

	bool is_quiet = Utils::Options::has_option("-q", arguments);

	FrequencyTable::Type ft;
	try {
		ft = FrequencyTable::get(file_path, threads_count, std::cout, is_quiet);
	}
	catch (const std::exception& e) {
		cerr << e.what() << endl;
		return -4;
	}

	//for (size_t i = 0; i < ft.size() - 1; ++i) {
	//	if (ft[i] != 0) {
	//		cout << static_cast<char>(i) << ": " << ft[i] << endl;
	//	}
	//}

	{
		std::lock_guard<std::mutex> lock(m);

		cout << "Total threads count: " << threads_count << endl;
		//cout << "Bytes processed count: " << ft[256] << endl;
		cout << "File size: " << file_size << endl;
	}

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	{
		std::lock_guard<std::mutex> lock(m);

		cout << "Total execution time: " << elapsed.count() << "s" << endl;
	}

	return 0;
}