#pragma once

#include <stddef.h>
#include <vector>
#include <assert.h>
#include "TactileValue.h"

namespace tactile {

/* Abstraction for an array of similar tactile sensing elements (tactels).
	This adds accumulation modes to aggregate all sensor values within the array
	into a single value.
 */
class TactileValueArray {
public: 
	typedef std::vector<float> vector_data;
	typedef std::vector<TactileValue>::const_iterator const_iterator;
	typedef std::vector<TactileValue>::iterator iterator;

	enum AccMode {Sum=0,
					  SumPositive, SumNegative, 
					  CountPositive, CountNegative, 
					  Min, Max, 
					  lastMode};

	/// initialize array of given size, with given default range
	TactileValueArray (size_t n = 0, float min=FLT_MAX, float max=-FLT_MAX);

	static AccMode getMode (const std::string& sName);
	static std::string getModeName (AccMode m);

	size_t size() const {return vSensors.size();}
	const TactileValue& operator[](size_t i) const
		{return vSensors[i];}

	const_iterator begin() const {return vSensors.begin();}
	const_iterator end() const {return vSensors.end();}

	iterator begin() {return vSensors.begin();}
	iterator end() {return vSensors.end();}

	/// update from all values in consecutive order from it to end
	template <class T>
	void updateValues(T v, T end) {
		assert(end-v <= (std::ptrdiff_t) vSensors.size());
		for (iterator it=this->vSensors.begin(); v != end; ++it, ++v)
			it->update(*v);
	}

	/// convenience method to update from all values in vector
	void updateValues(const vector_data &values);

	/// get values with given mode into existing Iterator tgt
	template <typename Iterator>
	void getValues(TactileValue::Mode mode, Iterator tgt) const {
		for (const_iterator it=begin(), e=end(); it!=e; ++it, ++tgt)
			*tgt = it->value(mode);
	}

	/// return values with given mode into new vector v
	vector_data getValues (TactileValue::Mode mode) const;


	static float accumulate (const vector_data& data,
	                         AccMode mode=Sum, bool bMean=true);

	void setMeanLambda (float fLambda);
	void setRangeLambda (float fLambda);
	void setReleaseDecay (float fDecay);

	float getMeanLambda () const;
	float getRangeLambda () const;
	float getReleaseDecay () const;

protected:
	void init(size_t n);

private:
	std::vector<TactileValue> vSensors;
	float fMin, fMax;
};

}
