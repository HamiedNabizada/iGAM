#pragma once
#include <initializer_list>
    enum class Direction {z_positive, z_negative, x_negative, x_positive, y_negative, y_positive};
    constexpr std::initializer_list<Direction> all_Direction = { Direction::z_positive, Direction::z_negative, Direction::x_negative, Direction::x_positive, Direction::y_negative, Direction::y_positive };

