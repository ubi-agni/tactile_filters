#pragma once

#include <stddef.h>
#include <vector>
#include <functional>
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

	/// initialize all taxels
	void init(size_t n, float min=FLT_MAX, float max=-FLT_MAX);
	/// re-initialize all taxels
	void reset(float min=FLT_MAX, float max=-FLT_MAX);

	static AccMode getMode (const std::string& sName);
	static std::string getModeName (AccMode m);

	size_t size() const {return vSensors.size();}
	const TactileValue& operator[](size_t i) const {return vSensors[i];}
	TactileValue& operator[](size_t i) {return vSensors[i];}

	const_iterator begin() const {return vSensors.begin();}
	const_iterator end() const {return vSensors.end();}

	iterator begin() {return vSensors.begin();}
	iterator end() {return vSensors.end();}

	/// update from all values in consecutive order from it to end
	template <class T>
	void updateValues(T v, T end, iterator start) {
		assert(end-v <= this->end()-start);
		for (iterator it=start; v != end; ++it, ++v)
			it->update(*v);
	}

	/// update from all values in consecutive order from it to end
	template <class T>
	void updateValues(T v, T end) {
		updateValues(v, end, this->vSensors.begin());
	}

	/// convenience method to update from all values in vector
	template <class Iteratable>
	void updateValues(const Iteratable &values) {
		// resize first time if not yet initialized
		if (vSensors.size() == 0) init(values.size());
		assert(vSensors.size() == values.size());
		updateValues(values.begin(), values.end(), vSensors.begin());
	}

	/// get values with given mode into existing Iterator tgt
	template <typename Iterator>
	void getValues(TactileValue::Mode mode, Iterator tgt) const {
		for (const_iterator it=begin(), e=end(); it!=e; ++it, ++tgt)
			*tgt = it->value(mode);
	}

	/// return values with given mode into new vector v
	vector_data getValues (TactileValue::Mode mode) const;

	typedef std::function<float(const TactileValue &self)> AccessorFunction;
	float accumulate(const AccessorFunction &accessor,
	                 AccMode mode=Sum, bool bMean=true) const;
	static float accumulate (const vector_data& data,
	                         AccMode mode=Sum, bool bMean=true);

	void setMeanLambda (float fLambda);
	void setRangeLambda (float fLambda);
	void setReleaseDecay (float fDecay);

	float getMeanLambda () const;
	float getRangeLambda () const;
	float getReleaseDecay () const;

private:
	std::vector<TactileValue> vSensors;
};

}
