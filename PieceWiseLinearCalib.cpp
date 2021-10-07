/* ============================================================
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
#include "PieceWiseLinearCalib.h"
#include <assert.h>
#include <stdexcept>
#ifdef HAVE_YAML
#include <yaml-cpp/yaml.h>
#endif

namespace tactile {

PieceWiseLinearCalib::PieceWiseLinearCalib(const CalibrationMap &values)
{
	init(values);
}

void PieceWiseLinearCalib::init(const CalibrationMap &values)
{
	assert(values.size() > 1);
	this->values = values;
	range = Range();
	for (const auto &value : values)
		range.update(value.second);
}

float PieceWiseLinearCalib::map(float x) const
{
	assert(values.size() > 1);
	// retrieve first key that is larger than x
	CalibrationMap::const_iterator next = values.upper_bound(x);
	// if x is smaller than key range, clip to smallest value
	if (next == values.begin()) return values.begin()->second;
	// if x is larger than key range, clip to largest value
	if (next == values.end()) return values.rbegin()->second;
	CalibrationMap::const_iterator prev = next;
	--prev;
	return prev->second +
	       (x - prev->first) / (next->first - prev->first) * (next->second - prev->second);
}

Range PieceWiseLinearCalib::input_range() const
{
	return Range(values.begin()->first, values.rbegin()->first);
}

Range PieceWiseLinearCalib::output_range() const
{
	return range;
}

const std::string NO_YAML_SUPPORT("compiled without YAML support");
PieceWiseLinearCalib::CalibrationMap PieceWiseLinearCalib::load(const YAML::Node &node)
{
#ifdef HAVE_YAML
	CalibrationMap values;
	for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
		values[it->first.as<float>()] = it->second.as<float>();
	return values;
#else
	throw std::runtime_error(NO_YAML_SUPPORT);
#endif
}

PieceWiseLinearCalib::CalibrationMap PieceWiseLinearCalib::load(const std::string &sYAMLFile)
{
#ifdef HAVE_YAML
	return load(YAML::LoadFile(sYAMLFile));
#else
	throw std::runtime_error(NO_YAML_SUPPORT);
#endif
}

}  // namespace tactile
