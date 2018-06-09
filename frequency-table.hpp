#pragma once

#include <array>
#include <string>
#include <queue>

#include <thread>
#include <fstream>

class FrequencyTable
{
public:
	FrequencyTable() = delete;
	FrequencyTable(const std::string& file_path)
	{
		set_file_path(file_path);
		_frequencies.fill(0);
	}

	FrequencyTable(const FrequencyTable&) = delete;
	FrequencyTable(const FrequencyTable&&) = delete;

	~FrequencyTable()
	{
		join_workers();
	}

public:
	FrequencyTable& operator=(const FrequencyTable&) = delete;
	FrequencyTable& operator=(const FrequencyTable&&) = delete;

public:
	std::string get_file_path() const
	{
		return _file_path;
	}
	void set_file_path(const std::string& file_path)
	{
		_file_path = file_path;
		_is_updated = false;
	}

	std::array<unsigned long long, 255> get_frequencies() const
	{
		if (!_is_updated) {
			FrequencyTable* self = const_cast<FrequencyTable*>(this);
			self->generate_table();
			self->join_workers();

			_is_updated = true;
		}

		return _frequencies;
	}

private:
	void generate_table()
	{
		std::ifstream file;
		file.open(_file_path, std::ios::in | std::ios::binary);

		if (file.is_open()) {
			while (!file.eof()) {
				const std::streamsize BUFFER_SIZE = 8192;
				char buffer[BUFFER_SIZE];

				file.read(buffer, sizeof(buffer));
				std::streamsize read_size = file.gcount();

				//process(buffer, read_size);

				//std::thread worker([=]() { 
				//	this->process(std::move(buffer), read_size); 
				//});
				//worker.detach();

				//std::thread worker([=]() { 
				//	this->process(std::move(buffer), read_size);
				//});
				//if (worker.joinable())
				//	worker.join();

				_workers.push_back(std::thread([=]() { 
					this->process(buffer, read_size);
				}));
			}
		}

		file.close();
	}

	void process(const char buffer[], std::streamsize size)
	{
		for (size_t i = 0; i < size; ++i) {
			const unsigned char current_symbol = static_cast<const unsigned char>(buffer[i]);
			_frequencies[current_symbol] += 1;
		}
	}

	void join_workers()
	{
		for (auto& worker : _workers) {
			while (!worker.joinable());
			worker.join();
		}

		_workers.clear();
	}

private:
	std::string _file_path;
	std::array<unsigned long long, 255> _frequencies;
	std::vector<std::thread> _workers;

	mutable bool _is_updated;
};