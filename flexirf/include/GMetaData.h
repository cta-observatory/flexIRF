#ifndef G_Metadata
#define G_Metadata

#include <iostream>
#include <vector>
#include <string>
#include "fitsio.h"

using namespace std;

namespace flexIRF{

	class GMetaData
	{
	 private:

		vector<string> fKeywords;
		vector<string> fValues;
		vector<string> fComments;

	 public:
		GMetaData();
		virtual ~GMetaData(){};
		void AddMetaKeyword(string keyword, string value, string comment);
		inline int GetSize(){return (int) fKeywords.size();}
		inline string GetKeyword(int iKey){return fKeywords[iKey];}
		inline string GetValue(int iKey){return fValues[iKey];}
		inline string GetComment(int iKey){return fComments[iKey];}
	};
}
#endif

