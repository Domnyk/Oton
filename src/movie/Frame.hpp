#pragma once

#include <opencv2/core.hpp>

using namespace cv;

class Frame {
public:
    Frame(): frameNum_(0) {};
	Frame(unsigned, Mat);
	Frame(const Frame&);

	Frame resize(Size);

	inline const Mat& getPixelMatrix() const {
		return pixelMatrix_;
	}
	inline const Size getSize() const {
		return Size(pixelMatrix_.cols, pixelMatrix_.rows);
	}

    unsigned char* data() {
        return pixelMatrix_.data;
    }

    unsigned int data_length() {
        return pixelMatrix_.total() * pixelMatrix_.elemSize();
    }

    unsigned const char* data() const {
        return pixelMatrix_.data;
    }

    unsigned int data_length() const {
        return pixelMatrix_.total() * pixelMatrix_.elemSize();
    }
private:
    unsigned frameNum_;
	Mat pixelMatrix_;
};
