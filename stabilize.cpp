/*
*  stabilize.cpp
*  -read a sequence of images
*
*/

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

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
    string trackerTypes[4] = {"CSRT", "MEDIANFLOW", "MOSSE", "KCF"};
    //create tracker
    string trackerType = trackerTypes[0];
    Ptr<Tracker> tracker;

    if (trackerType == "CSRT")
        tracker = TrackerCSRT::create();
    else if (trackerType == "MEDIANFLOW")
        tracker = TrackerMedianFlow::create();
    else if (trackerType == "MOSSE")
        tracker = TrackerMOSSE::create();
    else if (trackerType == "KCF")
        tracker = TrackerKCF::create();
    else
    {
        cout << "Invalid tracker specified." << endl;
        cout << "Available trackers are: " << endl;
        for (int i = 0; i < sizeof(trackerTypes)/sizeof(trackerTypes[0]); i++)
            cout << i << " : " << trackerTypes[i] << endl;
        return -1;
    }

    if (!sequence.isOpened())
    {
        cerr << "Failed to open Image Sequence.\n" << endl;
        return 1;
    }

    Mat image;
    //namedWindow("Image | q or esc to quit", WINDOW_NORMAL);

    //read first frame
    sequence >> image;
     //define initial bounding box
    Rect2d bbox(204, 131, 97, 222);
    //display bounding box
    rectangle(image, bbox, Scalar(255, 0, 0), 2, 1);
    //imshow("Image | q or esc to quit", image);
    //initialize tracker
    tracker->init(image, bbox);

    for(;;)
    {
        sequence >> image;
        if(image.empty())
        {
            cout << "End of Sequence" << endl;
            break;
        }

        //update tracking result
        bool ok = tracker->update(image, bbox);

        if (ok)
        {
            //tracking success, draw the tracked object
            rectangle(image, bbox, Scalar(255, 0, 0), 2, 1);
        }
        else
        {
            //tracking failure detected
            putText(image, "Tracking failure", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255), 2);
        }

        imshow("image | q or esc to quit", image);

        char key = (char)waitKey(500);
        if(key == 'q' || key == 'Q' || key == 27)
            break;
    }

    return 0;
}