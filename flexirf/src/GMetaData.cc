/* ======================================================================== *\
!
 !   Author(s): Tarek Hassan     12/2013 <mailto:thassan@ifae.es>
 !
 !   Copyright: CTA Software Development, 2013
 !
 \* ======================================================================== */
//////////////////////////////////////////////////////////////////////////////
//
// GMetaData
//
// Class to store all keywords within metadata
//
//////////////////////////////////////////////////////////////////////////////


#include "GMetaData.h"

#include <iostream>
#include <string.h>

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct pdf table with type
//
flexIRF::GMetaData::GMetaData() {

}


void flexIRF::GMetaData::AddMetaKeyword(string keyword, string value, string comment){

	if (keyword.length() > 8){
		cout << "ERROR: Keyword too long (max length = 8)" << endl;
		return;
	}
	if (value.length() > 20){
		cout << "ERROR: Value too long (max length = 20)" << endl;
		return;
	}
	if (comment.length() > 70){
		cout << "ERROR: Comment too long (max length = 70)" << endl;
		return;
	}
	fKeywords.push_back(keyword);
	fValues.push_back(value);
	fComments.push_back(comment);

}








