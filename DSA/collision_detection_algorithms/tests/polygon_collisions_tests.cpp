#include "polygon_collisions.hpp"

TEST(PolygonCollision_test, PointOnPolygon) {
    Point2D<int> p(2, 2);

    std::vector<Point2D<int>> polygon_points;
    polygon_points.push_back(Point2D<int>(2, 2));
    polygon_points.push_back(Point2D<int>(4, 6));
    polygon_points.push_back(Point2D<int>(8, 7));
    polygon_points.push_back(Point2D<int>(10, 3));
    polygon_points.push_back(Point2D<int>(7, 1));

    const auto result = is_point_in_polygon(p, polygon_points);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, true);
}

TEST(PolygonCollision_test, PointInsidePolygon) {
    Point2D<int> p(4, 4);

    std::vector<Point2D<int>> polygon_points;
    polygon_points.push_back(Point2D<int>(2, 2));
    polygon_points.push_back(Point2D<int>(4, 6));
    polygon_points.push_back(Point2D<int>(8, 7));
    polygon_points.push_back(Point2D<int>(10, 3));
    polygon_points.push_back(Point2D<int>(7, 1));

    const auto result = is_point_in_polygon(p, polygon_points);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, true);
}

TEST(PolygonCollision_test, PointOutsidePolygon) {
    Point2D<int> p(0, 0);
    
    std::vector<Point2D<int>> polygon_points;
    polygon_points.push_back(Point2D<int>(2, 2));
    polygon_points.push_back(Point2D<int>(4, 6));
    polygon_points.push_back(Point2D<int>(8, 7));
    polygon_points.push_back(Point2D<int>(10, 3));
    polygon_points.push_back(Point2D<int>(7, 1));

    const auto result = is_point_in_polygon(p, polygon_points);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, false);
}