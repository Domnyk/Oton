#include <iostream>
#include <stdexcept>
#include "VideoStream.hpp"

using namespace std;

VideoStream::VideoStream(const string& fileName) : video_capture_(), num_of_frames_(0) {
	open(fileName);
}

void VideoStream::open(const string& fileName) {
	try {
        video_capture_.open(fileName);

        if(!video_capture_.isOpened()) {
			string errorMsg = "Could not open file: " + fileName;
			throw runtime_error(errorMsg);
		}

        num_of_frames_ = video_capture_.get(CAP_PROP_FRAME_COUNT);
	} catch (std::exception& e) {
		cerr << "An error occured during opening of the video stream: " << e.what() << endl;
	}
}

Frame VideoStream::get_frame(unsigned frameNum) {
	if (frameNum > num_of_frames_ - 1 ) {
		string errorMsg = "Frame index exceeded. Last frame index number: " + 
							to_string(num_of_frames_  - 1) + 
							". Desired frame index number: " + 
							to_string(frameNum); 
		throw runtime_error(errorMsg);
	}


	Mat frameData;
    video_capture_.set(CAP_PROP_POS_FRAMES, frameNum);
    video_capture_ >> frameData;
	return Frame(frameNum, frameData);
}
