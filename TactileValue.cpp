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
#include "TactileValue.h"
#include <math.h>

namespace tactile {

TactileValue::TactileValue (float fMin, float fMax)
	: fMeanLambda (0.7), fRangeLambda (0.9995), fReleaseDecay (0.05)
{
	init(fMin, fMax);
}

TactileValue::Mode TactileValue::getMode (const std::string& sName) {
	if (sName == "rawCurrent") return rawCurrent;
	if (sName == "rawMean") return rawMean;
	if (sName == "absCurrent") return absCurrent;
	if (sName == "absMean") return absMean;
	if (sName == "dynCurrent") return dynCurrent;
	if (sName == "dynMean") return dynMean;
	if (sName == "dynCurrentRelease") return dynCurrentRelease;
	if (sName == "dynMeanRelease") return dynMeanRelease;
	return absCurrent; // the default fallback
}

std::string TactileValue::getModeName (Mode m) {
	switch (m) {
		case rawCurrent: return "rawCurrent";
		case rawMean: return "rawMean";
		case absCurrent: return "absCurrent";
		case absMean: return "absMean";
		case dynCurrent: return "dynCurrent";
		case dynMean: return "dynMean";
		case dynCurrentRelease: return "dynCurrentRelease";
		case dynMeanRelease: return "dynMeanRelease";
		default: return "";
	}
}

void TactileValue::init(float fMin, float fMax)
{
	rAbsRange.init(fMin, fMax);
	rDynRange.init();
	this->fCur = this->fMean = NAN;
	this->fReleased = FLT_MAX;
}

void TactileValue::update (float fNew) {
	if (!isfinite(fNew)) return; // do not use invalid value

	rAbsRange.update (fNew); // set all-time minimum + maximum
	rDynRange.update (fNew); // adapt sliding minimum + maximum
	rDynRange.min() = fNew - fRangeLambda * (fNew - rDynRange.min());
	rDynRange.max() = fNew + fRangeLambda * (rDynRange.max() - fNew);

	if (isnan(fCur)) { // first update: init vars and return
		this->fCur = this->fMean = fNew;
		return;
	}
	// update mean
	fMean = fNew + fMeanLambda * (fMean - fNew);

	// compute release mode
	const float fMargin = 0.1 * rAbsRange.range();
	if (fReleased != FLT_MAX && fNew > fCur + fMargin) {
		// if we recently released (fReleased != FLT_MAX) 
		// and fNew increased considerably, we leave release mode
		fReleased = FLT_MAX;
	} else if (fReleased == FLT_MAX && fNew < fCur - fMargin) {
		// if new value drops from old value considerably
		// we enter release mode, remembering the old value in fReleased
		fReleased = fCur;
	} else if (fReleased != FLT_MAX) {
		// we are in release mode: linearly decrease fReleased
		fReleased -= fReleaseDecay * rDynRange.range();
		// leave release mode, if we decayed enough
		if (fReleased < rDynRange.min()) {
			fReleased = FLT_MAX;
		}
	}

	// store current value
	fCur = fNew;
}

float TactileValue::value (Mode mode) const {
	if (mode == rawCurrent) return fCur;
	if (mode == rawMean) return fMean;

	const Range &r = (mode == absCurrent || mode == absMean) ? rAbsRange : rDynRange;
	float fRange = r.range();
	if (fRange < FLT_EPSILON)
		return NAN; // do not divide by zero

	if (mode >= dynCurrentRelease && fReleased != FLT_MAX)
		return - (fReleased - r.min()) / fRange;

	float v = fCur;
	if (mode == absMean || mode == dynMean || mode == dynMeanRelease)
		v = fMean;

	return (v - r.min()) / fRange;
}

void  TactileValue::setMeanLambda (float fLambda) {
	this->fMeanLambda=fLambda;
}
void  TactileValue::setRangeLambda (float fLambda) {
	this->fRangeLambda=fLambda;
}
void  TactileValue::setReleaseDecay (float fDecay) {
	this->fReleaseDecay=fDecay;
}

}
