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

	int startnum, interval, endnum, totalnum = 0;
	double t = 0.0, percentage;

	if (argc < 7) {
		cout << "Not enough parameters!" << endl;
		return -1;
	}
	else if (argc == 7) {
		startnum = atoi(argv[4]);
		interval = atoi(argv[5]);
		endnum = atoi(argv[6]);
	}
	totalnum = ((endnum - startnum) / interval + 1);

	int ii, kk;
	char filename[20];
	Mat avIM = Mat::zeros(IMG_HEIGHT, IMG_WIDTH, CV_32SC3),		//max of CV_32SC3=2,147,483,647
		stdIM = Mat::zeros(IMG_HEIGHT, IMG_WIDTH, CV_32SC3),	//2147483647 / 127 = 16,909,320 pics (that's enough)
		tempIM ;												//result is the same while using CV_32SC3 or CV_32SC3 (long/double)
	
	/*Compute Average Image*/	
	percentage = 0.0;
	kk = 0;
	cout << "Computing average image:       ";

	t = (double)getTickCount();
	for (ii = startnum; ii <= endnum; ii += interval) {
		string fullname = argv[1];
		sprintf(filename, "\\%d.png", ii);
		fullname.append(filename);
		//cout << fullname << endl;
		
		tempIM = imread(fullname.c_str());
		if (tempIM.empty()) { cout << "Can't open image!\n"; return -1; }
		tempIM.convertTo(tempIM, CV_32SC3);			//convert to the same type as result Mat for adding
		//cout << tempIM << endl;

		avIM += tempIM;
		++kk;

		percentage = (double)kk / (double)totalnum;
		printf("\b\b\b\b\b\b%5.1f%%", percentage* 100.0);

		//if (0 == (kk % 100)) {			//print a dot every 100 images
		//	cout << '.';
		//}
	}
	avIM = avIM * (double)(1.0 / kk);		//multiply (1 / total number of images)

	t = ((double)getTickCount() - t) / getTickFrequency();

	cout << ".......Done!" << endl;
	cout << "Elapsed time for computing average image: ";
	printf("%.2f s\n\n", t);

	/*Compute Standard Deviation Image*/
	percentage = 0.0;
	kk = 0;
	cout << "Computing standard deviation image:       ";

	t = (double)getTickCount();
	for (ii = startnum; ii <= endnum; ii += interval) {
		string fullname = argv[1];
		sprintf(filename, "\\%d.png", ii);
		fullname.append(filename);
		//cout << fullname << endl;

		tempIM = imread(fullname.c_str());
		if (tempIM.empty()) { cout << "Can't open image!\n"; return -1; }
		tempIM.convertTo(tempIM, CV_32SC3);			//convert to the same type as result Mat for adding
		//cout << tempIM << endl;

		tempIM = tempIM - avIM;
		pow(tempIM, 2.0, tempIM);

		stdIM += tempIM;
		++kk;

		percentage = (double)kk / (double)totalnum;
		printf("\b\b\b\b\b\b%5.1f%%", percentage * 100.0);

		//if (0 == (kk % 100)) {			//print a dot every 100 images
		//	cout << '.';
		//}
	}
	
	stdIM = stdIM * (double)(1.0 / (kk - 1));
	sqrt(stdIM, stdIM);
	stdIM = stdIM + Scalar(0.5, 0.5, 0.5);		//round function

	t = ((double)getTickCount() - t) / getTickFrequency();

	cout << ".......Done!" << endl;
	cout << "Elapsed time for computing standard deviation image: ";
	printf("%.2f s\n", t);

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
