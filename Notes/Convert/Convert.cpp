#include "Convert.hpp"

float Convert::degToRad (float degree) {
    return degree * M_PI * 180.0;
}

float Convert::radToDeg (float radian) {
    return radian * 180.0 / M_PI;
}