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
#pragma once

#include "Calibration.h"
#include <map>
#include <string>

namespace YAML {
class Node;
}

namespace tactile {

class PieceWiseLinearCalib : public Calibration {
public:
	typedef std::map<float, float> CalibrationMap;

	PieceWiseLinearCalib() {}
	PieceWiseLinearCalib(const CalibrationMap &values);

	void init(const CalibrationMap &values);
	float map(float x) const;
	Range input_range() const;
	Range output_range() const;

	static CalibrationMap load(const YAML::Node &node);
	static CalibrationMap load(const std::string &sYAMLFile);

private:
	CalibrationMap values;
	Range range;
};

}  // namespace tactile
