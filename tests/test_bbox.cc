#include <math/bbox.hh>

#include "testing.h"

BEGIN_TEST()
	BBoxf bbox(Vector3f(100, 100, 100), -1, -4, 3, 2, 10, 20);

	EXPECT_TRUE(bbox.Contains(Vector3f(100, 100, 110 + 0.01)));
	EXPECT_TRUE(bbox.Contains(Vector3f(100, 100, 120 - 0.01)));

	EXPECT_TRUE(bbox.Contains(Vector3f(100 - 0.99, 100 - 3.99, 115)));
	EXPECT_TRUE(bbox.Contains(Vector3f(100 + 2.99, 100 - 3.99, 115)));
	EXPECT_TRUE(bbox.Contains(Vector3f(100 + 2.99, 100 + 1.99, 115)));
	EXPECT_TRUE(bbox.Contains(Vector3f(100 - 0.99, 100 + 1.99, 115)));

	EXPECT_TRUE(!bbox.Contains(Vector3f(100, 100, 110 - 0.01))); // below
	EXPECT_TRUE(!bbox.Contains(Vector3f(100, 100, 120 + 0.01))); // above

	EXPECT_TRUE(!bbox.Contains(Vector3f(100 - 1.01, 100, 115))); // left
	EXPECT_TRUE(!bbox.Contains(Vector3f(100 + 3.01, 100, 115))); // right
	EXPECT_TRUE(!bbox.Contains(Vector3f(100, 100 - 4.01, 115))); // front
	EXPECT_TRUE(!bbox.Contains(Vector3f(100, 100 + 2.01, 115))); // back
END_TEST()
