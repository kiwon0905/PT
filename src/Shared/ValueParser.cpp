#include "Shared/ValueParser.h"

#include <iostream>
#include <fstream>
#include <locale>
#include <algorithm>
#include <cctype>
#include <functional>


namespace std
{
	std::string to_string(const std::string & s)
	{
		return s;
	}
}

void ltrim(std::string & s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void rtrim(std::string & s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void trim(std::string & s)
{
	rtrim(s);
	ltrim(s);
}


ValueParser::ValueParser()
{
}


ValueParser::~ValueParser()
{
	close();
}

bool ValueParser::createNewFile(const std::string & s)
{
	close();
	std::ofstream fout(s);

	if (fout.good())
	{
		mFileName = s;
		return true;
	}
	return false;

}

bool ValueParser::loadFromFile(const std::string & s)
{
	close();

	std::ifstream fin(s);

	if (!fin.good())
	{
		std::cerr << "Error: Unable to open settings file: \"" << s << "\" for reading!" << std::endl;
		return false;
	}
	mFileName = s;
	while (fin.good())
	{
		std::string line, key, val;
		std::getline(fin, line);

		//if this is not a comment or an empty line
		if (line.size() > 0 && line[0] != '#')
		{
			std::size_t pos = line.find_first_of("=");
			key = line.substr(0, pos);
			val = line.substr(pos + 1);
			trim(key);
			trim(val);
		}
		else
		{
			key = line;
			val = "";
		}
		mData.push_back({ key, val });
	}
	return true;
}

bool ValueParser::close()
{
	if (mFileName != "")
	{
		std::ofstream fout(mFileName);
		mFileName = "";
		if (!fout.good())
		{
			mData.clear();
			return false;
		}

		for (auto & keyval : mData)
		{
			if (keyval.first.size() > 0 && keyval.first[0] != '#')
				fout << keyval.first << " = " << keyval.second << std::endl;
			else
				fout << keyval.first << std::endl;
		}
		mData.clear();
	}

	return true;
}

std::size_t ValueParser::getIndexOf(const std::string & key)
{
	for (std::size_t i = 0; i < mData.size(); ++i)
	{
		if (mData[i].first[0] == '#')
			continue;
		if (mData[i].first == key)
		{
			return i;
		}
	}
	return -1;
}

bool ValueParser::get(const std::string & key, float & val)
{
	int i = getIndexOf(key);
	if (i == -1)
		return false;
	val = std::stof(mData[i].second);
	return true;
}
bool ValueParser::get(const std::string & key, int & val)
{
	int i = getIndexOf(key);
	if (i == -1)
		return false;

	val = std::stoi(mData[i].second);
	return true;

}
bool ValueParser::get(const std::string & key, std::string & val)
{
	std::size_t i = getIndexOf(key);
	if (i == -1)
		return false;
	val = mData[i].second;
	return true;
}
bool ValueParser::get(const std::string & key, char & c)
{
	std::size_t i = getIndexOf(key);
	if (i == -1)
		return false;

	c = mData[i].second[0];
	return true;
}
bool ValueParser::get(const std::string & key, bool & b)
{
	auto iequals = [](const std::string& a, const std::string& b) ->bool
	{
		unsigned int sz = a.size();
		if (b.size() != sz)
			return false;
		for (unsigned int i = 0; i < sz; ++i)
			if (tolower(a[i]) != tolower(b[i]))
				return false;
		return true;
	};
	std::size_t i = getIndexOf(key);
	if (i == -1)
		return false;

	b = (iequals(mData[i].second, "true"));
	return true;

}