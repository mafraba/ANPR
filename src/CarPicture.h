/*
 * CarPicture.h
 *
 *  Created on: 21/01/2011
 *      Author: SEMDFF
 */

#ifndef CARPICTURE_H_
#define CARPICTURE_H_

#include <vector>
#include <cv.h>
#include "Band.h"

using namespace cv;

namespace anpr {

class CarPicture {

private:

	//
	Mat data;
	//
	const bool cloned;
	//
	int projectionSmoothFactor;
	//
	double bandClipFactor;

public:

	/**
	 *
	 */
	CarPicture(cv::Mat input, bool clone = false);

	/**
	 *
	 */
	virtual ~CarPicture();

	/**
	 *
	 */
	Size_<int> getSize();

	/**
	 * Calculates and returns the specified number of bands where
	 * there is most probably a number plate
	 */
	std::vector<Band>* getProbableBands(int nBands);

	void setBandClipFactor(double bcf);

	/**
	 *
	 */
	void setProjectionSmoothFactor(int psf);
};

}

#endif /* CARPICTURE_H_ */
