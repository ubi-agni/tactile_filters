#pragma once

#include "Range.h"
#include <string>

namespace tactile {

/* Abstraction of a single tactile sensor value.
	The class computes some internal data statistics to compute 
	values smoothed over time, current range statistics and derived values
*/
class TactileValue {
public:
	enum Mode {rawCurrent = 0, // current raw value, now computation, no averinging
		        rawMean, // averaged raw value
		        absCurrent,  // raw value - min value
		        absMean, // averaged absCurrent
		        dynCurrent,  // relative value (0-1) within min-max range
		        dynMean, // averaged dynCurrent
		        dynCurrentRelease, // like dynCurrent, but negative values indicate difference force to recently released grasp
		        dynMeanRelease, // averaged dynMeanRelease
		        lastMode,
	};
	TactileValue (float fMin=FLT_MAX, float fMax=-FLT_MAX);

	static Mode getMode (const std::string& sName);
	static std::string getModeName (Mode m);

	void init (float fMin=FLT_MAX, float fMax=-FLT_MAX);
	void update (float fNew);

	float value (Mode mode) const;

	void  setMeanLambda (float fLambda);
	void  setRangeLambda (float fLambda);
	void  setReleaseDecay (float fDecay);

	float getMeanLambda () const {return fMeanLambda;}
	float getRangeLambda () const {return fRangeLambda;}
	float getReleaseDecay () const {return fReleaseDecay;}

	const Range& absRange () const {return rAbsRange;}
	const Range& dynRange () const {return rDynRange;}

protected:
	float fMeanLambda, fRangeLambda, fReleaseDecay;
	float fCur, fMean, fReleased;
	Range rAbsRange;
	Range rDynRange;
};

}
