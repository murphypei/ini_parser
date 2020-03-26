// Read an INI file into easy-to-access name/value pairs.

// SPDX-License-Identifier: BSD-3-Clause

// Copyright (C) 2009-2020, Ben Hoyt

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// https://github.com/benhoyt/inih

#include "ini_reader.h"
#include "ini.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>

using std::string;

INIReader::INIReader(const string &filename) { _error = ini_parse(filename.c_str(), ValueHandler, this); }

INIReader::INIReader(const char *buffer, size_t buffer_size)
{
    string content(buffer, buffer_size);
    _error = ini_parse_string(content.c_str(), ValueHandler, this);
}

int INIReader::ParseError() const { return _error; }

string INIReader::Get(const string &section, const string &name, const string &default_value) const
{
    string key = MakeKey(section, name);
    // Use _values.find() here instead of _values.at() to support pre C++11 compilers
    return _values.count(key) ? _values.find(key)->second : default_value;
}

string INIReader::GetString(const string &section, const string &name, const string &default_value) const
{
    const string str = Get(section, name, "");
    return str.empty() ? default_value : str;
}

long INIReader::GetInteger(const string &section, const string &name, long default_value) const
{
    string valstr = Get(section, name, "");
    const char *value = valstr.c_str();
    char *end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double INIReader::GetReal(const string &section, const string &name, double default_value) const
{
    string valstr = Get(section, name, "");
    const char *value = valstr.c_str();
    char *end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool INIReader::GetBoolean(const string &section, const string &name, bool default_value) const
{
    string valstr = Get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

std::vector<int> INIReader::GetIntArray(const std::string &section, const std::string &name) const
{
    std::string valstr = Get(section, name, "");
    std::vector<int> result;
    if (!valstr.empty())
    {
        std::vector<std::string> strs;
        this->stringSplit(valstr, ",", strs);
        for (auto &s : strs)
        {
            result.push_back(std::stoi(s));
        }
    }
    return result;
}

std::vector<std::string> INIReader::GetStrArray(const std::string &section, const std::string &name) const
{
    std::string valstr = Get(section, name, "");
    std::vector<string> result;
    if (!valstr.empty())
    {
        stringSplit(valstr, ",", result);
    }
    return result;
}

std::vector<float> INIReader::GetRealArray(const std::string &section, const std::string &name) const
{
    std::string valstr = Get(section, name, "");
    std::vector<float> result;
    if (!valstr.empty())
    {
        std::vector<std::string> strs;
        stringSplit(valstr, ",", strs);
        for (auto &s : strs)
        {
            result.push_back(std::stof(s));
        }
    }
    return result;
}

bool INIReader::HasSection(const string &section) const
{
    const string key = MakeKey(section, "");
    std::map<string, string>::const_iterator pos = _values.lower_bound(key);
    if (pos == _values.end())
        return false;
    // Does the key at the lower_bound pos start with "section"?
    return pos->first.compare(0, key.length(), key) == 0;
}

bool INIReader::HasValue(const string &section, const string &name) const
{
    string key = MakeKey(section, name);
    return _values.count(key);
}

string INIReader::MakeKey(const string &section, const string &name)
{
    string key = section + "=" + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

int INIReader::ValueHandler(void *user, const char *section, const char *name, const char *value)
{
    if (!name) // Happens when INI_CALL_HANDLER_ON_NEW_SECTION enabled
        return 1;
    INIReader *reader = static_cast<INIReader *>(user);
    string key = MakeKey(section, name);
    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value ? value : "";
    return 1;
}

void INIReader::stringSplit(const std::string &src, const std::string &delim, std::vector<std::string> &parts) const
{
    parts.clear();
    parts.shrink_to_fit();
    size_t start_pos = 0;
    size_t find_pos = src.find(delim, start_pos);
    while (find_pos != std::string::npos)
    {
        parts.emplace_back(src.substr(start_pos, find_pos - start_pos));
        start_pos = find_pos + 1;
        find_pos = src.find(delim, start_pos);
    }
    parts.emplace_back(src.substr(start_pos, src.size() - start_pos));
}