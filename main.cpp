#include "frequency-table.hpp"

// experimental
#include <iostream>
#include <chrono>
using namespace std;

int main(int argc, const char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();

	FrequencyTable ft("D:/dev/c++/src/rsa/huffman-coding/resource-files/lorem-ipsum.txt");

	auto freq = ft.get_frequencies();
	for (size_t i = 0; i < freq.size(); ++i) {
		if (freq[i] != 0) {
			cout << static_cast<char>(i) << ": " << freq[i] << endl;
		}
	}

	auto finish = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s" << endl;

	return 0;
}