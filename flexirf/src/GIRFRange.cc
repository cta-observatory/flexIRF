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
// GIRFRange
//
// Dummy class (for now) to select the Axis range to extract from GIRFs
//
//////////////////////////////////////////////////////////////////////////////

using namespace std;

#include "GIRFRange.h"
#include "GIRFAxis.h"

////////////////////////////////////////////////////////////////
//
// Construct object
//
flexIRF::GIRFRange::GIRFRange() {

	//TODO: Is there anything needed in the constructor?

}

////////////////////////////////////////////////////////////////
//
// Construct object: read config file if available. If not,
// set default values.
//
void flexIRF::GIRFRange::AddAxisRange(VarType varType, float valueMin, float valueMax){

	AxisRange axisRange;

	axisRange.varType = varType;
	axisRange.lowRange = valueMin;
	axisRange.highRange = valueMax;
	axisRange.required = 0;

	fAxisRanges.push_back(axisRange);
}

void flexIRF::GIRFRange::AddAxisRange(VarType varType, float valueMin, float valueMax, bool required){

	AxisRange axisRange;

	axisRange.varType = varType;
	axisRange.lowRange = valueMin;
	axisRange.highRange = valueMax;
	axisRange.required = required;

	fAxisRanges.push_back(axisRange);
}



