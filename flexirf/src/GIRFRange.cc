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

////////////////////////////////////////////////////////////////
//
// Construct object
//
GIRFRange::GIRFRange() {

	//TODO: Is there anything needed in the constructor?

}

////////////////////////////////////////////////////////////////
//
// Construct object: read config file if available. If not,
// set default values.
//
void GIRFRange::AddAxisRange(GIRFAxis::VarType varType, float valueMin, float valueMax){

	AxisRange axisRange;

	axisRange.varType = varType;
	axisRange.lowRange = valueMin;
	axisRange.highRange = valueMax;

	fAxisRanges.push_back(axisRange);
}



