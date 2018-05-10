#ifndef SUPPORT_H
#define SUPPORT_H

// STD
#include <string>

namespace support
{
	/**
	 * Assumes little endian //TODO not sure
	 * size in bytes
	 */
	std::string GetBinaryRepresentation(size_t const size, void const * const ptr);

	/**
	 * size in bytes
	 */
	std::string GetHexRepresentation(size_t const size, void const * const ptr);

	enum class enu_VENDOR {OTHER=0, MESYTEC=1, CAEN=2, AFFEAFFE=3, CAENNOTVALID=4};

	std::string VendorAsString(enu_VENDOR p_val);

	enu_VENDOR VendorFromChar(char p_val);

}

#endif // SUPPORT_H
