/*
 * Band.h
 *
 *  Created on: 21/01/2011
 *      Author: SEMDFF
 */

#ifndef BAND_H_
#define BAND_H_

#include <cv.h>
#include "Plate.h"

using namespace cv;

namespace anpr {

class Band {

private:
	Mat data;
	int top, bottom;
	bool cloned;
	int projectionSmoothFactor;
	double plateClipFactor;

public:

	/**
	 *
	 */
	Band(Mat data, int y0, int y1, bool clone = false);

	/**
	 *
	 */
	virtual ~Band();

	/**
	 *
	 */
	Mat& getData();

	/**
	 *
	 */
	int getTop();
	int getBottom();

	/**
	 *
	 */
	std::vector<Plate> *getProbablePlateRegion(int nPlates);

	/*
	 *
	 */
	void setPlateClipFactor(double pcf);

	/**
	 *
	 */
	void setProjectionSmoothFactor(int psf);
};

}

#endif /* BAND_H_ */
