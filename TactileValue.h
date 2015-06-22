/* ============================================================
 *
 * Copyright (C) 2015 by Robert Haschke <rhaschke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of the
 * GNU Lesser General Public License Version 3 (the "LGPL"),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CITEC, "Cognitive Interaction Technology" Excellence Cluster
 *     Bielefeld University
 *
 * ============================================================ */
#pragma once

#include "Range.h"
#include "Calibration.h"
#include <string>
#include <memory>

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

	void setCalibration(const std::shared_ptr<Calibration> &c) {calib = c;}
	std::shared_ptr<Calibration> getCalibration() const {return calib;}

protected:
	float fMeanLambda, fRangeLambda, fReleaseDecay;
	float fCur, fMean, fReleased;
	Range rAbsRange;
	Range rDynRange;
	std::shared_ptr<Calibration> calib;
};

}
