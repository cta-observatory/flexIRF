/* ======================================================================== *\
!
 !   Author(s): Tarek Hassan     09/2015 <mailto:thassan@ifae.es>
 !
 !   Copyright: CTA Software Development, 2015
 !
 \* ======================================================================== */

#include <fstream>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////
//
// GIRFConfig
//
// Dummy class to select an IRF configuration
//
//////////////////////////////////////////////////////////////////////////////

using namespace std;

#include "GIRFConfig.h"

////////////////////////////////////////////////////////////////
// 
// Construct object: read config file if available. If not,
// set default values.
//
GIRFConfig::GIRFConfig() :
		fNSBLevel(kNoNSBLevel), fObsMode(kNoObsMode), fSubArray(kNoSubArray) {
	fWeather.Transmitance = 0;
	fWeather.molProfile = kNoMolProfile;
	fHWStatus.reflectivity = 0;
	fHWStatus.trigger = kNoTrigger;

	//Check if there is a config file in the path:
	ifstream configfile("IRFConfig.txt");
	if (!configfile.is_open()) {
		cout << "WARNING: No IRF configuration file found\n";
	}

}

GIRFConfig::GIRFConfig(string filename) :
		fNSBLevel(kNoNSBLevel), fObsMode(kNoObsMode), fSubArray(kNoSubArray) {
	fWeather.Transmitance = 0;
	fWeather.molProfile = kNoMolProfile;
	fHWStatus.reflectivity = 0;
	fHWStatus.trigger = kNoTrigger;

	ifstream configfile(filename.c_str());

	if (!configfile.is_open()) {
		cout << "ERROR: Configuration file not accesible.\n";
		return;
	}

	LoadConfigFile(filename);
}

int GIRFConfig::LoadConfigFile(string filename) {

	// TODO
	// Read input card and set values


	return 0;
}

void GIRFConfig::SetDefault() {
	fNSBLevel = kExtragalactic;
	fObsMode = kPoint;
	fSubArray = kFull;
	fWeather.Transmitance = 1;
	fWeather.molProfile = kAverage;
	fHWStatus.reflectivity = 1;
	fHWStatus.trigger = kStandard;
}

