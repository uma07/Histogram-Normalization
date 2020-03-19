
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;




void image_histogram(Mat image, int histogram[])
{

    	//Initialize all intensity values to 0
    	for(int i = 0; i < 256; i++)
            	histogram[i] = 0;

    	//Calculate the no of pixels for each intensity values
    	for(int y = 0; y < image.rows; y++)
        	for(int x = 0; x < image.cols; x++)
            		histogram[(int)image.at<uchar>(y,x)]++;

}



void histDisplay(int histogram[], const char* name)
{

    	int hist[256];
    
	for(int i = 0; i < 256; i++)
        	hist[i]=histogram[i];

    	//Draw the histograms
    	int hist_w = 512; int hist_h = 400;
    	int bin_w = cvRound((double) hist_w/256);

    	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255));

    	//Find the maximum intensity element from the histogram
    	int max = hist[0];
    	for(int i = 1; i < 256; i++)
        	if(max < hist[i])
	            	max = hist[i];

    	//Normalize the histogram between 0 and histImage.rows
    	for(int i = 0; i < 256; i++)
        	hist[i] = ((double)hist[i]/max) * histImage.rows;

    	//Draw the intensity line for histogram
	//void line(Mat &img, Point pt1, Point pt2, const Scalar &color, int thickness=1, int lineType=8, int shift=0)
    	for(int i = 0; i < 256; i++)
            	line(histImage, Point(bin_w*(i), hist_h), Point(bin_w*(i), hist_h - hist[i]), Scalar(0), 1, 8, 0);

    	//Display histogram
    	namedWindow(name, CV_WINDOW_AUTOSIZE);
    	imshow(name, histImage);

}





int main(int argc, char *argv[])
{


    	Mat in = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    	//Generate the histogram
    	int histogram[256];
    	image_histogram(in, histogram);

    	//Caluculate the size of image
    	int size = in.rows * in.cols;

    	//Calculate the probability of each intensity
    	float prob[256], nk, A_nk[256];

    	for(int i = 0; i < 256; i++)
        	prob[i] = (double)histogram[i] / size;		//pr(rk) = nk/size,  k = 0, ---, 255

	for(int i = 0; i < 256; i++)
	{
		nk = 0.0;

		for(int j = 0; j < i; j++)
			nk += prob[j];

		A_nk[i] = nk;
	}
	
	for(int i = 0; i < 256; i++)
		A_nk[i] *= 255;			//sk = T(rk) = 255*[pr(r0)+pr[r1]+ --- +pr[rk]],  k = 0, ---, 255

    	//Generate the equlized image
    	Mat out(in.rows, in.cols, CV_8UC1);

    	for(int i = 0; i < in.rows; i++)
        	for(int j = 0; j < in.cols; j++)
		{
            		out.at<uchar>(i, j) = A_nk[in.at<uchar>(i, j)];
			//cout << "A_nk[" << i << "][" << j << "] = " << out.at<uchar>(i, j) << endl;
		}

   	namedWindow("Original Image");
    	imshow("Original Image", in);

    	histDisplay(histogram, "Original Histogram");

    	namedWindow("Equilized Image");
    	imshow("Equilized Image",out);

    	int hist[256];
    	image_histogram(out, hist);
    	histDisplay(hist, "Equilized Histogram");

    	waitKey();

    	return 0;


}

