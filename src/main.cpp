#define BOOST_LOG_DYN_LINK

#include <iostream>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include "network/tcp_server.hpp"
#include "network/tcp_connection.hpp"

using namespace cv;
using namespace std;

int main() {
	try {
		int frameNum = -1;
		Mat frame;
		string filePath = "./data/Megamind.avi";

		VideoCapture capture(filePath);
		if (!capture.isOpened()) {
        	throw "Could not open file";
    	}

    	namedWindow("TEST", WINDOW_AUTOSIZE);
    	moveWindow("TEST", 400, 0);

    	
    	for(;;) {
    		capture >> frame;
    		if (frame.empty()) {
    			cout << "End of video" << endl;
    			break;
    		}

    		++frameNum;
    		cout << "Frame: " << frameNum << endl;

    		imshow("TEST", frame);
    		waitKey(static_cast<int>(capture.get(CAP_PROP_FPS )));
    	}

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	/* 
	try {
    	boost::asio::io_context io_context;
    	tcp_server server(io_context);

    	BOOST_LOG_TRIVIAL(info) << "Server is running on port: " << server.get_port();

    	io_context.run();
  	} catch (std::exception& e) {
    	std::cerr << e.what() << std::endl;
  	}
  	*/

  	return 0;
}