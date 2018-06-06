#include <iostream>
#include <stdexcept>
#include "VideoStream.hpp"

using namespace std;

VideoStream::VideoStream(const string& fileName) : videoCapture_(), num_of_frames_(0) {
	open(fileName);
}

void VideoStream::open(const string& fileName) {
	try {
		videoCapture_.open(fileName);

		if(!videoCapture_.isOpened()) {
			string errorMsg = "Could not open file: " + fileName;
			throw runtime_error(errorMsg);
		}

		num_of_frames_ = videoCapture_.get(CAP_PROP_FRAME_COUNT);
	} catch (std::exception& e) {
		cerr << "An error occured during opening of the video stream: " << e.what() << endl;
	}
}

Frame VideoStream::getFrame(unsigned frameNum) {
	if (frameNum > num_of_frames_ - 1 ) {
		string errorMsg = "Frame index exceeded. Last frame index number: " + 
							to_string(num_of_frames_  - 1) + 
							". Desired frame index number: " + 
							to_string(frameNum); 
		throw runtime_error(errorMsg);
	}


	Mat frameData;
	videoCapture_.set(CAP_PROP_POS_FRAMES, frameNum);
	videoCapture_ >> frameData;
	return Frame(frameNum, frameData);
}
