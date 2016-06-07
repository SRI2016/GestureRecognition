#ifndef KINECTMOTION_H
#define KINECTMOTION_H
#include <string>
#include <opencv\cv.h>
#include "Image.h"

class KinectMotion {
public:
	KinectMotion(std::string iDepth, std::string iRgb);
	Image getDepth();
	Image getRgb();
private:
	Image depth;
	Image rgb;
};
#endif