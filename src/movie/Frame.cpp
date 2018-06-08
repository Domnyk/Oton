#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Frame.hpp"

using namespace std;

Frame::Frame() : frame_num_(), pixel_matrix_() {}

Frame::Frame(unsigned frameNum, Mat pixelMatrix) :
    frame_num_(frameNum), pixel_matrix_(pixelMatrix.clone()) {
}

Frame Frame::resize(Size desiredSize) {
	Frame resizedFrame(*this);
	unsigned const scaleFactorX = 0;
	unsigned const scaleFactorY = 0;

    const Size sourceSize = get_size();
	if (sourceSize.width < desiredSize.width || sourceSize.height < desiredSize.height) {
		throw runtime_error("Desired size is bigger than source size");
	}

    ::resize(this->pixel_matrix_, resizedFrame.pixel_matrix_, desiredSize,
            scaleFactorX, scaleFactorY, INTER_AREA);

	return resizedFrame;
}

 const Mat& Frame::get_pixel_matrix() const {
    return pixel_matrix_;
}
 const Size Frame::get_size() const {
    return Size(pixel_matrix_.cols, pixel_matrix_.rows);
}

unsigned char* Frame::data() {
    return pixel_matrix_.data;
}

unsigned const char* Frame::data() const {
    return pixel_matrix_.data;
}

unsigned int Frame::data_length() const {
    return pixel_matrix_.total() * pixel_matrix_.elemSize();
}

bool Frame::is_key_frame() const {
    if(frame_num_ % Frame::KEY_FRAME_PARAM == 0) {
        return true;
    }

    return false;
}
