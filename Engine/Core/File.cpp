#include "pch.h"
#include "File.h"

#include <iostream>
#include <sstream>
#include <fstream>

namespace nu{
	std::string GetWorkingDirectory() {
		std::error_code ec;
		auto path = std::filesystem::current_path(ec);

		if (ec) { std::cerr << ec.message() << std::endl;}

		return ec ? std::string{} : path.string();
	}

	bool SetWorkingDirectory(const std::string& path) {
		std::error_code ec;
		std::filesystem::current_path(path, ec);

		if (ec) { std::cerr << ec.message() << std::endl; }

		return !ec;
	}

	std::string GetFilename(const std::string& path) {
		return std::filesystem::path{ path }.filename().string();
	}

	std::string GetFileExtension(const std::string& path) {
		return std::filesystem::path{ path }.extension().string();
	}

	std::string GetFilenameNoExtension(const std::string& path) {
		return std::filesystem::path{ path }.stem().string();
	}

	bool FileExists(const std::string& path) {
		std::error_code ec;
		bool result = std::filesystem::exists(path, ec);

		if (ec) {
			std::cerr << ec.message() << std::endl;
		}

		return !ec && result;
	}

	std::vector<std::string> GetFilesInDirectory(const std::string& path) {
		std::vector<std::string> files;
		std::error_code ec;

		auto iter = std::filesystem::directory_iterator(path, ec);

		if (ec) {
			std::cerr << ec.message() << std::endl;
		}

		for (const auto& entry : iter) {
			if (entry.is_regular_file(ec) && !ec) {
				files.push_back(entry.path().string());
			}
		}
		return files;
	}

	std::vector<std::string> GetDirectoriesIn(const std::string& path) {
		std::vector<std::string> directories;
		std::error_code ec;

		auto iter = std::filesystem::directory_iterator(path, ec);

		if (ec) {
			std::cerr << ec.message() << std::endl;
			return directories;
		}

		for (const auto& entry : iter) {
			if (entry.is_directory(ec) && !ec) {
				directories.push_back(entry.path().string());
			}
		}

		return directories;
	}

	bool ReadTextFile(const std::string& path, std::string& data) {
		std::ifstream file(path);
		if (!file.is_open()) {
			return false;
		}

		std::stringstream ss;
		ss << file.rdbuf();

		data = ss.str();

		return true;
	}

	bool WriteTextFile(const std::string& path, const std::string& data, bool append) {
		std::ios::openmode mode = append ? std::ios::app : std::ios::out;
		std::ofstream file(path, mode);

		if (!file.is_open()) {
			return false;
		}

		file << data;

		return true;
	}

	std::vector<uint8_t> ReadBinaryFile(const std::string& path)
	{
		// open the file in binary mode (no newline conversion) with the read position at the end (ate = "at end")
		std::ifstream file(path, std::ios::binary | std::ios::ate);

		// if the file could not be opened, return an empty vector
		if (!file.is_open())
		{
			return {};
		}

		// the read position is at the end, so tellg() gives the file size in bytes
		std::streamsize size = file.tellg();

		// move the read position back to the beginning so the read starts at the first byte
		file.seekg(0, std::ios::beg);

		// create a vector large enough to hold every byte in the file
		std::vector<uint8_t> bytes(size);

		// read all bytes into the vector
		// read() takes a char*, so reinterpret_cast the uint8_t* from bytes.data()
		if (!file.read(reinterpret_cast<char*>(bytes.data()), size))
		{
			// the read failed, so return an empty vector
			return {};
		}

		// return the bytes read from the file
		return bytes;
	}


}