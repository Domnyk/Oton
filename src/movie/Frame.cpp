#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Frame.hpp"

using namespace std;

Frame::Frame() : frameNum_(), pixelMatrix_() {}

Frame::Frame(unsigned frameNum, Mat pixelMatrix) :
    frameNum_(frameNum), pixelMatrix_(pixelMatrix.clone()) {
}

Frame Frame::resize(Size desiredSize) {
	Frame resizedFrame(*this);
	unsigned const scaleFactorX = 0;
	unsigned const scaleFactorY = 0;

	const Size sourceSize = getSize();
	if (sourceSize.width < desiredSize.width || sourceSize.height < desiredSize.height) {
		throw runtime_error("Desired size is bigger than source size");
	}

    ::resize(this->pixelMatrix_, resizedFrame.pixelMatrix_, desiredSize,
            scaleFactorX, scaleFactorY, INTER_AREA);

	return resizedFrame;
}

 const Mat& Frame::getPixelMatrix() const {
    return pixelMatrix_;
}
 const Size Frame::getSize() const {
    return Size(pixelMatrix_.cols, pixelMatrix_.rows);
}

unsigned char* Frame::data() {
    return pixelMatrix_.data;
}

unsigned const char* Frame::data() const {
    return pixelMatrix_.data;
}

unsigned int Frame::data_length() const {
    return pixelMatrix_.total() * pixelMatrix_.elemSize();
}

bool Frame::is_key_frame() const {
    if(frameNum_ % Frame::KEY_FRAME_PARAM == 0) {
        return true;
    }

    return false;
}
