/*
*  argTest.cpp
*  -test commandline interface
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
         << " <operation name>\n"
         << "example: " 
         << argv[0] 
         << " 2524 track\n"
         << "q, Q, esc -- quit\n"
         << endl;
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        help(argv);
        return 1;
    }

    string arg1 = argv[1];
    string arg2 = argv[2];
    string origPath = "./shots/orig_" + arg1 + "/orig_" + argv[1] + ".0001.png";

    cout << origPath << endl;
    cout << arg2 << endl;

    return 0;
}