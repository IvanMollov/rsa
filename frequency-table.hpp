#pragma once

#include "utils.hpp"

#include <string>
#include <array>
#include <vector>

#include <thread>

#include <fstream>

#include <cstddef>
#include <cmath>

#include <cassert>

namespace FrequencyTable
{
	static const std::size_t FREQUENCY_TABLE_SIZE = 256;
	using Type = std::array<unsigned long long, FREQUENCY_TABLE_SIZE + 1>;

	void count_symbols(char* buffer, std::size_t buffer_size, Type& frequencies)
	{
		for (std::size_t i = 0; i < buffer_size; ++i) {
			const unsigned char symbol_index = static_cast<const unsigned char>(buffer[i]);

			frequencies[symbol_index] += 1;
			frequencies[256] += 1; // count processed bytes
		}
	}

	void process(const std::string& file_path, std::streampos from, std::streampos to, Type& frequencies)
	{
		std::ifstream in(file_path, std::ios::in | std::ios::binary);

		if (in.is_open()) {
			in.seekg(from);
			if (in.good()) {
				const std::size_t BUFFER_SIZE = 1024 * 64;
				char buffer[BUFFER_SIZE];

				// read file[from; to]
				while (in.good() && in.tellg() < to) {
					const auto read_start = in.tellg();

					in.read(buffer, BUFFER_SIZE);
					// don't process bytes past to
					const auto read_size = std::min(in.gcount(), to - read_start + 1);

					count_symbols(buffer, read_size, frequencies);
				}	
			}

			in.close();
		}
	}

	inline Type merge_frequencies(const std::vector<Type> frequencies)
	{
		Type merged_frequencies;
		merged_frequencies.fill(0);
		for (auto ft : frequencies) {
			for (std::size_t i = 0; i < ft.size(); ++i) {
				merged_frequencies.at(i) += ft.at(i);
			}
		}

		return merged_frequencies;
	}

	inline Type get(const std::string& file_path, unsigned int threads_count)
	{
		std::streampos file_size = Utils::File::get_file_size(file_path);
		// segment the file in equal parts among every worker thread
		const std::streampos segment_size = static_cast<const std::streampos>(std::ceil(file_size / static_cast<double>(threads_count)));

		// frequency_tables[k] is the frequency table 
		// over which worker[k] is working
		// TODO: less detached from one another
		std::vector<Type> frequency_tables(threads_count);
		std::vector<std::thread> workers(threads_count);

		// start processing each segment in a new worker thread and in a new frequency table
		for (unsigned int i = 0; i < threads_count; ++i) {
			// TODO: Is this check necessery?
			std::streampos segment_start = i * (segment_size + static_cast<std::streampos>(!!i));
			segment_start = std::min(segment_start, file_size);
			std::streampos segment_end = segment_start + segment_size;
			segment_end = std::min(segment_end, file_size);

			frequency_tables[i].fill(0);
			assert(frequency_tables.size() <= threads_count);

			workers[i] = std::thread(process, std::ref(file_path), segment_start, segment_end, std::ref(frequency_tables[i]));
			assert(workers.size() <= threads_count);

			assert(frequency_tables.size() == workers.size());
		}
		// wait all threads to finish 
		for (std::thread& worker : workers) {
			if (worker.joinable())
				worker.join();
		}

		// accumulate final result
		return merge_frequencies(frequency_tables);
	}
}
