#pragma once

#include <vector>
#include <iostream>
#include <string>

class ValueParser
{
public:
	ValueParser();
	~ValueParser();


	bool loadFromFile(const std::string & name);
	bool createNewFile(const std::string & name);
	bool close();

	template <class T>
	void set(const std::string & key, const T & value);

	bool get(const std::string & key, float & val);
	bool get(const std::string & key, int & val);
	bool get(const std::string & key, std::string & val);
	bool get(const std::string & key, char & c);
	bool get(const std::string & key, bool & b);

	void printAll()
	{
		for (auto & val : mData)
			std::cout << val.first <<" "<< val.second << "\n";
	}
private:
	std::size_t getIndexOf(const std::string & key);

	std::string mFileName;
	std::vector<std::pair<std::string, std::string>> mData;
};

namespace std
{
	std::string to_string(const std::string & s);
}

template <class T>
void ValueParser::set(const std::string & key, const T & value)
{
	if (mFileName != "")
	{
		std::size_t i = getIndexOf(key);
		if (i != -1)
			mData[i].second = std::to_string(value);
		else
			mData.push_back({ key, std::to_string(value) });
	}
}