#ifndef FILENAMES_H
#define FILENAMES_H

#include "tinydir.h"
#include <vector>
#include <string>

class FileNames {
	private:
		tinydir_dir dir;
		
	public:
		FileNames(const char * path);
		FileNames();
		void readDir(const char * path);
		void print();
		
		std::vector <std::string> depth;
		std::vector <std::string> rgb;
		std::vector <std::string> leap;
		std::vector <std::string> bin;
		std::vector <std::string> txt;
};

#endif