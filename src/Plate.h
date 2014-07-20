/*
 * Plate.h
 *
 *  Created on: 21/01/2011
 *      Author: SEMDFF
 */

#ifndef PLATE_H_
#define PLATE_H_

#include <cv.h>
#include "highgui.h"

using namespace cv;

namespace anpr {

/**
 *
 */
class Plate {

private:
	Mat data;
	int top, bottom, left, right;
	bool cloned;

public:
	Plate(Mat input, int x0, int y0, int x1, int y1, bool clone = false);
	virtual ~Plate();

	// Size
	Size getSize();

	//
	Mat getData();

	//
	int getTop();
	int getBottom();
	int getLeft();
	int getRight();
};

}

#endif /* PLATE_H_ */
