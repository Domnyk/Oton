#pragma once

#include <opencv2/core.hpp>

using namespace cv;

class Frame {
public:
	Frame(unsigned, Mat);
	Frame resize();
private:
	const unsigned frameNum_;
	const Mat frameData_;
};