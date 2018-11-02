#include "Support.h"

// STD
#include <cstdio> // for sprintf
#include <sys/stat.h> // for stat, mkdir
#include <iostream>

std::string support::GetBinaryRepresentation(size_t const size, void const * const ptr)
{
	std::string outString;

	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;

	char cStr[2];
	cStr[1] = '\0';

	for (int i=size-1; i>=0; i--) {
		for (int j=7; j>=0; j--) {
			byte = (b[i] >> j) & 1;
			sprintf(cStr, "%u", byte);
			outString.append(cStr);
		}
	}

	return outString;
}

std::string support::GetHexRepresentation(size_t const size, void const * const ptr)
{
	std::string outString;

	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;

	char cStr[3];
	cStr[2] = '\0';

	for (int i=size-1; i>=0; i--) {
		byte = b[i] & 0xff;
		sprintf(cStr, "%02x", byte);
		outString.append(cStr);
	}

	return outString;
}

std::string support::VendorAsString(enu_VENDOR p_val)
{
	std::string outString;

	switch (p_val) {
	case enu_VENDOR::OTHER:        outString = "OTHER"; break;
	case enu_VENDOR::MESYTEC:      outString = "MESYTEC"; break;
	case enu_VENDOR::CAEN:         outString = "CAEN"; break;
	case enu_VENDOR::AFFEAFFE:     outString = "AFFEAFFE"; break;
	case enu_VENDOR::CAENNOTVALID: outString = "CAENNOTVALID"; break;
	default: outString = "WTF"; break;
	};

	return outString;
}

support::enu_VENDOR support::VendorFromChar(char p_val)
{
	return (enu_VENDOR)p_val;
}

void support::CheckThatDirExists(const char* dir_name)
{
	struct stat info;
	if( stat( dir_name, &info ) != 0 ) {
		mkdir(dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		std::cerr << "[INFO  ] " << "Created directory '" << dir_name << "'." << std::endl;
	} else if( info.st_mode & S_IFDIR ) {
		std::cerr << "[INFO  ] " << "Directory '" << dir_name << "' already exists. All previous text files there will be overwritten." << std::endl;
	} else {
		std::cerr << "[FATAL ] " <<  "'" << dir_name << "' already exists in the current directory and it is not a directory. "
		     << "We stop here such that the existing file named '" << dir_name << "' is not overwritten. "
		     << "Please, back it up if you need and remove it from the current directory." << std::endl;
		exit(EXIT_FAILURE);
	}
}
