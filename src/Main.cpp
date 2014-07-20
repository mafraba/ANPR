/*
 * Main.cpp
 *
 *  Created on: 21/01/2011
 *      Author: manuel
 */

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "CarPicture.h"
#include "Band.h"
#include "Plate.h"

using namespace std;
using namespace cv;
using namespace anpr;

int main(int argc, char *argv[]) {

	int height, width;

	// Welcome message
	cout << "Automatic Plate Number Recognition" << endl;

	// Process input args
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <input image> " << endl;
	}

	// Open the input image
	Mat input = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	// namedWindow("Input", CV_WINDOW_AUTOSIZE);
	namedWindow("Output", CV_WINDOW_AUTOSIZE);
	// imshow("Input", input);
	// waitKey();

	height = input.size().height;
	width = input.size().width;

	// ------------------------------------------------------------------
	// FIRST STAGE:
	// 1. Extract candidate bands
	// 2. Extract candidate plate regions from each band

	// Create CarPicture object
	CarPicture carpic(input);

	// Extract plates
	carpic.setProjectionSmoothFactor(height * 0.04);
	carpic.setBandClipFactor(0.5);
	vector<Band> *bands = carpic.getProbableBands(2);
	vector<Plate> candidatePlates;
	for (unsigned int i = 0; i < bands->size(); i++) {
		Band &b = bands->at(i);
		b.setProjectionSmoothFactor(width * 0.1);
		b.setPlateClipFactor(0.6);
		vector<Plate> *ps = b.getProbablePlateRegion(2);
		for (unsigned int j = 0; j < ps->size(); j++) {
			candidatePlates.push_back(ps->at(j));
		}
		// delete &b;
	}
	// delete bands;

	// Show cadidate plate regions
	Mat output = Mat(input.size(), CV_8UC3);
	cvtColor(input, output, CV_GRAY2RGB);
	for (unsigned int p = 0; p < candidatePlates.size(); p++) {
		Plate &plt = candidatePlates.at(p);
		rectangle(output, Point(plt.getLeft() + 1, plt.getTop() + 1), Point(
				plt.getRight() - 1, plt.getBottom() - 1), CV_RGB(0,255,0), 2);
		imshow("Output", output);
	}

	/*
	 // ------------------------------------------------------------------
	 // SECOND STAGE:
	 // Repeat the first stage on each candidate plate region
	 // detected in the first stage, hance refining them
	 vector<Plate> plates;
	 for (unsigned int k = 0; k < candidatePlates.size(); k++) {
	 Plate &p = candidatePlates.at(k);
	 CarPicture probablePlateRegion(p.getData());
	 probablePlateRegion.setProjectionSmoothFactor(height/25);
	 probablePlateRegion.setBandClipFactor(0.4);
	 bands = probablePlateRegion.getProbableBands(2);
	 for (unsigned i = 0; i < bands->size(); i++) {
	 Band &b = bands->at(i);
	 b.setProjectionSmoothFactor(width/6);
	 b.setPlateClipFactor(0.5);
	 vector<Plate> *ps = b.getProbablePlateRegion(2);
	 for (unsigned j = 0; j < ps->size(); j++) {
	 Plate plate(ps->at(j));
	 Plate tmp(plate.getData(), plate.getLeft() + p.getLeft(),
	 plate.getTop() + p.getTop(), plate.getRight()
	 + p.getLeft(), plate.getBottom() + p.getTop());
	 plates.push_back(tmp);
	 }
	 }
	 // delete &b;
	 // delete bands;
	 // delete &p;
	 }

	 // Show plates
	 for (unsigned int p = 0; p < plates.size(); p++) {
	 Plate &plt = plates.at(p);
	 rectangle(output, Point(plt.getLeft() + 1, plt.getTop() + 1), Point(
	 plt.getRight() - 1, plt.getBottom() - 1), CV_RGB(255,0,0));
	 imshow("Input", output);
	 }
	 */

	// End :)
	waitKey();
	return 0;
}
