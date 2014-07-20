/*
 * Band.cpp
 *
 *  Created on: 21/01/2011
 *      Author: SEMDFF
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cv.h>
#include "Band.h"
#include "commons.h"

namespace anpr {

Band::Band(cv::Mat input, int y0, int y1, bool clone) :
	data(input), top(y0), bottom(y1), cloned(clone),
			projectionSmoothFactor(100), plateClipFactor(0.5) {
	if (clone)
		data = input.clone();
}

Band::~Band() {
	if (cloned)
		data.release();
}

void Band::setProjectionSmoothFactor(int psf) {
	projectionSmoothFactor = psf;
}

Mat& Band::getData() {
	return data;
}

int Band::getBottom() {
	return bottom;
}

int Band::getTop() {
	return top;
}

void Band::setPlateClipFactor(double pcf) {
	plateClipFactor = pcf;
}

std::vector<Plate> *Band::getProbablePlateRegion(int nPlates) {

	static int bandNumber = 1;

	// Calculate horizontal gradient
	Mat dx = Mat(data.size(), CV_32FC1);
	Sobel(data, dx, CV_32F, 1, 1);
	dx = abs(dx);

#ifdef SHOW_ALL
	Mat dxImg, tmp;
	normalize(dx.clone(), tmp, 0, 255, NORM_MINMAX);
	convertScaleAbs(tmp, dxImg);
	std::ostringstream dxs;
	dxs << "dx " << bandNumber;
	namedWindow(dxs.str(), CV_WINDOW_AUTOSIZE);
	imshow(dxs.str(), dxImg);
#endif

	// Calculate horizontal projection of the gradient
	Mat dxPrj = Mat(data.size().width, 1, CV_32FC1);
	reduce(dx, dxPrj, 0, CV_REDUCE_SUM);
	Mat dxPrj2 = dxPrj.clone();
	blur(dxPrj2, dxPrj, Size(projectionSmoothFactor, 1)); // smooth it

#ifdef SHOW_ALL
	Mat dxPrjImg;
	normalize(repeat(dxPrj, 20, 1), tmp, 0, 255, NORM_MINMAX);
	convertScaleAbs(tmp, dxPrjImg);
	std::ostringstream dxps;
	dxps << "dx " << bandNumber << " prj";
	namedWindow(dxps.str(), CV_WINDOW_AUTOSIZE);
	imshow(dxps.str(), dxPrjImg);
#endif

	// Pick N plates
	std::vector<Plate> *plateList = new std::vector<Plate>();
	for (int i = 0; i < nPlates; i++) {
		// Find max peak in projection
		double max;
		Point maxLoc;
		minMaxLoc(dxPrj, NULL, &max, NULL, &maxLoc); // find max

		// Select plate
		Rect rect;
		floodFill(dxPrj, maxLoc, Scalar(0), &rect,
				Scalar(plateClipFactor * max), Scalar(0), 4
						| CV_FLOODFILL_FIXED_RANGE);
		// the selected band should have been now zeroed in the projection
		// so it will not be selected again
		int w = rect.width;
		int x0 = max(0, rect.x );
		int x1 = min(data.cols, x0 + w );
		Rect roi(x0, 0, x1 - x0, data.rows);
		double aratio = roi.width / (double) roi.height;
		if (aratio > 3 && aratio < 10) {
			Plate *plate = new Plate(data(roi), x0, top, x1, bottom);
			// Add to plate list
			plateList->push_back(*plate);
		}
	}

	bandNumber++;

	return plateList;
}

}
