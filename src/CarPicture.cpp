/*
 * CarPicture.cpp
 *
 *  Created on: 21/01/2011
 *      Author: SEMDFF
 */

#include <cv.h>
#include <highgui.h>
#include "CarPicture.h"
#include "commons.h"

using namespace anpr;
using namespace cv;

/**
 * Constructor
 */
CarPicture::CarPicture(cv::Mat input, bool clone) :
	data(input), cloned(clone), projectionSmoothFactor(20), bandClipFactor(0.5) {
	if (clone) {
		data = input.clone();
	}
}

/**
 * Destructor
 */
CarPicture::~CarPicture() {
	if (cloned)
		data.release();
}

/**
 *
 */
Size_<int> CarPicture::getSize() {
	return data.size();
}

/**
 * Calculates and returns the specified number of bands where
 * there is most probably a number plate
 */
std::vector<Band>* CarPicture::getProbableBands(int nBands) {

	// Calculate vertical gradient
	Mat dy = Mat(data.size(), CV_32FC1);
	Sobel(data, dy, CV_32F, 1, 0);
	dy = abs(dy);

#ifdef SHOW_ALL
	Mat dyImg, tmp;
	cv::normalize(dy, tmp, 0, 255, NORM_MINMAX);
	convertScaleAbs(tmp, dyImg);
	namedWindow("dy", CV_WINDOW_AUTOSIZE);
	imshow("dy", dyImg);
#endif

	// Calculate vertical projection of the gradient
	Mat dyPrj = Mat(1, data.size().height, CV_32FC1);
	reduce(dy, dyPrj, 1, CV_REDUCE_SUM);
	Mat dyPrj2 = dyPrj.clone();
	blur(dyPrj2, dyPrj, Size(1, projectionSmoothFactor)); // smooth it

#ifdef SHOW_ALL
	Mat tmp2;
	normalize(repeat(dyPrj, 1, 20), tmp, 0, 255, NORM_MINMAX);
	convertScaleAbs(tmp, tmp2);
	namedWindow("dyPrj", CV_WINDOW_AUTOSIZE);
	imshow("dyPrj", tmp2);
#endif

	// Pick N bands
	std::vector<Band> *bandList = new std::vector<Band>();
	for (int i = 0; i < nBands; i++) {
		// Find max peak in projection
		double max;
		Point maxLoc;
		minMaxLoc(dyPrj, NULL, &max, NULL, &maxLoc); // find max

		// Select band
		Rect rect;
		floodFill(dyPrj, maxLoc, cvScalarAll(0), &rect, cvScalarAll(
				bandClipFactor * max), cvScalarAll(0), 4
				| CV_FLOODFILL_FIXED_RANGE);
		// the selected band should be now zeroed in the projection
		// so it will not be selected again
		int h = rect.height;
		int y0 = max(0,rect.y);
		int y1 = min(data.rows,y0 + h);
		Band *band = new Band(data(Rect(0, y0, data.cols, y1 - y0)), y0, y1);

		// Add to band list
		bandList->push_back(*band);
	}

	return bandList;
}

void CarPicture::setBandClipFactor(double bcf) {
	bandClipFactor = bcf;
}

void CarPicture::setProjectionSmoothFactor(int psf) {
	projectionSmoothFactor = psf;
}

