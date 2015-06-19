/* ============================================================
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
#include "PieceWiseLinearCalib.h"

using namespace tactile;

TEST(PieceWiseLinearCalib, min_size)
{
	PieceWiseLinearCalib c;
	c.init(PieceWiseLinearCalib::CalibrationMap({{0,0}, {100,1}}));
	EXPECT_FLOAT_EQ(c.map(0), 0);
	EXPECT_FLOAT_EQ(c.map(20), 0.2);
	EXPECT_FLOAT_EQ(c.map(100), 1);
	EXPECT_FLOAT_EQ(c.map(-1), 0);
	EXPECT_FLOAT_EQ(c.map(101), 1);
}

TEST(PieceWiseLinearCalib, constant)
{
	PieceWiseLinearCalib c;
	c.init(PieceWiseLinearCalib::CalibrationMap({{0,0}, {100,0}}));
	EXPECT_FLOAT_EQ(c.map(0), 0);
	EXPECT_FLOAT_EQ(c.map(20), 0);
	EXPECT_FLOAT_EQ(c.map(100), 0);
	EXPECT_FLOAT_EQ(c.map(-1), 0);
	EXPECT_FLOAT_EQ(c.map(101), 0);
}

TEST(PieceWiseLinearCalib, two_slopes)
{
	PieceWiseLinearCalib c;
	c.init(PieceWiseLinearCalib::CalibrationMap({{0,0}, {100,1}, {200,5}}));
	EXPECT_FLOAT_EQ(c.map(-1), 0);
	EXPECT_FLOAT_EQ(c.map(0), 0);
	EXPECT_FLOAT_EQ(c.map(20), 0.2);
	EXPECT_FLOAT_EQ(c.map(100), 1);
	EXPECT_FLOAT_EQ(c.map(101), 1.04);
	EXPECT_FLOAT_EQ(c.map(125), 2);
	EXPECT_FLOAT_EQ(c.map(150), 3);
	EXPECT_FLOAT_EQ(c.map(175), 4);
	EXPECT_FLOAT_EQ(c.map(200), 5);
	EXPECT_FLOAT_EQ(c.map(201), 5);
}

void test_trapez(const PieceWiseLinearCalib &c) {
	EXPECT_FLOAT_EQ(c.map(-1), 0);
	EXPECT_FLOAT_EQ(c.map(0), 0);
	EXPECT_FLOAT_EQ(c.map(0.2), 0.2);
	EXPECT_FLOAT_EQ(c.map(1), 1);
	EXPECT_FLOAT_EQ(c.map(1.2), 1);
	EXPECT_FLOAT_EQ(c.map(1.8), 1);
	EXPECT_FLOAT_EQ(c.map(2), 1);
	EXPECT_FLOAT_EQ(c.map(2.2), 0.8);
	EXPECT_FLOAT_EQ(c.map(2.8), 0.2);
	EXPECT_FLOAT_EQ(c.map(3), 0);
}

TEST(PieceWiseLinearCalib, trapez)
{
	PieceWiseLinearCalib c;
	c.init(PieceWiseLinearCalib::CalibrationMap({{0,0}, {1,1}, {2,1}, {3,0}}));
	test_trapez(c);
}

TEST(PieceWiseLinearCalib, multi_init)
{
	PieceWiseLinearCalib c;
	PieceWiseLinearCalib::CalibrationMap m({{0,1}, {1,2}, {2,3}, {3,4}});
	PieceWiseLinearCalib::CalibrationMap m_final({{0,0}, {1,1}, {2,1}, {3,0}});
	for (auto it=m_final.begin(), end=m_final.end(); it!=end; ++it)
		m[it->first] = it->second;
	c.init(m);
	test_trapez(c);
}

TEST(PieceWiseLinearCalib, yaml)
{
#ifdef HAVE_YAML
	PieceWiseLinearCalib c(PieceWiseLinearCalib::load("trapez.yaml"));
	test_trapez(c);
#else
	ASSERT_THROW(PieceWiseLinearCalib::load("trapez.yaml"), std::runtime_error);
#endif
}
