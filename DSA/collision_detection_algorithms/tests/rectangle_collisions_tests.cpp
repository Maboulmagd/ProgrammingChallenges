#include "rectangle_collisions.hpp"

TEST(AxisAlignedRectangleCollision_test, RectanglePointsInvalid) {
    Point2D<int> p(1, 2);

    Point2D<int> a(0, 0);
    Point2D<int> b(0, 1);

    const auto result = is_point_in_axis_aligned_rectangle(p, a, b);

    ASSERT_EQ(result.first, InputError::ERROR);
    ASSERT_EQ(result.second, false);
}

TEST(AxisAlignedRectangleCollision_test, PointOnRectangle) {
    Point2D<int> p(0, 0);

    Point2D<int> a(0, 0);
    Point2D<int> b(5, 5);

    const auto result = is_point_in_axis_aligned_rectangle(p, a, b);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, true);
}

TEST(AxisAlignedRectangleCollision_test, PointInsideRectangle) {
    Point2D<int> p(1, 2);

    Point2D<int> a(0, 0);
    Point2D<int> b(5, 5);

    const auto result = is_point_in_axis_aligned_rectangle(p, a, b);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, true);
}

TEST(AxisAlignedRectangleCollision_test, PointOutsideRectangle) {
    Point2D<int> p(6, 6);

    Point2D<int> a(0, 0);
    Point2D<int> b(5, 5);

    const auto result = is_point_in_axis_aligned_rectangle(p, a, b);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, false);
}

TEST(NonAxisAlignedRectangleCollision_test, PointOnNonAxisAlignedRectangle) {
    Point2D<int> p(3, 3);

    std::vector<Point2D<int>> non_axis_aligned_rect_points;
    non_axis_aligned_rect_points.push_back(Point2D<int>(2, 4));
    non_axis_aligned_rect_points.push_back(Point2D<int>(3, 3));
    non_axis_aligned_rect_points.push_back(Point2D<int>(6, 6));
    non_axis_aligned_rect_points.push_back(Point2D<int>(5, 7));

    const auto result = is_point_in_non_axis_aligned_rectangle(p, non_axis_aligned_rect_points);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, true);
}

TEST(NonAxisAlignedRectangleCollision_test, PointInsideNonAxisAlignedRectangle) {
    Point2D<int> p(3, 4);

    std::vector<Point2D<int>> non_axis_aligned_rect_points;
    non_axis_aligned_rect_points.push_back(Point2D<int>(2, 4));
    non_axis_aligned_rect_points.push_back(Point2D<int>(3, 3));
    non_axis_aligned_rect_points.push_back(Point2D<int>(6, 6));
    non_axis_aligned_rect_points.push_back(Point2D<int>(5, 7));

    const auto result = is_point_in_non_axis_aligned_rectangle(p, non_axis_aligned_rect_points);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, true);
}

TEST(NonAxisAlignedRectangleCollision_test, PointOutsideNonAxisAlignedRectangle) {
    Point2D<int> p(0, 0);

    std::vector<Point2D<int>> non_axis_aligned_rect_points;
    non_axis_aligned_rect_points.push_back(Point2D<int>(2, 4));
    non_axis_aligned_rect_points.push_back(Point2D<int>(3, 3));
    non_axis_aligned_rect_points.push_back(Point2D<int>(6, 6));
    non_axis_aligned_rect_points.push_back(Point2D<int>(5, 7));

    const auto result = is_point_in_non_axis_aligned_rectangle(p, non_axis_aligned_rect_points);

    ASSERT_EQ(result.first, InputError::OK);
    ASSERT_EQ(result.second, false);
}