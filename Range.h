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
#include <values.h>

namespace tactile {

class Range {
public:
	Range (float fMin=FLT_MAX, float fMax=-FLT_MAX)
		: fMin(fMin), fMax(fMax) {}

	float  min() const {return fMin;}
	float& min() {return fMin;}

	float  max() const {return fMax;}
	float& max() {return fMax;}

	float  range() const {return fMax-fMin;}

	void   update (float fValue) {
		if (fValue < fMin) fMin=fValue;
		if (fValue > fMax) fMax=fValue;
	}

private:
	float fMin, fMax;
};

}
