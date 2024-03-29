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

#include "Range.h"

namespace tactile {

std::ostream &operator<<(std::ostream &os, const Range &r)
{
	return os << "[" << r.min() << "," << r.max() << "]";
}

void Range::init(float fMin, float fMax)
{
	this->fMin = fMin;
	this->fMax = fMax;
}

void Range::update(float fValue)
{
	if (fValue < fMin) fMin = fValue;
	if (fValue > fMax) fMax = fValue;
}

void Range::update(const Range &other)
{
	if (other.fMin < fMin) fMin = other.fMin;
	if (other.fMax > fMax) fMax = other.fMax;
}

bool tactile::Range::operator==(const tactile::Range &other) const
{
	return (this->fMin == other.fMin) && (this->fMax == other.fMax);
}

}  // namespace tactile
