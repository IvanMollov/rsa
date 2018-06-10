#pragma once

#include <array>
#include <string>
#include <queue>

#include <thread>
#include <mutex>

#include <fstream>

#include <cstddef>
#include <cmath>

#include <iostream>

namespace FrequencyTable
{
	static const std::size_t FREQUENCY_TABLE_SIZE = 256;
	using Type = std::array<unsigned long long, FREQUENCY_TABLE_SIZE + 1>;

	void process(const std::string* file_path, std::streampos from, std::streampos to, Type* frequencies)
	{
		std::ifstream in(*file_path, std::ios::in | std::ios::binary);

		if (in.is_open()) {
			in.seekg(from);
			if (in.good()) {
				const std::size_t BUFFER_SIZE = 1024;
				char* buffer = new char[BUFFER_SIZE];

				while (in.good() && in.tellg() < to) {
					auto b = in.tellg();
					in.read(buffer, BUFFER_SIZE);
					auto a = std::min(in.gcount(), to - b + 1);
					for (std::size_t i = 0; i < a; ++i) {
						const unsigned char symbol_index = static_cast<const unsigned char>(buffer[i]);

						frequencies->at(symbol_index) += 1; 
						frequencies->at(256) += 1;
					}
				}	

				delete[] buffer;
			}

			in.close();
		}
	}

	inline Type get(const std::string& file_path, unsigned int threads_count)
	{
		std::vector<Type*> frequencies;

		std::streampos file_size = -1;
		std::ifstream file(file_path, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
		if (file.is_open()) {
			file_size = file.tellg();
			file.close();
		}

		const std::streampos segment_size = static_cast<const std::streampos>(std::ceil(file_size / static_cast<double>(threads_count)));

		std::vector<std::thread> workers;
		for (std::streampos segment_start = 0; segment_start < file_size; segment_start += segment_size + static_cast<std::streampos>(1)) {
			segment_start = segment_start <= file_size ? segment_start : file_size;
			//std::streampos segment_end = 62;
			std::streampos segment_end = segment_start + segment_size;
			segment_end = segment_end <= file_size ? segment_end : file_size;

			frequencies.push_back(new Type());
			frequencies.back()->fill(0);

			workers.push_back(std::thread(process, &file_path, segment_start, segment_end, frequencies.back()));

			//process(&file_path, segment_start, segment_end, frequencies.back());
		}
		for (std::thread& worker : workers) {
			if (worker.joinable()) {
				worker.join();
			}
		}

		Type frequency_all;
		frequency_all.fill(0);
		for (auto ft : frequencies) {
			for (std::size_t i = 0; i < ft->size(); ++i) {
				frequency_all.at(i) += ft->at(i);
			}
		}

		return frequency_all;
	}
}
