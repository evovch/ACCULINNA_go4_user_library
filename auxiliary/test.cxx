#include "Support.h"

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
	for (unsigned int a=0; a<65536; a++) {
		std::cerr << a << "\t";
		std::cerr << support::GetBinaryRepresentation(sizeof(unsigned int), &a) << "\t";
		std::cerr << support::GetHexRepresentation(sizeof(unsigned int), &a) << std::endl;
	}
}
