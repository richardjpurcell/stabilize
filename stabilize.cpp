/*
*  stabilize.cpp
*  -read a sequence of images and output a list of offsets
*
*/

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

void help(char** argv)
{
    cout << "\nRead a sequence of images and process.\n"
         << "Assuming original files are stored in ./shots/shot_<shotNumber>/orig_<shotNumber>/\n"
         << "Assuming original files are named orig_<shotNumber>.%04d.png\n"
         << "Operation 1: track and create offset file. <track>\n"
         << "Operation 2: stabilize original and output new sequence. <stabilize>\n"
         << "Operation 3: de-stabilize overlay. <destabilize>\n"
         << "Usage: " 
         << argv[0]
         << " <shot number>"
         << " <shot length>"
         << " <operation name>\n"
         << "example: " 
         << argv[0] 
         << " 2524 226 track\n"
         << "q, Q, esc -- quit\n"
         << endl;
}

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        help(argv);
        return 1;
    }

    string shotNumber = argv[1];
    int shotLength = atoi(argv[2]);
    string opType = argv[3];
    string origPath = "./shots/shot_" + shotNumber + "/orig_" + shotNumber + "/orig_" + shotNumber + ".%04d.png";
    string ofstPath = "./shots/shot_" + shotNumber + "/ofst_" + shotNumber + ".txt";

    VideoCapture sequence(origPath);
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
    //read first frame
    sequence >> image;
    //window
    namedWindow("image | q or esc to quit", WINDOW_NORMAL);
    resizeWindow("image | q or esc to quit", image.cols/2, image.rows/2);
    //user defines initial bounding box
    Rect2d bbox = selectROI("image | q or esc to quit", image, false, false);
    //initialize tracker
    tracker->init(image, bbox);
    //open coordinates file
    ofstream outfile (ofstPath);
    //write coordinates
    outfile << setfill('0') << setw(4) << (int)bbox.x << " ";
    outfile << setfill('0') << setw(4) << (int)bbox.y << " ";
    outfile << setfill('0') << setw(4) << (int)bbox.height << " ";
    outfile << setfill('0') << setw(4) << (int)bbox.width << endl;
    //progress bar
    int barWidth = 70;
    int counter = 1;
    float progress = 0.0;
    int pos = 0;
    float progressIncrement = ((float)barWidth/shotLength);

    for(;;)
    {
        sequence >> image;
        counter += 1;
        if(image.empty())
        { 
            //finish progress bar
            cout << endl;
            cout << "End of Sequence" << endl;
            break;
        }

        //update tracking result
        bool ok = tracker->update(image, bbox);

        if (ok)
        {
            //tracking success, draw the tracked object
            rectangle(image, bbox, Scalar(255, 0, 0), 2, 1);
            //write coordinates
            outfile << setfill('0') << setw(4) << (int)bbox.x << " ";
            outfile << setfill('0') << setw(4) << (int)bbox.y << " ";
            outfile << setfill('0') << setw(4) << (int)bbox.height << " ";
            outfile << setfill('0') << setw(4) << (int)bbox.width << endl;
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

        cout << "[";
        pos = progress;
        //cout << pos << endl;

        for (int i = 0; i < barWidth; ++i)
        {
            if (i < pos) cout << "=";
            else if (i == pos) cout << ">";
            else cout << " ";
        }

        cout << "] " << int((progress * 100)/barWidth) << "% <frame " << counter << ">\r";
        cout.flush();

        progress += progressIncrement;
    }
    //finish outfile
    outfile << '~' << endl;
    outfile.close();

    return 0;
}