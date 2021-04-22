
/*
 * File:      boundBoxTest.cpp
 * Author:    Richard Purcell
 * Date:      2021-04-20
 * Version:   1.0
 * Purpose:   Test bounding box functions
 * Usage:     $ ./boundBoxTest
 * Notes:     none
 */
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

int main(int argc, char **arv)
{
    Mat im = imread("./video/IMG_2524/IMG_2524_0001.png");

    Rect2d bbox = selectROI("Image", im, false, false);

    //open coordinates file
    ofstream outfile ("coordinates.txt");
    //write coordinates
    outfile << setfill('0') << setw(4) << bbox.x << " ";
    outfile << setfill('0') << setw(4) << bbox.y << " ";
    outfile << setfill('0') << setw(4) << bbox.height << " ";
    outfile << setfill('0') << setw(4) << bbox.width << endl;
    outfile << "End" << endl;

    rectangle(im, bbox, Scalar(255, 0, 0), 2, 1);

    imshow("Image", im);

    waitKey(0);

    outfile.close();

    return 0;
}