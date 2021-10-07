/* ============================================================
 *
 * This file is a part of the RSB project
 *
 * Copyright (C) 2014 by Robert Haschke <rhaschke at techfak dot uni-bielefeld dot de>
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

#include <gtest/gtest.h>
#include "TactileValueArray.h"
#include <math.h>
#include <map>

using namespace tactile;

std::vector<int> VALUES({ -2, -1, 0, 1, 2 });

TEST(TactileValueArray, auto_init)
{
	TactileValueArray sensor;
	sensor.updateValues(VALUES);
	ASSERT_EQ(sensor.size(), VALUES.size());
	std::vector<float> stored = sensor.getValues(TactileValue::rawCurrent);
	for (size_t i = 0; i < stored.size(); ++i)
		EXPECT_EQ(stored[i], VALUES[i]);
}

TEST(TactileValueArray, auto_init_offset)
{
	int offset = 2;
	TactileValueArray sensor;
	sensor.updateValues(VALUES, offset);
	ASSERT_EQ(sensor.size(), VALUES.size() + offset);
	std::vector<float> stored = sensor.getValues(TactileValue::rawCurrent);
	for (size_t i = 0; i < offset; ++i)
		EXPECT_TRUE(isnan(stored[i]));
	for (size_t i = 2; i < stored.size(); ++i)
		EXPECT_EQ(stored[i], VALUES[i - offset]);

	ASSERT_DEATH(sensor.updateValues(VALUES, offset + 1), ".*Assertion .* failed.");
	ASSERT_DEATH(sensor.updateValues(VALUES, -(sensor.size() + 1)), ".*Assertion .* failed.");
}

TEST(TactileValueArray, update_neg_offset)
{
	int offset = 2;
	TactileValueArray sensor(VALUES.size() + offset);
	sensor.updateValues(VALUES);
	std::vector<float> stored = sensor.getValues(TactileValue::rawCurrent);
	ASSERT_EQ(stored.size(), VALUES.size() + offset);
	for (size_t i = 0; i < VALUES.size(); ++i)
		EXPECT_EQ(stored[i], VALUES[i]);
	for (size_t i = VALUES.size(); i < stored.size(); ++i)
		EXPECT_TRUE(isnan(stored[i]));

	sensor.updateValues(VALUES, -VALUES.size());
	stored = sensor.getValues(TactileValue::rawCurrent);
	for (size_t i = 0; i < offset; ++i)
		EXPECT_EQ(stored[i], VALUES[i]);
	for (size_t i = 0; i < VALUES.size(); ++i)
		EXPECT_EQ(stored[i + offset], VALUES[i]);
}

TEST(TactileValueArray, getValues)
{
	int offset = 2;
	TactileValueArray sensor(VALUES.size() + offset);
	sensor.updateValues(VALUES);

	std::vector<float> expected(VALUES.size());
	sensor.getValues(TactileValue::rawCurrent, expected);
	for (size_t i = 0; i < VALUES.size(); ++i)
		EXPECT_EQ(expected[i], VALUES[i]);

	expected.resize(offset);
	sensor.getValues(TactileValue::rawCurrent, expected, -offset);
	for (float i : expected)
		EXPECT_TRUE(isnan(i));

	expected.resize(VALUES.size() + offset + 1);
	ASSERT_DEATH(sensor.getValues(TactileValue::rawCurrent, expected), ".*Assertion .* failed.");
	ASSERT_DEATH(sensor.getValues(TactileValue::rawCurrent, expected, -(sensor.size() + 1)),
	             ".*Assertion .* failed.");
}

TEST(TactileValueArray, accumulate)
{
	TactileValueArray sensor;
	sensor.updateValues(VALUES);
	std::map<TactileValueArray::AccMode, float> expected = { { TactileValueArray::Sum, 0 },
		                                                      { TactileValueArray::SumPositive, 3 },
		                                                      { TactileValueArray::SumNegative, -3 },
		                                                      { TactileValueArray::CountPositive, 2 },
		                                                      { TactileValueArray::CountNegative, 2 },
		                                                      { TactileValueArray::Min, -2 },
		                                                      { TactileValueArray::Max, 2 } };
	for (auto it = expected.begin(); it != expected.end(); ++it) {
		EXPECT_FLOAT_EQ(sensor.accumulate(TactileValue::rawCurrent, it->first, false), it->second);
		EXPECT_FLOAT_EQ(sensor.accumulate(TactileValue::rawMean, it->first, false), it->second);
	}
}
