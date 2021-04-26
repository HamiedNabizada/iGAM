#include "helpFunctions.h"

std::string direction_to_string(Direction direction) {
    switch (direction) {
    case Direction::z_positive:
        return "z+";
    case Direction::z_negative:
        return "z-";
    case Direction::x_positive:
        return "x+";
    case Direction::x_negative:
        return "x-";
    case Direction::y_positive:
        return "y+";
    case Direction::y_negative:
        return "y-";
    default:
        return "Invalid direction";
    }
};
