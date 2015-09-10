//
//  main.cpp
//  Stat-Face_Rec
//
//  Created by Avinash Bhaskaran on 4/8/15.
//  Copyright (c) 2015 Avinash Bhaskaran. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <dirent.h>
#include <glob.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace cv;
using namespace std;
// Global variables
// Copy this file from opencv/data/haarscascades to target folder
string face_cascade_name = "/Users/avinashbhaskaran/Downloads/opencv-2.4.9/data/haarcascades/haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
int filenumber; // Number of file to be saved
string filename;
Mat cropped_image;
typedef struct pdmtype  {
    short pdm_patch[196][8];
} pdm_patch;
std::vector <pdm_patch> pdm;
string root_path = "/users/avinashbhaskaran/Desktop/Stat-Face_Rec/build/Debug/Database";

void Create_PDM( )  {
    pdm_patch patch;
    std::cout<<cropped_image.rows<<" "<<cropped_image.cols<<std::endl;
//    int count = 0;
//    short patch[196][8];
    for ( int i = 0; i< 128; i+=16) {
        for ( int j = 0; j<128; j+=16 )  {
            int patch_id = 0;
            for ( int pixely = 1; pixely< 15; pixely++) {
                for (int pixelx = 1; pixelx <15; pixelx++)   {
                    patch.pdm_patch[patch_id][0] = cropped_image.ptr(i + pixely-1)[j + pixelx -1] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch.pdm_patch[patch_id][1] = cropped_image.ptr(i + pixely-1)[j + pixelx] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch.pdm_patch[patch_id][2] = cropped_image.ptr(i + pixely-1)[j + pixelx +1] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch.pdm_patch[patch_id][3] = cropped_image.ptr(i + pixely)[j + pixelx-1] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch.pdm_patch[patch_id][4] = cropped_image.ptr(i + pixely)[j + pixelx +1] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch.pdm_patch[patch_id][5] = cropped_image.ptr(i + pixely+1)[j + pixelx -1] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch.pdm_patch[patch_id][6] = cropped_image.ptr(i + pixely+1)[j + pixelx] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch.pdm_patch[patch_id][7] = cropped_image.ptr(i + pixely+1)[j + pixelx +1] - cropped_image.ptr(i + pixely)[j + pixelx];
                    patch_id++;
                }
            }
            pdm.push_back(patch);
        }
    }
    std::cout<<pdm.size()<<" is size of pdm" << std::endl;
}

void Get_ClassMeanPDM() {
    DIR* dir = opendir(root_path.c_str());
    struct dirent *ent = readdir(dir);
    if ( ent == NULL)  {
        cout<<"I don't think I've seen you before. Please enter your name"<<std::endl;
        string dirname;
        cin>>dirname;
        dirname = root_path + "/" + dirname;
#if defined(_WIN32)
        mkdir(dirname.c_str());
#else
        mkdir(dirname.c_str(),0777);
#endif
        string filename = dirname + "/" + "0.png";
        imwrite(filename, cropped_image);
        
    }
    while (true)    {
        
    }

}

void Get_GlobalPDM()    {
    
}




//                    for ( int ii = 0; ii<3; ii++)    {
//                        for ( int jj = 0; jj<3;jj++)    {
//                            if ( jj!= 1 && ii!=1 )  {
//                                patch[patch_id][count] = cropped_image.ptr(i+)
//                            }
//                        }
//                    }




// Function detectAndDisplay
void detectAndDisplay(Mat frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;
    Mat crop;
    Mat res;
    Mat gray;
    string text;
    std::stringstream sstm;
    
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    
    // Detect faces
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
    
    // Set Region of Interest
    cv::Rect roi_b;
    cv::Rect roi_c;
    
    size_t ic = 0; // ic is index of current element
    int ac = 0; // ac is area of current element
    
    size_t ib = 0; // ib is index of biggest element
    int ab = 0; // ab is area of biggest element
    
    for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)
        
    {
        roi_c.x = faces[ic].x;
        roi_c.y = faces[ic].y;
        roi_c.width = (faces[ic].width);
        roi_c.height = (faces[ic].height);
        
        ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)
        
        roi_b.x = faces[ib].x;
        roi_b.y = faces[ib].y;
        roi_b.width = (faces[ib].width);
        roi_b.height = (faces[ib].height);
        
        ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element
        
        if (ac > ab)
        {
            ib = ic;
            roi_b.x = faces[ib].x;
            roi_b.y = faces[ib].y;
            roi_b.width = (faces[ib].width);
            roi_b.height = (faces[ib].height);
        }
        
        crop = frame(roi_b);
        resize(crop, res, Size(128, 128), 0, 0, INTER_LINEAR); // This will be needed later while saving images
        cvtColor(res, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale
        
        // Form a filename
        filename = "";
        stringstream ssfn;
        ssfn << filenumber << ".png";
        filename = ssfn.str();
        filenumber++;
        
        imwrite(filename, gray);
        cropped_image = gray.clone();
        
        Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
        Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
    }
    
    // Show image
    std::cout<<roi_b.x<<" "<<roi_b.y<<" "<<roi_b.height<<" "<<roi_b.width<<std::endl;
    sstm << "Crop area size: " << roi_b.width << "x" << roi_b.height << " Filename: " << filename;
    text = sstm.str();
    
    putText(frame, text, cvPoint(30, 30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 255), 1, CV_AA);
    while (true)    {
        namedWindow("original",0);
        imshow("original", frame);
        if (waitKey(1)>=0)
            break;
    }
//    std::cout<<crop.empty();
    while (true)    {
        cv::namedWindow("detected",1);
        if (!crop.empty())
        {
            imshow("detected", crop);
        }
        else
            destroyWindow("detected");
        if (waitKey(1)>=0)
            break;
    }
}

int main(int argc, const char * argv[])
{
    cv::Mat Image;
    cv::VideoCapture capture(0); // open default camera
    if ( capture.isOpened() == false )
        return -1;
    
    //    cv::namedWindow("Test OpenCV 2.4.9",1);
    cv::Mat frame;
    //    cv::namedWindow("Image clicked",0);
    char confirm;
    while ( true )
    {
        capture >> frame;
        cv::namedWindow("Test OpenCV 2.4.9",1);
        cv::imshow("Test OpenCV 2.4.9", frame );
        int key = cv::waitKey(1);
        if ( key >=0 ) {
            Image = frame;
            cv::namedWindow("Image clicked",1);
            cv::imshow("Image clicked", Image);
            std::cout<<" Confirm Image : y/n"<<std::endl;
            std::cin>>confirm;
            if (confirm == 'y') {
                break;
            }
            else    {
                cv::destroyWindow("Image clicked");
                //                key = cv::waitKey(1);
            }
        }
    }
    while (true)    {
        cv::namedWindow("Image clicked",0);
        cv::imshow("Image clicked", Image);
        //    std::cout<<Image.size();
        if (cv::waitKey(1)>=0)  break;
    }
    if (!face_cascade.load(face_cascade_name))
    {
        printf("--(!)Error loading\n");
        return (-1);
    };
    
    // Read the image file
    frame = Image;
    // Apply the classifier to the frame
    if (!frame.empty())
    {
        detectAndDisplay(frame);
        
    }
    else
    {
        printf(" --(!) No captured frame -- Break!");
        //        break;
    }
    
    //    int c = waitKey(10);
    
    //    if (27 == char(c))
    //    {
    //        break;
    //    }
    
    
    
    //-- Show what you got
    namedWindow("Gray Image",0);
    imshow( "Gray Image", frame );
    Create_PDM();
    DIR* dir = opendir(root_path.c_str());
    struct dirent *ent = readdir(dir);
    if ( ent == NULL)  {
        cout<<"I don't think I've seen you before. Please enter your name"<<std::endl;
        string dirname;
        cin>>dirname;
        dirname = root_path + "/" + dirname;
#if defined(_WIN32)
        mkdir(dirname.c_str());
#else
        mkdir(dirname.c_str(),0777);
#endif
        string filename = dirname + "/" + "0.png";
        imwrite(filename, cropped_image);

    }
    
    
    
    return 0;
}
