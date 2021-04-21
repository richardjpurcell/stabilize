/*
*  stabilize.cpp
*  -read a sequence of images
*
*/

#include <opencv2/opencv.hpp>

#include <iostream>

using namespace cv;
using namespace std;

void help(char** argv)
{
    cout << "\nRead a sequence of images.\n"
         << "Usage: " 
         << argv[0]
         << " <path to first image in sequence\n"
         << "example: " 
         << argv[0] 
         << " right%%02d.jpg\n"
         << "q, Q, esc -- quit\n"
         << endl;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        help(argv);
        return 1;
    }

    string arg = argv[1];
    VideoCapture sequence(arg);

    if (!sequence.isOpened())
    {
        cerr << "Failed to open Image Sequence.\n" << endl;
        return 1;
    }

    Mat image;
    namedWindow("Image | q or esc to quit", WINDOW_NORMAL);

    for(;;)
    {
        sequence >> image;
        if(image.empty())
        {
            cout << "End of Sequence" << endl;
            break;
        }

        imshow("image | q or esc to quit", image);

        char key = (char)waitKey(500);
        if(key == 'q' || key == 'Q' || key == 27)
            break;
    }

    return 0;
}