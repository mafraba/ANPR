/*
 * Plate.cpp
 *
 *  Created on: 21/01/2011
 *      Author: SEMDFF
 */

#include <cv.h>
#include "Plate.h"

namespace anpr {

Plate::~Plate() {
	if (cloned)
		data.release();
}

Plate::Plate(Mat input, int x0, int y0, int x1, int y1, bool clone) :
	data(input), top(y0), bottom(y1), left(x0), right(x1), cloned(clone) {
	if (clone) {
		data = input.clone();
	}
}

Mat Plate::getData() {
	return data;
}

Size Plate::getSize() {
	return Size(right - left, bottom - top);
}

int Plate::getRight() {
	return right;
}

int Plate::getTop() {
	return top;
}

int Plate::getBottom() {
	return bottom;
}

int Plate::getLeft() {
	return left;
}

}
