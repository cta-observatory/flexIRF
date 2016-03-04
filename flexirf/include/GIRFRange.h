#ifndef G_IRF_Range
#define G_IRF_Range

#include <string>
#include <vector>
#include <GIRFAxis.h>

namespace flexIRF{
	class GIRFRange {

	private:
		std::vector<AxisRange> fAxisRanges;
	//	int LoadConfigFile(std::string filename);

	public:
		inline void AddAxisRange(AxisRange axisRange)  {fAxisRanges.push_back(axisRange);} 		// insert axis range in the list
		void AddAxisRange(VarType varType, float valueMin, float valueMax); 		// insert axis range in the list
		void AddAxisRange(VarType varType, float valueMin, float valueMax, bool required);

		GIRFRange();
		virtual ~GIRFRange() {};
		inline std::vector<AxisRange> GetAxisRanges() const {return fAxisRanges;}
		inline uint GetNumAxisRanges() const {return fAxisRanges.size();}

	};
}
#endif

