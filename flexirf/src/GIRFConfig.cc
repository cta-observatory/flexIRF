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
#include "GIRFAxis.h"
#include "GIRFRange.h"

////////////////////////////////////////////////////////////////
// 
// Construct object: read config file if available. If not,
// set default values.
//
flexIRF::GIRFConfig::GIRFConfig() :
		fNSBLevel(kNoNSBLevel), fObsMode(kNoObsMode), fSubArray(kNoSubArray) {

	SetDefault();

	//Check if there is a config file in the path:
	ifstream configfile("IRFConfig.txt");
	if (!configfile.is_open()) {
		cout << "WARNING: No IRF configuration file found\n";
	}

}

flexIRF::GIRFConfig::GIRFConfig(string filename) :
		fNSBLevel(kNoNSBLevel), fObsMode(kNoObsMode), fSubArray(kNoSubArray) {

	SetDefault();

	ifstream configfile(filename.c_str());

	if (!configfile.is_open()) {
		cout << "ERROR: Configuration file not accesible.\n";
		return;
	}

	LoadConfigFile(filename);
}

int flexIRF::GIRFConfig::LoadConfigFile(string filename) {

	// TODO
	// Read input card and set values

	return 0;
}

void flexIRF::GIRFConfig::SetDefault() {

	fNSBLevel = kExtragalactic;
	fObsMode = kPoint;
	fSubArray = kFull;
	fWeather.Transmitance = 1;
	fWeather.molProfile = kAverageProf;
	fHWStatus.reflectivity = 1;
	fHWStatus.trigger = kStandard;

}

void flexIRF::GIRFConfig::Print() {

	cout << "*******************************************" << endl;
	cout << "***       Printing CTAconfig          ***" << endl;
	cout << "*******************************************" << endl;

	cout << "fNSBLevel = " << fNSBLevel << endl;
	cout << "fObsMode = " << fObsMode << endl;
	cout << "fSubArray = " << fSubArray << endl;
	cout << "Transmitance = " << fWeather.Transmitance << endl;
	cout << "molProfile = " << fWeather.molProfile << endl;
	cout << "reflectivity = " << fHWStatus.reflectivity << endl;
	cout << "trigger = " << fHWStatus.trigger << endl;

	std::vector<AxisRange> axisRanges = GetAxisRanges();
	for(std::vector<AxisRange>::iterator axisRange = axisRanges.begin(); axisRange != axisRanges.end(); ++axisRange) {
		cout << "axisRange->varType = " << axisRange->varType << ", lowRange = " << axisRange->lowRange << ", highRange = " << axisRange->highRange << endl;;
	}

	cout << "" << endl;
	cout << "*******************************************" << endl;

}






