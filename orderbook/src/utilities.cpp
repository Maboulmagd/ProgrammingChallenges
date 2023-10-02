#include "utilities.h"

// Function to convert a double to the fixed-point representation
uint64_t doubleToFixedPoint(double value) {
    return static_cast<uint64_t>(value * SCALE_FACTOR);
}

// Function to convert the fixed-point representation to a double
double fixedPointToDouble(uint64_t fixedValue) {
    return static_cast<double>(fixedValue) / SCALE_FACTOR;
}
