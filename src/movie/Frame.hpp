#pragma once

#include <opencv2/core.hpp>

using namespace cv;

class Frame {
public:
    Frame();
	Frame(unsigned, Mat);

	Frame resize(Size);

    const Mat& getPixelMatrix() const;

    const Size getSize() const;

    unsigned char* data();
    unsigned const char* data() const;
    unsigned int data_length() const;
private:
    unsigned frameNum_;
	Mat pixelMatrix_;
};
