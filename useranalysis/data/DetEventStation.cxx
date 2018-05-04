#include "DetEventStation.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "DetMessage.h"

DetEventStation::DetEventStation(const char* name, const char* folder, Short_t id) :
	TGo4EventElement(name, folder, id) // Unique ID as third argument
{
	//cerr << "DetEventStation::DetEventStation() -> ";
	this->Clear();
}

DetEventStation::~DetEventStation()
{
}

void DetEventStation::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "DetEventStation::Clear()" << endl;

}

void DetEventStation::Dump(void) const
{
	//TODO dump all data members!

}

ClassImp(DetEventStation)
