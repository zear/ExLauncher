/*
Copyright 2016 Andreas Bjerkeholt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "utils.h"
#include <chrono>
#include "global.h"
#include <stdio.h>
#include <cctype>
#ifdef WINDOWS
#include <windows.h>
#include <tchar.h>
#endif
#ifdef UNIX
#include <dirent.h>
#endif

using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

// FIXME find some good solution here..
std::string trimRight(std::string str, const std::string& trimChars)
{
   std::string::size_type pos = str.find_last_not_of(trimChars);
   str.erase(pos + 1);
   return str;
}

std::string trimLeft(std::string str, const std::string& trimChars)
{
   std::string::size_type pos = str.find_first_not_of(trimChars);
   str.erase(0, pos);
   return str;
}

std::string trim(std::string str, const std::string& trimChars)
{
	return trimRight(trimLeft(str, trimChars), trimChars);
}

string lineBreak(string text, int lineLength)
{
	string out = "";

    int curRowStartPos = 0;
    int curRowLength = 0;
    int lastSpaceForCurRow = -1;
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
			out += trim(text.substr(curRowStartPos, i - curRowStartPos));
			out += '\n';
            lastSpaceForCurRow = -1;
            curRowLength = 0;
            curRowStartPos = i + 1;
            continue;
        }

        if (text[i] == ' ')
        {
            lastSpaceForCurRow = i;
        }

        if (curRowLength >= lineLength)
        {
            // If we have an appropriate space to break on, do so, otherwise force break anyway
            if (lastSpaceForCurRow > -1)
            {
                i = lastSpaceForCurRow;
            }

			out += trim(text.substr(curRowStartPos, i - curRowStartPos));
			out += '\n';
            lastSpaceForCurRow = -1;
            curRowLength = 0;
            curRowStartPos = i;
            continue;
        }

        curRowLength++;
    }

    if (text.length() > curRowStartPos)
    {
		out += trim(text.substr(curRowStartPos, text.length() - curRowStartPos));
    }

    return out;
}

int clip(int val, int min, int max)
{
	if (val < min)
		return min;

	if (val > max)
		return max;

	return val;
}

double clip(double val, double min, double max)
{
	if (val < min)
		return min;

	if (val > max)
		return max;

	return val;
}

chrono::time_point<chrono::steady_clock> start;

void measureTimeStart()
{
	start = chrono::steady_clock::now();
}

double measureTimeFinish()
{
	auto finish = chrono::steady_clock::now();
	double elapsed_seconds = chrono::duration_cast<chrono::duration<double> >(finish - start).count();

	return elapsed_seconds;
}

vector<string> getDirectories(string path)
{
	vector<string> directories;

#ifdef WINDOWS
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	string pathMatch = path + "*";

	hFind = FindFirstFile(pathMatch.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (string(".") != ffd.cFileName && string("..") != ffd.cFileName)
					directories.push_back(ffd.cFileName);
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);
	}

#else
	DIR* dirp = opendir(path.c_str());
	if (dirp != nullptr)
	{
		struct dirent* entry;
		while ((entry = readdir(dirp)) != NULL)
		{
			if (entry->d_type == DT_DIR)
			{
				if (string(".") != entry->d_name && string("..") != entry->d_name)
					directories.push_back(entry->d_name);
			}
		}

		closedir(dirp);
	}
#endif

	return directories;
}

vector<string> getFilesByExtension(string path, string extension)
{
	vector<string> files;

#ifdef WINDOWS
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	string pathMatch = path;
	pathMatch += "*";
	if (!extension.empty())
	{
		pathMatch += ".";
		pathMatch += extension;
	}

	hFind = FindFirstFile(pathMatch.c_str(), &ffd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL ||
				ffd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE ||
				ffd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED ||
				ffd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			{
				if (string(".") != ffd.cFileName && string("..") != ffd.cFileName)
					files.push_back(ffd.cFileName);
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);
	}

#else
	DIR* dirp = opendir(path.c_str());
	if (dirp != nullptr)
	{
		struct dirent* entry;
		while ((entry = readdir(dirp)) != NULL)
		{
			if (entry->d_type == DT_REG && string(".") != entry->d_name && string("..") != entry->d_name)
			{
				if (extension.empty() || stringEndsWith(entry->d_name, string(".") + extension))
					files.push_back(entry->d_name);
			}
		}

		closedir(dirp);
	}
#endif

	return files;
}

string getCapitalizedString(string str)
{
	str[0] = std::toupper(str[0]);
	return str;
}

bool stringEndsWith(std::string const& str, std::string const& end)
{
	if (str.length() >= end.length())
	{
		return (0 == str.compare(str.length() - end.length(), end.length(), end));
	}
	else
	{
		return false;
	}
}
