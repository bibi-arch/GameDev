/// @file Convert.hpp
/// @author Bibiane Gagné Caron

/// @class Convert
/// @brief Conversions

#define _USE_MATH_DEFINES

#include <cmath>

class Convert {
public:
    /// @brief Degré à Radian
    /// @param degree Degré
    /// @return Radian
    static float degToRad (float degree);

    /// @brief Radian à Degré
    /// @param radian Radian
    /// @return Degré
    static float radToDeg (float radian);
};