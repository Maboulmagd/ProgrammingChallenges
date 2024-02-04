#pragma once

#ifndef POLYGON_COLLISIONS_HPP
#define POLYGON_COLLISIONS_HPP

#include "include.h"

constexpr float EPSILON_FLOAT = 0.0000001f;
constexpr double EPSILON_DOUBLE = 0.0000001;

// I beleive this approach works for both convex and concave polygons???
template <typename T>
std::pair<std::error_code, bool> is_point_in_polygon(Point2D<T> p, std::vector<Point2D<T>> polygon_points) noexcept {
    int counter = 0;// Count how many intersections with line segments of the polygon

    for (int i = 0; i < polygon_points.size(); ++i) {
        Point2D<T> a;
        Point2D<T> b;
        if (i == polygon_points.size()-1) {
            a = polygon_points[polygon_points.size()-1];
            b = polygon_points[0]; 
        }
        else {
            a = polygon_points[i];
            b = polygon_points[i+1];
        }

        if (p.y < std::min(a.y, b.y) - EPSILON_FLOAT) { continue; }
        if (p.y > std::max(a.y, b.y) + EPSILON_FLOAT) { continue; }
        if (p.x > std::max(a.x, b.x) + EPSILON_FLOAT) { continue; }

        if (!(std::abs(a.y - b.y) < EPSILON_FLOAT)) {
            // We have the point, and we are drawing horizontal line to the right, so we know its y coordinate of the line, but where does it intersect
            // i.e what's the horizontal line's x coordinate (where does it intersect with our line segment a-b)?
            // Using linear interpolation (equation of slopes, the value of x can be solved for)
            const float x_intersection = (((p.y - a.y) * (b.x - a.x)) / (b.y - a.y)) + a.x;
            if (std::abs(a.x - b.x) < EPSILON_FLOAT || p.x <= x_intersection + EPSILON_FLOAT) {
                ++counter;
            }
        }
    }

    bool res = true;
    if (counter % 2 == 0) {
        res = false;
    }

    return std::make_pair(InputError::OK, res);
}

#endif
