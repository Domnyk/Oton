#include <opencv2/core.hpp>

using namespace cv;

class Resolution {
	static const Size get1080p() {
		return Size(1920, 1080);
	}

	static const Size get720p() {
		return Size(1280, 720);
	}

	static const Size get360p() {
		return Size(480, 360);
	}

	static const Size get144p() {
		return Size(256, 144);
	}
};