#include <QCoreApplication>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;



///Variables

Mat src, src_hsv, src_gray, dst, cdst, detected_edges, hist, imagenes[14];
MatND histogramas[14];
string nomImg, nombres[14];
int opcion = 0, i=0;
int ratio = 3;
int kernel_size = 3;
//char* window_name = "Edge Map";

/**
 * @function CannyThreshold
 */

void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, 40, 40*ratio, kernel_size );

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);
  src.copyTo( dst, detected_edges);
  //imshow( window_name, dst);
}


/**
 * @function main
 */

int main( int argc, char *argv[] )
{

QCoreApplication a (argc, argv);

    while(opcion!=3)
    {
        cout << "\n\n\t Choose an option: \n\n\n 1. Store a tetrabrick \n 2. Consult saved images \n 3. Exit\n\n   " << endl;
        cin >> opcion ;
        switch(opcion)
        {
            ///Introduce image's name
            case 1:
            {
            /// Load image
            cout << "Select an image: ";
            cin >> nomImg;
            src = imread( nomImg, CV_LOAD_IMAGE_COLOR );

                if( !src.data )
                {
                cout << "Image not found" << endl;
                return -1;
                }

            ///Convert to gray for Canny detector
            cvtColor( src, src_gray, CV_BGR2GRAY );

            /// Create a window
            //namedWindow( window_name, CV_WINDOW_AUTOSIZE );

            /// Call the function
            CannyThreshold(0, 0);

            waitKey();

            /// Convert the result to hsv
            cvtColor( dst, src_hsv, COLOR_BGR2HSV );

            /// Sizes for hsv
            int h_bins = 30, s_bins = 30, v_bins = 30;
            int histSize[] = { h_bins, s_bins, v_bins };

            /// hue varies from 0 to 180, saturation from 0 to 255
            float h_ranges[] = { 0, 180 };
            float s_ranges[] = { 0, 255 };
            float v_ranges[] = { 50, 255 };

            const float* ranges[] = { h_ranges, s_ranges, v_ranges };

            /// Use the 3 channels
            int channels[] = { 0, 1, 2 };

            /// Calculate the histograms for the HSV images
            calcHist( &src_hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false );
            normalize( hist, hist, 1, 0,  NORM_L1, -1, Mat() );

            /// Compare distance with tetrabricks from the database
                for(int k=0; k<15; k++)
                {
                    if(histogramas[k].empty())
                    {
                        cout << "Tetrabrick not stored. Enter name: ";
                        cin >> nomImg;
                        nombres[i]=nomImg;
                        imagenes[i]= src.clone();
                        histogramas[i] = hist.clone();
                        i++;
                        break;
                    }
                double bhattacharyyaHist = compareHist(hist, histogramas[k], CV_COMP_BHATTACHARYYA );

                cout << "The distance with "; cout << nombres[k]; cout << " is: "; cout <<  bhattacharyyaHist << endl;
                    if( bhattacharyyaHist<0.09)
                    {
                        cout << "Stored Tetrabrick." << endl;
                        break;
                    }
                }
            break;
            }

            ///Consult saved images
            case 2:
            {
            cout <<  "The names of the tetrabricks stored are: " << endl;
                int w=0;
                while(!histogramas[w].empty())
                {
                    cout << nombres[w] << endl;
                    w++;
                }
            cout << "The amount of tetrabricks stored is:  " << w <<endl;
            break;
            }

            ///Exit
            case 3:
            {
            return 0;
            }
        }
    }
}
