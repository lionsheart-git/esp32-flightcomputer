/**
 * @file Constants.hpp
 * @brief Collection of constants used throughout the program.
 */

#ifndef FLIGHTCOMPUTER_CONSTANTS_HPP
#define FLIGHTCOMPUTER_CONSTANTS_HPP

/**
 * @def EPSILON
 * @brief General threshold to add or subtract.
 *
 * Due to numerical instability values sometimes are not as exact as expected after an operation.
 * Here EPSILON can be used to account for that.
 */
#ifndef EPSILON
#define EPSILON 1E-4
#endif

#endif //FLIGHTCOMPUTER_CONSTANTS_HPP
