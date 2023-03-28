// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#define cimg_use_jpeg
#define cimg_use_png

#include "CImg/CImg.h"
#include "CC_Algorithm.h"
#include "CC_Constants.h"
#include "CC_Converter.h"
#include "CC_Interface.h"
#include "CC_Mapper.h"
#include "CC_Point.h"

#endif //PCH_H
