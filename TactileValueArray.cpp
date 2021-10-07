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
#include "TactileValueArray.h"
#include <numeric>

namespace tactile {

TactileValueArray::TactileValueArray(size_t n, float min, float max)
{
	init(n, min, max);
}

void TactileValueArray::init(size_t n, float min, float max)
{
	vSensors.resize(n);
	reset(min, max);
}

void TactileValueArray::reset(float min, float max)
{
	for (iterator it = vSensors.begin(), e = vSensors.end(); it != e; ++it)
		it->init(min, max);
}


TactileValueArray::AccMode TactileValueArray::getMode(const std::string &sName)
{
	if (sName == "Sum") return Sum;
	if (sName == "SumPositive") return SumPositive;
	if (sName == "SumNegative") return SumNegative;
	if (sName == "CountPositive") return CountPositive;
	if (sName == "CountNegative") return CountNegative;
	if (sName == "Min") return Min;
	if (sName == "Max") return Max;
	return SumPositive;  // default fallback
}

std::string TactileValueArray::getModeName(AccMode m)
{
	switch (m) {
		case Sum: return "Sum";
		case SumPositive: return "SumPositive";
		case SumNegative: return "SumNegative";
		case CountPositive: return "CountPositive";
		case CountNegative: return "CountNegative";
		case Min: return "Min";
		case Max: return "Max";
		default: return "";
	}
}

TactileValueArray::vector_data TactileValueArray::getValues(TactileValue::Mode mode) const
{
	vector_data vReturn(vSensors.size());
	getValues(mode, vReturn);
	return vReturn;
}

typedef float (*AccumulatorFunction)(const float, const float);
static float Add(float result, float c)
{
	return result + c;
}
static float posAdd(float result, float c)
{
	if (c > 0)
		return result + c;
	else
		return result;
}
static float negAdd(float result, float c)
{
	if (c < 0)
		return result + c;
	else
		return result;
}
static float posCnt(float result, float c)
{
	if (c > 0)
		return result + 1;
	else
		return result;
}
static float negCnt(float result, float c)
{
	if (c < 0)
		return result + 1;
	else
		return result;
}
static float minFun(float result, float c)
{
	return std::min(result, c);
}
static float maxFun(float result, float c)
{
	return std::max(result, c);
}

static float initial[] = { 0, 0, 0, 0, 0, FLT_MAX, -FLT_MAX };
static AccumulatorFunction accumulators[] = { Add, posAdd, negAdd, posCnt, negCnt, minFun, maxFun };

float TactileValueArray::accumulate(const vector_data &data, AccMode mode, bool bMean)
{
	float result = std::accumulate(data.begin(), data.end(), initial[mode], accumulators[mode]);
	if (bMean && data.size()) result /= data.size();
	return result;
}

float TactileValueArray::accumulate(TactileValue::Mode mode, AccMode acc_mode, bool bMean)
{
	return accumulate([mode](const TactileValue &self) { return self.value(mode); }, acc_mode,
	                  bMean);
}

float TactileValueArray::accumulate(const AccessorFunction &accessor, AccMode mode,
                                    bool bMean) const
{
	float result = initial[mode];
	AccumulatorFunction acc = accumulators[mode];
	for (const_iterator it = vSensors.begin(), e = vSensors.end(); it != e; ++it)
		result = acc(result, accessor(*it));
	if (bMean && vSensors.size()) result /= vSensors.size();
	return result;
}

void TactileValueArray::setMeanLambda(float fLambda)
{
	for (iterator it = vSensors.begin(), e = vSensors.end(); it != e; ++it)
		it->setMeanLambda(fLambda);
}
void TactileValueArray::setRangeLambda(float fLambda)
{
	for (iterator it = vSensors.begin(), e = vSensors.end(); it != e; ++it)
		it->setRangeLambda(fLambda);
}
void TactileValueArray::setReleaseDecay(float fDecay)
{
	for (iterator it = vSensors.begin(), e = vSensors.end(); it != e; ++it)
		it->setReleaseDecay(fDecay);
}

float TactileValueArray::getMeanLambda() const
{
	return accumulate([](const TactileValue &self) { return self.getMeanLambda(); }, Sum, true);
}

float TactileValueArray::getRangeLambda() const
{
	return accumulate([](const TactileValue &self) { return self.getRangeLambda(); }, Sum, true);
}

float TactileValueArray::getReleaseDecay() const
{
	return accumulate([](const TactileValue &self) { return self.getReleaseDecay(); }, Sum, true);
}

}  // namespace tactile
