#include "UserEventDigiBuilding.h"

UserEventDigiBuilding::UserEventDigiBuilding(const char* name) :
	TGo4EventElement(name)
{
	this->Clear();
}

UserEventDigiBuilding::~UserEventDigiBuilding()
{
}

void UserEventDigiBuilding::Clear(Option_t* t)
{
}

void UserEventDigiBuilding::Dump(void) const
{

}

ClassImp(UserEventDigiBuilding)
