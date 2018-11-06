#include <QCoreApplication>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

//-- Variables

int opcion;
int ratio=3;
int kernel_size=3;

Mat src;
string nomImg;

//-- Functions

Mat CannyThreshold(Mat src);
Mat Histogram(Mat dst);
void Storage(Mat hist);

int main()
{

    while(opcion!=3)
    {
        cout << "\n\n\t Choose an option: \n\n\n 1. Store a tetrabrick \n 2. Consult saved images \n"
                " 3. Delete storage file \n 4. Exit\n\n   " << endl;
        cin >> opcion ;
        switch(opcion)
        {
            ///Store a tetrabrick
            case 1:
            {
                cout << "Select an image: ";
                cin >> nomImg;
                src = imread( nomImg, CV_LOAD_IMAGE_COLOR );
                if( !src.data )
                    {
                    cout << "Image not found" << endl;
                    return -1;
                    }

                //-- Call the Canny function
                Mat dst=CannyThreshold(src);
                //-- Call the Histogram function
                Mat hist=Histogram(dst);
                //-- Call the Storage function
                Storage(hist);
                break;

            }

            ///Consult saved data
            case 2:
            {
            int tamano=0;
            ifstream inputfile("data.txt");

            //-- If there isn't previous data stored
            if(!inputfile){
                cout << "There isn't any data stored" << endl;
                break;
            }

            //-- If there is previous data stored
            string str;
            string nombres[15];
            Mat histogramas[15];
            string minNombre[2];
            double minDistance=1;
            double total=0;
            double media=0;
            int k=0;
            double varianza=0;
            while(getline(inputfile, str)){
                nombres[tamano] += str;
                tamano++;
            }
            cout << "There are " << tamano << " models in the DataBase" << endl;

            if(tamano<2){
                cout << "There aren't enough tetra-bricks stored to compare distances" << endl;
                break;
            }

            //-- Take the minimum distance
            string filename = "Data.xml";
            FileStorage fs(filename, FileStorage::READ);
            for (int i=0;i<15;i++){
            fs[nombres[i]] >> histogramas[i];
            }
            //-- Compare each histogram

            for(int i=0; i<15; i++){

                if(histogramas[i].empty()){
                    break;
                }
                for(int j=0; j<15; j++){
                    if(histogramas[j].empty()){
                        break;
                    }
                    double bhattacharyyaHist = compareHist(histogramas[i], histogramas[j], CV_COMP_BHATTACHARYYA );
                    if(minDistance > bhattacharyyaHist && bhattacharyyaHist >= 0.09){
                        minDistance = bhattacharyyaHist;
                        minNombre[0] = nombres[i];
                        minNombre[1] = nombres[j];
                    }
                    media = media + bhattacharyyaHist;
                    total = total + (bhattacharyyaHist * bhattacharyyaHist);
                    k++;
                }
            }
            media = (media / k) * (media / k);
            total = total / k;
            varianza = total - media;
            cout << "The minimum distance is " << minDistance << " between " << minNombre[0] << " and " << minNombre[1] << endl;
            cout << "The variance of distances is: " << varianza << endl;
            break;
            }

            ///Delete storage file
            case 3:
            {
            if( remove( "data.txt" ) != 0 )
            perror( "Error deleting file" );
              else
            puts( "data.txt File successfully deleted" );
            if( remove( "Data.xml" ) != 0 )
            perror( "Error deleting file" );
              else
            puts( "Data.xml File successfully deleted" );
              break;
            }

            ///Exit
            case 4:
            {
            return 0;
            }
        }
    }
}

/**
 * @function CannyThreshold
 */

Mat CannyThreshold(Mat src){
    Mat src_gray, detected_edges, dst;
    //-- Convert to gray for Canny detector
    cvtColor( src, src_gray, CV_BGR2GRAY );
    //-- Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );

    //-- Canny detector
    Canny( detected_edges, detected_edges, 40, 40*ratio, kernel_size );

    //-- Create a matrix of the same type and size as src (for dst)
    dst.create( src.size(), src.type() );

    //-- Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( "Canny", dst);
    waitKey(0);
    return dst;
}

/**
 * @function Histogram
 */

Mat Histogram(Mat dst){
    Mat src_hsv, hist;
    //-- Convert the result to hsv
    cvtColor( dst, src_hsv, COLOR_BGR2HSV );

    //-- Sizes for hsv
    int h_bins = 30, s_bins = 30, v_bins = 30;
    int histSize[] = { h_bins, s_bins, v_bins };

    //-- hue varies from 0 to 180, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 255 };
    float v_ranges[] = { 50, 255 };

    const float* ranges[] = { h_ranges, s_ranges, v_ranges };

    //-- Use the 3 channels
    int channels[] = { 0, 1, 2 };

    //-- Calculate the histograms for the HSV images
    calcHist( &src_hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false );
    normalize( hist, hist, 1, 0,  NORM_L1, -1, Mat() );
    return hist;
}

/**
 * @function Storage
 */

void Storage(Mat hist){
    int k=0;
    Mat histogramas[15];
    string nombres[15];
    string nomImg, nombre;

    string filename = "Data.xml";

    ifstream inputfile("data.txt");

    //-- If there isn't previous data stored, save current data
    if(!inputfile){
        cout << "There isn't any data stored. Enter name: ";
        cin >> nomImg;
        nombre=nomImg;

        //-- Save data
        FileStorage fs(filename, FileStorage::WRITE);
        fs << nombre << hist;
        fs.release();

        //-- Save names
        ofstream outputfile("data.txt");
        outputfile << nombre << endl;

        return;
    }

    //-- If there is previous data stored, compare it with the current data
    string str;
    while(getline(inputfile, str)){
        nombres[k] += str;
        k++;
    }

    FileStorage fs(filename, FileStorage::READ);
    for (int i=0;i<15;i++){

        if (nombres[i].empty()){
            cout << "Tetrabrick not stored. Enter name: ";
            cin >> nomImg;
            //-- The new name musn't be repeated
            for(int j=0; j<15; j++){
                if(nombres[j].empty()){
                    break;
                }
                if(nombres[j] == nomImg){
                    cout << "The name chosen is already used." << endl;
                    return;
                }
            }
            nombres[k]=nomImg;
            histogramas[k]=hist;
            k++;
            break;
        }
    fs[nombres[i]] >> histogramas[i];

    double bhattacharyyaHist = compareHist(hist, histogramas[i], CV_COMP_BHATTACHARYYA );

    if( bhattacharyyaHist<0.09){
        cout << "Stored Tetrabrick with the name: " << nombres[i] << endl;
        return;
        }
    }

    //-- Save data
    fs.open(filename, FileStorage::WRITE);
    for (int i=0; i<15; i++){
        if (nombres[i].empty()){
            break;
        }
        fs << nombres[i] << histogramas[i];
    }
    fs.release();

    //-- Save names
    ofstream outputfile("data.txt");
    for (int i=0; i<15;i++){
        if (nombres[i].empty()){
            break;
        }
            outputfile << nombres[i] << endl;
    }

    return;
}





