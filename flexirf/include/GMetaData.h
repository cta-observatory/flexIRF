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

		vector<string> fStKeywords;
		vector<string> fStValues;
		vector<string> fStComments;
		vector<string> fFlKeywords;
		vector<float> fFlValues;
		vector<string> fFlComments;

	 public:
		GMetaData();
		virtual ~GMetaData(){};
		void AddMetaKeyword(string keyword, string value, string comment);
		void AddMetaKeyword(string keyword, float value, string comment);
		inline int GetStSize(){return (int) fStKeywords.size();}
		inline int GetFlSize(){return (int) fFlKeywords.size();}
		inline string GetStKeyword(int iKey){return fStKeywords[iKey];}
		inline string GetStValue(int iKey){return fStValues[iKey];}
		inline string GetStComment(int iKey){return fStComments[iKey];}
		inline string GetFlKeyword(int iKey){return fFlKeywords[iKey];}
		inline float  GetFlValue(int iKey){return fFlValues[iKey];}
		inline string GetFlComment(int iKey){return fFlComments[iKey];}
	};
}
#endif

