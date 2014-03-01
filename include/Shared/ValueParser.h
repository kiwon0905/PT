#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class ValueParser
{
public:
	ValueParser();
	~ValueParser();

	bool loadFromFile(const std::string& filename);
	bool close() const;

	bool isChanged() const;

	bool get(const std::string& param, std::string &value) const;
	bool get(const std::string& param, bool &value) const;
	bool get(const std::string& param, char &value) const;
	bool get(const std::string& param, int &value) const;
	bool get(const std::string& param, float &value) const;

	void set(const std::string& param, std::string value);
	void set(const std::string& param, bool value);
	void set(const std::string& param, char value);
	void set(const std::string& param, int value);
	void set(const std::string& param, float value);

	void printAll()
	{
		for (auto & d : m_data)
			std::cout << d.first << " = " << d.second << std::endl;
	}
private:
	int findIndex(const std::string& param) const;
	bool read();
	bool write() const;

	bool m_isChanged;
	std::string m_filename;
	std::vector<std::pair<std::string, std::string> > m_data;
	size_t m_size;
};
