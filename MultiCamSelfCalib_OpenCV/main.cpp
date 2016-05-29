#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdlib.h>

#define	IMG_WIDTH	1280	
#define	IMG_HEIGHT	720
#define	START_NUM	295
#define END_NUM		500
#define TOTAL_NUM	(END_NUM - START_NUM + 1)

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	if (argc < 2) return -1;

	int ii, kk;
	char filename[20];
	Mat avIM = Mat::zeros(IMG_HEIGHT, IMG_WIDTH, CV_32SC3),		//max of CV_32SC3=2,147,483,647
		stdIM = Mat::zeros(IMG_HEIGHT, IMG_WIDTH, CV_32SC3),	//2147483647 / 127 = 16,909,320 pics (that's enough)
		tempIM ;
	

	for (ii = START_NUM; ii <= END_NUM; ++ii) {
		string fullname = argv[1];
		sprintf(filename, "\\%d.png", ii);
		fullname.append(filename);
		//cout << fullname << endl;
		
		tempIM = imread(fullname.c_str());
		if (tempIM.empty()) { cout << "Can't open image!\n"; return -1; }
		tempIM.convertTo(tempIM, CV_32SC3);			//convert to the same type as result Mat for adding
		//cout << tempIM << endl;

		avIM += tempIM;
	}
	avIM = avIM * (double)(1.0 / TOTAL_NUM);	//multiply (1 / total number of images)
	avIM.convertTo(avIM, CV_8SC3);				//convert back to 8-bit depth to show
	
	namedWindow("Testing");
	imshow("Testing", avIM);
	//cout << avIM << endl;

	waitKey();
	
	return 0;
}