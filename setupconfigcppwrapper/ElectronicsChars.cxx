#include "ElectronicsChars.h"

// STD //TODO delete?
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "setupconfig/pureC/functions_electr.h"

ElectronicsChars::ElectronicsChars() :
	TObject()
{
	cerr << "DEFAULT ElectronicsChars CONSTRUCTOR" << endl;
}

ElectronicsChars::ElectronicsChars(const char* p_filename) :
	TObject()
{
	// Import XML electronics characteristics file
	InitStcElectrChars(&fCharacteristics);
	ImportXML_electr(&fCharacteristics, p_filename);
	DumpStcElectrChars(&fCharacteristics);

	if (!(this->CheckConsistency())) {
		cerr << "[FATAL ] Imported XML electronics characteristics is inconsistent. Please check it. Aborting." << endl;
		exit(EXIT_FAILURE); //TODO
	}

	this->Link();
}

ElectronicsChars::~ElectronicsChars()
{
}

void ElectronicsChars::Print(Option_t* option) const
{
	DumpStcElectrChars(&fCharacteristics);
}

bool ElectronicsChars::CheckConsistency(void)
{
	//TODO any additional checks
	return true;
}

signed short ElectronicsChars::GetRangeLow(TString p_blockName) const
{
	////cerr << "Searching " << p_blockName << endl;
	//TODO catch exception
	return fBlocks.at(p_blockName)->fRangeLow;
}

signed short ElectronicsChars::GetRangeHigh(TString p_blockName) const
{
	//TODO catch exception
	return fBlocks.at(p_blockName)->fRangeHigh;
}

unsigned short ElectronicsChars::GetNbins(TString p_blockName) const
{
	//TODO catch exception
	return fBlocks.at(p_blockName)->fNbins;
}

void ElectronicsChars::Link(void)
{
	// For each elblock
	for (unsigned short iElblock=0; iElblock<fCharacteristics.fNblocks; iElblock++) {
		stc_elblock* v_curBlock = &(fCharacteristics.fBlocksList[iElblock]);
		TString v_blockName(v_curBlock->fBlockName);
		fBlocks.insert(std::pair<TString, stc_elblock*>(v_blockName, v_curBlock));
		////cerr << "Adding " << v_blockName << endl;
	}
}

ClassImp(ElectronicsChars)
