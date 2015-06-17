#include "TactileValueArray.h"
#include <numeric>

namespace tactile {

TactileValueArray::TactileValueArray(size_t n, float min, float max)
	: fMin(min), fMax(max)
{
	init(n);
}

void TactileValueArray::init(size_t n) {
	vSensors.resize(n);
	for (iterator it=vSensors.begin(), e=vSensors.end(); it!=e; ++it)
		it->init(fMin, fMax);
}


TactileValueArray::AccMode TactileValueArray::getMode (const std::string& sName) {
	if (sName == "Sum") return Sum;
	if (sName == "SumPositive") return SumPositive;
	if (sName == "SumNegative") return SumNegative;
	if (sName == "CountPositive") return CountPositive;
	if (sName == "CountNegative") return CountNegative;
	if (sName == "Min") return Min;
	if (sName == "Max") return Max;
	return SumPositive; // default fallback
}

std::string TactileValueArray::getModeName (AccMode m) {
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

void TactileValueArray::updateValues(const TactileValueArray::vector_data &values) {
	// resize first time if not yet initialized
	if (vSensors.size() == 0) init(values.size());
	assert(vSensors.size() == values.size());
	updateValues(values.begin(), values.end());
}

TactileValueArray::vector_data 
TactileValueArray::getValues (TactileValue::Mode mode) const {
	vector_data vReturn(vSensors.size());
	getValues(mode, vReturn.begin());
	return vReturn;
}

typedef float(*AccumulatorFunction)(const float, const float);
static float Add    (float result, float c) {return result+c;}
static float posAdd (float result, float c) {if (c > 0) return result+c; else return result;}
static float negAdd (float result, float c) {if (c < 0) return result+c; else return result;}
static float posCnt (float result, float c) {if (c > 0) return result+1; else return result;}
static float negCnt (float result, float c) {if (c < 0) return result+1; else return result;}
static float minFun (float result, float c) {return std::min(result, c);}
static float maxFun (float result, float c) {return std::max(result, c);}

static float initial[] = {0,  0, 0,  0, 0,  FLT_MAX, -FLT_MAX};
static AccumulatorFunction accumulators[] = {Add,  posAdd, negAdd,  posCnt, negCnt,  minFun, maxFun};

float TactileValueArray::accumulate (float (*accessor)(const TactileValue& self),
                                     AccMode mode, bool bMean) const {
	float result = initial[mode];
	AccumulatorFunction acc = accumulators[mode];
	for (const_iterator it=vSensors.begin(), e=vSensors.end(); it!=e; ++it)
		result = acc(result, accessor(*it));
	if (bMean && vSensors.size()) result /= vSensors.size();
	return result;
}

float TactileValueArray::accumulate (const TactileValueArray::vector_data& data,
                                     AccMode mode, bool bMean) {
	float result = std::accumulate(data.begin(), data.end(), initial[mode], accumulators[mode]);
	if (bMean && data.size()) result /= data.size();
	return result;
}

void TactileValueArray::setMeanLambda (float fLambda) {
	for (iterator it=vSensors.begin(), e=vSensors.end(); it!=e; ++it)
		it->setMeanLambda(fLambda);
}
void TactileValueArray::setRangeLambda (float fLambda) {
	for (iterator it=vSensors.begin(), e=vSensors.end(); it!=e; ++it)
		it->setRangeLambda(fLambda);
}
void TactileValueArray::setReleaseDecay (float fDecay) {
	for (iterator it=vSensors.begin(), e=vSensors.end(); it!=e; ++it)
		it->setReleaseDecay(fDecay);
}

float TactileValueArray::getMeanLambda() const {
	return vSensors[0].getMeanLambda();
}

float TactileValueArray::getRangeLambda() const {
	return vSensors[0].getRangeLambda();
}

float TactileValueArray::getReleaseDecay() const {
	return vSensors[0].getReleaseDecay();
}

}
