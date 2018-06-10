#include "frequency-table.hpp"

#include <exception>

// experimental
#include <iostream>
#include <chrono>
using namespace std;

int main(int argc, const char* argv[])
{
	try {
		auto start = std::chrono::high_resolution_clock::now();

		string file_path = "D:/dev/c++/src/rsa/huffman-coding/resource-files/lorem-ipsum.txt";

		std::streampos file_size = 0;
		std::ifstream file(file_path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
		if (file.is_open()) {
			file_size = file.tellg();
			file.close();
		}

		unsigned int threads_count = std::thread::hardware_concurrency();
		FrequencyTable::Type ft = FrequencyTable::get(file_path, threads_count);

		//FrequencyTable::Type ft;
		//ft.fill(0);
		//FrequencyTable::process(&file_path, 0, file_size, &ft);

		for (size_t i = 0; i < ft.size() - 1; ++i) {
			if (ft[i] != 0) {
				cout << static_cast<char>(i) << ": " << ft[i] << endl;
			}
		}

		std::cout << "Threads count: " << threads_count << std::endl;

		std::cout << "Bytes count: " << ft[256] << endl;

		std::cout << "File size: " << file_size << endl;

		auto finish = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsed = finish - start;
		std::cout << "Elapsed time: " << elapsed.count() << "s" << endl;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << endl;
	}


	return 0;
}