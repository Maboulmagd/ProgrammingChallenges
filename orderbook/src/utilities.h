#ifndef ORDERBOOK_SRC_UTILITIES_H
#define ORDERBOOK_SRC_UTILITIES_H

#include <cstdint>

// TODO Ideally each header would have a namespace

// Define the scaling factor for the fixed-point representation (10^5 for 5 decimal places)
static constexpr uint64_t SCALE_FACTOR = 100000;

// Function to convert a double to the fixed-point representation
uint64_t doubleToFixedPoint(double value);

// Function to convert the fixed-point representation to a double
double fixedPointToDouble(uint64_t fixedValue);

#endif //ORDERBOOK_SRC_UTILITIES_H
