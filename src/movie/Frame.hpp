#pragma once

#include <opencv2/core.hpp>

using namespace cv;

class Frame {
public:
	Frame(unsigned, Mat);
	Frame(const Frame&);

	Frame resize(Size);

	inline const Mat& getPixelMatrix() const {
		return pixelMatrix_;
	}
	inline const Size getSize() const {
		return Size(pixelMatrix_.cols, pixelMatrix_.rows);
	}
private:
	const unsigned frameNum_;
	Mat pixelMatrix_;
};