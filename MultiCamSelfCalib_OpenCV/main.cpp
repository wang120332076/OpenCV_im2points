#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdlib.h>

#define	IMG_WIDTH	1280	
#define	IMG_HEIGHT	720
#define	START_NUM	4030
#define END_NUM		25320
#define INTERVAL	10
#define TOTAL_NUM	((END_NUM - START_NUM)/INTERVAL + 1)

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	if (argc < 4) return -1;

	int ii, kk;
	char filename[20];
	Mat avIM = Mat::zeros(IMG_HEIGHT, IMG_WIDTH, CV_64FC3),		//max of CV_64FC3=2,147,483,647
		stdIM = Mat::zeros(IMG_HEIGHT, IMG_WIDTH, CV_64FC3),	//2147483647 / 127 = 16,909,320 pics (that's enough)
		tempIM ;												//result is the same while using CV_64FC3 or CV_64FC3 (long/double)
	
	/*Compute Average Image*/
	kk = 0;
	for (ii = START_NUM; ii <= END_NUM; ii += INTERVAL) {
		string fullname = argv[1];
		sprintf(filename, "\\%d.png", ii);
		fullname.append(filename);
		//cout << fullname << endl;
		
		tempIM = imread(fullname.c_str());
		if (tempIM.empty()) { cout << "Can't open image!\n"; return -1; }
		tempIM.convertTo(tempIM, CV_64FC3);			//convert to the same type as result Mat for adding
		//cout << tempIM << endl;

		avIM += tempIM;
		++kk;

		if (0 == (kk % 100)) {			//print a dot every 100 images
			cout << '.';
		}
	}
	avIM = avIM * (double)(1.0 / kk);	//multiply (1 / total number of images)
	cout << "\nAverage Computed!" << endl;

	/*Compute Standard Deviation Image*/
	kk = 0;
	for (ii = START_NUM; ii <= END_NUM; ii += INTERVAL) {
		string fullname = argv[1];
		sprintf(filename, "\\%d.png", ii);
		fullname.append(filename);
		//cout << fullname << endl;

		tempIM = imread(fullname.c_str());
		if (tempIM.empty()) { cout << "Can't open image!\n"; return -1; }
		tempIM.convertTo(tempIM, CV_64FC3);			//convert to the same type as result Mat for adding
		//cout << tempIM << endl;

		tempIM = tempIM - avIM;
		pow(tempIM, 2.0, tempIM);

		stdIM += tempIM;
		++kk;

		if (0 == (kk % 100)) {			//print a dot every 100 images
			cout << '.';
		}
	}
	cout << "\nStandard Deviation Computed!" << endl;
	stdIM = stdIM * (double)(1.0 / (kk - 1));
	sqrt(stdIM, stdIM);

	avIM.convertTo(avIM, CV_8UC3);				//convert back to 8-bit depth to show&save
	stdIM.convertTo(stdIM, CV_8UC3);

	namedWindow("Average Image");
	imshow("Average Image", avIM);
	namedWindow("Standard Deviation Image");
	imshow("Standard Deviation Image", stdIM);

	string savename = argv[2];
	imwrite(savename.c_str(), avIM);

	savename = argv[3];
	imwrite(savename.c_str(), stdIM);

	cout << '\a';
	waitKey();
	
	return 0;
}