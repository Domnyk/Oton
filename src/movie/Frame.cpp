#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "Frame.hpp"

using namespace std;

Frame::Frame(unsigned frameNum, Mat pixelMatrix) :
    frameNum_(frameNum), pixelMatrix_(pixelMatrix.clone()) {
}

Frame::Frame(const Frame& otherFrame) :
	frameNum_(otherFrame.frameNum_), 
	pixelMatrix_(otherFrame.pixelMatrix_.clone()) {
}

Frame Frame::resize(Size desiredSize) {
	Frame resizedFrame(*this);
	unsigned const scaleFactorX = 0;
	unsigned const scaleFactorY = 0;

	const Size sourceSize = getSize();
	if (sourceSize.width < desiredSize.width || sourceSize.height < desiredSize.height) {
		throw runtime_error("Desired size is bigger than source size");
	}

	try {
		::resize(this->pixelMatrix_, resizedFrame.pixelMatrix_, desiredSize, 
			scaleFactorX, scaleFactorY, INTER_AREA);
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}

	return resizedFrame;
}
