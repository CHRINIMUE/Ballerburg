#include "opencv2/opencv.hpp"
#include <conio.h>

using namespace cv;
using namespace std;


int main()
{
	string videofilename = "C:\\videos\\dancing_pair.mp4";

	VideoCapture cap(1);
	if (!cap.isOpened()) {
		printf("Error! Could not open video file %s\n", videofilename.c_str());
		_getch();
		return -1;
	}

	while (true)
	{
		Mat frame;
		cap >> frame;
		int width = frame.cols;
		int height = frame.rows;
		imshow("video frame", frame);
		waitKey(1);
	}
}