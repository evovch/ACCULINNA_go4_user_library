#ifndef STRUCTS_ELECTR_H
#define STRUCTS_ELECTR_H

#ifdef __cplusplus
#include <TObject.h> /* mother class */
#endif /* __cplusplus */

/* ============================================================================================= */
/* structure stc_elblock ================================================================== */
/* ============================================================================================= */

#ifdef __cplusplus
class stc_elblock : public TObject
{
public:
	stc_elblock() : TObject() {}
	virtual ~stc_elblock() {}

#else
typedef struct _stc_elblock {
#endif /* __cplusplus */

	char fBlockName[64];
	unsigned short fNchannels;
	signed short fRangeLow;
	signed short fRangeHigh;
	unsigned short fNbins;
	char fType[64];
	char fVendor[64];

#ifdef __cplusplus
	ClassDef(stc_elblock, 1);
};
ClassImp(stc_elblock)
#else
} stc_elblock;
#endif /* __cplusplus */

/* ============================================================================================= */
/* structure stc_electr_chars ================================================================== */
/* ============================================================================================= */

#ifdef __cplusplus
class stc_electr_chars : public TObject
{
public:
	stc_electr_chars() : TObject(), fBlocksList(nullptr) {}
	virtual ~stc_electr_chars() {}

#else
typedef struct _stc_electr_chars {
#endif /* __cplusplus */

	signed int fNblocks; /* Number of <elblock> entries */
	#ifdef __cplusplus
	stc_elblock* fBlocksList; //[fNblocks]
	#else
	stc_elblock* fBlocksList;
	#endif /* __cplusplus */

#ifdef __cplusplus
	ClassDef(stc_electr_chars, 1);
};
ClassImp(stc_electr_chars)
#else
} stc_electr_chars;
#endif /* __cplusplus */

/* ============================================================================================= */
/* ============================================================================================= */
/* ============================================================================================= */

#endif /* STRUCTS_ELECTR_H */
