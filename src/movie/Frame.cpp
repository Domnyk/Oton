#include <iostream>
#include <opencv2/imgproc.hpp>
#include "Frame.hpp"

using namespace std;

Frame::Frame(unsigned frameNum, Mat pixelMatrix) :
	frameNum_(frameNum), pixelMatrix_(pixelMatrix) {
}

Frame::Frame(const Frame& otherFrame) :
	frameNum_(otherFrame.frameNum_), 
	pixelMatrix_(otherFrame.pixelMatrix_.clone()) {
}

Frame Frame::resize(Size desiredSize) {
	Frame resizedFrame(*this);
	unsigned const scaleFactorX = 0;
	unsigned const scaleFactorY = 0;

	try {
		::resize(this->pixelMatrix_, resizedFrame.pixelMatrix_, desiredSize, 
			scaleFactorX, scaleFactorY, INTER_AREA);
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}

	return resizedFrame;
}