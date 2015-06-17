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

static float posAdd (float result, float c) {
	if (c > 0) return result+c;
	return result;
}
static float negAdd (float result, float c) {
	if (c < 0) return result+c;
	return result;
}
static float posCount (float result, float c) {
	if (c > 0) return result+1;
	return result;
}
static float negCount (float result, float c) {
	if (c < 0) return result+1;
	return result;
}
static float minFunc (float a, float b) {return std::min(a,b);}
static float maxFunc (float a, float b) {return std::max(a,b);}
float TactileValueArray::accumulate (const TactileValueArray::vector_data& data, 
                                     AccMode mode, bool bMean) {
	float fRet=0;
	switch (mode) {
		case Sum: 
			fRet = std::accumulate(data.begin(),data.end(), 0.);
			break;

		case SumPositive:
			fRet = std::accumulate(data.begin(),data.end(), 0., posAdd);
			break;
		case SumNegative:
			fRet = std::accumulate(data.begin(),data.end(), 0., negAdd);
			break;

		case CountPositive:
			fRet = std::accumulate(data.begin(),data.end(), 0., posCount);
			break;
		case CountNegative:
			fRet = std::accumulate(data.begin(),data.end(), 0., negCount);
			break;

		case Min:
			fRet = std::accumulate(data.begin(),data.end(), FLT_MAX, minFunc);
			break;
		case Max:
			fRet = std::accumulate(data.begin(),data.end(), -FLT_MAX, maxFunc);
			break;
	}
	if (bMean) fRet /= data.size();
	return fRet;
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
