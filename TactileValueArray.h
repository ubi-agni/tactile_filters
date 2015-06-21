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

	/// update from values [first, last) copying to internal buffer + offset
	template <class InputIterator>
	void updateValues(InputIterator first, InputIterator last,
	                  ptrdiff_t offset = 0) {
		// resize vSensors if not yet initialized
		if (vSensors.size() == 0 && offset >= 0)
			init(last - first + offset);

		// start from begin() (when offset >= 0) or from end() (otherwise)
		iterator start = offset >= 0 ? begin() + offset: end() + offset;
		assert(start >= begin() && start + (last-first) <= end());

		for (; first != last; ++first, ++start)
			start->update(*first);
	}
	/// convenience method to update from all values in source vector
	template <class Iteratable>
	void updateValues(const Iteratable &source, ptrdiff_t offset=0) {
		updateValues(source.begin(), source.end(), offset);
	}

	/// copy values beginning from offset into output iterator [first, last)
	template <typename OutputIterator>
	void getValues(TactileValue::Mode mode,
	               OutputIterator first, OutputIterator last,
	               ptrdiff_t offset=0) const {
		// start from begin() (when offset >= 0) or from end() (otherwise)
		const_iterator start = offset >= 0 ? begin() + offset: end() + offset;
		assert(start >= begin() && start + (last-first) <= end());
		for (; first != last; ++first, ++start)
			*first = start->value(mode);
	}
	/// convenience method to copy values with given mode into target vector
	template <class Iteratable>
	void getValues (TactileValue::Mode mode, Iteratable &target,
	                ptrdiff_t offset=0) const {
		// resize target vector if not yet initialized
		if (target.size() == 0) target.resize(vSensors.size());
		getValues(mode, target.begin(), target.end(), offset);
	}

	/// return values with given mode into new vector v
	vector_data getValues (TactileValue::Mode mode) const;

	/// accumulate values in data vector
	static float accumulate (const vector_data& data,
	                         AccMode mode=Sum, bool bMean=true);
	typedef std::function<float(const TactileValue &self)> AccessorFunction;
	/// retrieve values with given mode and accumulate them with acc_mode
	float accumulate (TactileValue::Mode mode,
	                  AccMode acc_mode=Sum, bool bMean=true);
	/// accumulate values in taxels accessed through accessor function
	float accumulate(const AccessorFunction &accessor,
	                 AccMode mode=Sum, bool bMean=true) const;

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
