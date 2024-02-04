#pragma once

#ifndef RECTANGLE_COLLISIONS_HPP
#define RECTANGLE_COLLISIONS_HPP

#include "include.h"

constexpr float EPSILON_FLOAT = 0.0000001f;
constexpr double EPSILON_DOUBLE = 0.0000001;

template <typename T>
std::pair<std::error_code, bool> is_point_in_axis_aligned_rectangle(Point2D<T> p, Point2D<T> a, Point2D<T> b) noexcept {
    // First let's determine which point, a/b is low/hi
    Point2D<T>* low = nullptr;
    Point2D<T>* hi = nullptr;

    if (a.x < b.x) {
        low = &a;
        hi = &b;
    }
    else {
        low = &b;
        hi = &a;
    }

    if (low->x >= hi->x || low->y >= hi->y) {
        return std::make_pair(InputError::ERROR, false);
    }

    assert(low->x < hi->x);
    assert(low->y < hi->y);

    bool res = false;

    if ((p.x >= low->x && p.x <= hi->x) && (p.y >= low->y && p.y <= hi->y)) { res = true; }

    return std::make_pair(InputError::OK, res);
}

template<typename T>
std::pair<std::error_code, bool> is_point_in_non_axis_aligned_rectangle(Point2D<T> p, std::vector<Point2D<T>> rectangle_points) {
    assert(rectangle_points.size() == 4);

    // First need to sort points by x-coordinate
    std::sort(rectangle_points.begin(), rectangle_points.end(), [](const Point2D<T>& a, const Point2D<T>& b) {
        return a.x < b.x;
    });

    // Swap points to allow for anti-clockwise order processing
    std::swap(rectangle_points[2], rectangle_points[3]);

    bool res = false;

    bool broke_out = false;
    bool right = true;// For first line segment, we need to check if point is right, then point might be in rectangle

    for (int i = 0; i < 3; ++i) {
        const Point2D<T>& a = rectangle_points[i];
        const Point2D<T>& b = rectangle_points[i+1];

        if (right) {
            // Get line equation, y = mx + b
            // Need to find slope, or m first
            const float m = (a.y - b.y) / (a.x - b.x);
            // Now get the y-intercept for that line (can use either point)
            const float b = a.y - (m * a.x);
            
            const float line_x_coordinate = (p.y - b) / m;
            // If point p's x-coordinate > the line's x-coordinate at the same y-coordinate as the point,
            // then point is right of the line, we MAY have collision
            if (std::abs(p.x - line_x_coordinate) < EPSILON_FLOAT) {// We have collision
                res = true;
                broke_out = true;
                break;
            }
            else if (p.x < line_x_coordinate - EPSILON_FLOAT) {// No way we have a collision
                res = false;
                broke_out = true;
                break;
            }
            else {
                assert(p.x > line_x_coordinate + EPSILON_FLOAT);
                res = true;
            }

            right = !right;
        }
        
        else {
            assert(right == false);

            // Get line equation, y = mx + b
            // Need to find slope, or m first
            const float m = (a.y - b.y) / (a.x - b.x);
            // Now get the y-intercept for that line (can use either point)
            const float b = a.y - (m * a.x);

            const float line_x_coordinate = (p.y - b) / m;
            // If point p's x-coordinate < line's x-coordinate at the same y-coordinate as the point,
            // then point is left of the line, we MAY have collision
            if (std::abs(p.x - line_x_coordinate) < EPSILON_FLOAT) {// We have collision
                res = true;
                broke_out = true;
                break;
            }
            else if (p.x > line_x_coordinate - EPSILON_FLOAT) {// No way we have a collision
                res = false;
                broke_out = true;
                break;
            }   
            else {
                assert(p.x < line_x_coordinate + EPSILON_FLOAT);
                res = true;
            }
        }
    }

    // Process last point, if need be
    if (!broke_out) {
        const Point2D<T>& a = rectangle_points[3];
        const Point2D<T>& b = rectangle_points[0]; 
        
        {
            // Get line equation, y = mx + b
            // Need to find slope, or m first
            const float m = (a.y - b.y) / (a.x - b.x);
            // Now get the y-intercept for that line (can use either point)
            const float b = a.y - (m * a.x);

            const float line_x_coordinate = (p.y - b) / m;
            // If point p's y-coordinate < the line's y-intercept, then point is left of the line, we MAY have collision
            if (std::abs(p.x - line_x_coordinate) < EPSILON_FLOAT) {// We have collision
                res = true;
            }
            else if (p.x < line_x_coordinate - EPSILON_FLOAT) {// No way we have a collision
                res = false;
            }
            else {
                assert(p.x > line_x_coordinate + EPSILON_FLOAT);
                res = true;
            }
        }
    }

    return std::make_pair(InputError::OK, res);
}

#endif