#pragma once

#include <opencv2/core.hpp>

using namespace cv;

class Frame {
public:
	Frame(unsigned);
	
private:
	unsigned frameNum_;
	Mat frameRepresentatio_;
};