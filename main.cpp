#include "frequency-table.hpp"
#include "utils.hpp"

#include <exception>

// experimental
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, const char* argv[])
{
	auto start = chrono::high_resolution_clock::now();

	string file_path = "D:/dev/c++/src/rsa/huffman-coding/resource-files/lorem-ipsum.txt";
	streampos file_size = Utils::File::get_file_size(file_path);

	unsigned int threads_count = thread::hardware_concurrency();
	FrequencyTable::Type ft = FrequencyTable::get(file_path, threads_count);

	for (size_t i = 0; i < ft.size() - 1; ++i) {
		if (ft[i] != 0) {
			cout << static_cast<char>(i) << ": " << ft[i] << endl;
		}
	}

	cout << "Threads count: " << threads_count << endl;
	cout << "Bytes count: " << ft[256] << endl;
	cout << "File size: " << file_size << endl;

	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	cout << "Elapsed time: " << elapsed.count() << "s" << endl;

	return 0;
}