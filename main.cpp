#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

/* global var: to be processed withing the effect fucntion
    consider passing by refernce and omit global scope */
cv::Mat img;
const char* keys = 
{
    "{help h usage ? | | print this message}"
    "{@image | | Image to process}"
};

void showHistoCallback(int state, void* userData)
{
    // separate image in BRG
    std::vector<cv::Mat> bgr;
    cv::split(img, bgr);
    int binnum = 256;
    
    // set range for BGR
    float range[] = {0, 256};
    const float* histoRange = {range};

    cv::Mat b_hist;
    cv::Mat g_hist;
    cv::Mat r_hist;

    cv::calcHist(&bgr[0], 1, 0, cv::Mat(), b_hist, 1, &binnum, &histoRange);
    cv::calcHist(&bgr[1], 1, 0, cv::Mat(), g_hist, 1, &binnum, &histoRange);
    cv::calcHist(&bgr[2], 1, 0, cv::Mat(), r_hist, 1, &binnum, &histoRange);

    // draw histo
    int wid = 512;
    int hei = 300;
    cv::Mat histImg(hei, wid, CV_8UC3, cv::Scalar(20, 20, 20)); // gray base image

    // normalize histo to image height
    cv::normalize(b_hist, b_hist, 0, hei, cv::NORM_MINMAX);
    cv::normalize(g_hist, g_hist, 0, hei, cv::NORM_MINMAX);
    cv::normalize(r_hist, r_hist, 0, hei, cv::NORM_MINMAX); 
    
    // draw line b/w each bin
    int step = cvRound((float)wid / (float)binnum);
    for (int i=1; i<binnum; i++)
    {
        line(histImg, cv::Point(step*(i-1), hei - cvRound(b_hist.at<float>(i-1))),
                      cv::Point(step*i, hei - cvRound(b_hist.at<float>(i))),
             cv::Scalar(255, 0 , 0)
        );
        line(histImg, cv::Point(step*(i-1), hei - cvRound(g_hist.at<float>(i-1))),
                      cv::Point(step*i, hei - cvRound(g_hist.at<float>(i))),
             cv::Scalar(0, 255 , 0)
        );
        line(histImg, cv::Point(step*(i-1), hei - cvRound(r_hist.at<float>(i-1))),
                      cv::Point(step*i, hei - cvRound(r_hist.at<float>(i))),
             cv::Scalar(0, 0 , 255)
        );
    }

    cv::imshow("Histogram", histImg);
};

void equalizeCallback(int state, void* userData)
{
    cv::Mat result;
    cv::Mat ycrcb;
    cv::cvtColor(img, ycrcb, cv::COLOR_BGR2YCrCb);

    // split YCbCr image into diff channels matrix
    std::vector<cv::Mat> channels;
    cv::split(ycrcb, channels);
    cv::equalizeHist(channels[0], channels[0]); // equalize in y only

    cv::merge(channels, ycrcb);
    cv::cvtColor(ycrcb, result, cv::COLOR_YCrCb2BGR);

    cv::imshow("Equalized", result); 
};

void lomoCallback(int state, void* userData)
{
    cv::Mat result;

    // Look up table:
    const double exp = std::exp(1.0);
    cv::Mat lut(1, 256, CV_8UC1);
    for (int i=0; i<256; i++)
    {
        float x = (float) i / 256.0;
        lut.at<uchar>(i) = cvRound(256 * (1 / (1 + pow(exp, -((x-0.5) / 0.1 )))));
    }

    // apply curve transfer only to red channel
    std::vector<cv::Mat> bgr;
    cv::split(img, bgr);
    cv::LUT(bgr[2], lut, bgr[2]);
    cv::merge(bgr, result);

    // halo with blur to smooth
    cv::Mat halo(img.rows, img.cols, CV_32FC3, cv::Scalar(0.3, 0.3, 0.3));
    cv::circle(halo, cv::Point(img.cols/2, img.rows/2), img.cols/3, cv::Scalar(1, 1, 1), -1);
    cv::blur(halo, halo, cv::Size(img.cols/3 ,img.cols/3));

    // for matrix multiplication
    cv::Mat resultf;
    result.convertTo(resultf, CV_32FC3);
    cv::multiply(resultf, halo, resultf);
    resultf.convertTo(result, CV_8UC3);

    cv::imshow("Lomograpy", result);
    halo.release();
    resultf.release();
    lut.release();
    bgr[0].release();
    bgr[1].release();
    bgr[2].release();
};

void cartoonCallback(int state, void* userData)
{
    // remove some noise
    cv::Mat imgMedian;
    cv::medianBlur(img, imgMedian, 7); // Gaussian blur
    // edge detection
    cv::Mat imgCanny;
    cv::Canny(imgMedian, imgCanny, 50, 150);
    // dilate edges
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(imgCanny, imgCanny, kernel);

    // rescaleing for pixel multiplication
    imgCanny = imgCanny/255;
    imgCanny = 1 - imgCanny;

    cv::Mat imgCannyf;
    imgCanny.convertTo(imgCannyf, CV_32FC3);
    blur(imgCannyf, imgCannyf, cv::Size(5, 5));

    // Homogonize color
    cv::Mat imgBF;
    cv::bilateralFilter(img, imgBF, 9, 150.0, 150.0);

    cv::Mat result = imgBF/25;
    result = result * 25; // truncates possible color values

    // merge color and edge results
    cv::Mat imgCanny3c;
    cv::Mat cannyChannels[] = {imgCannyf, imgCannyf, imgCannyf};
    cv::merge(cannyChannels, 3, imgCanny3c);

    cv::Mat resultf;
    result.convertTo(resultf, CV_32FC3);
    cv::multiply(resultf, imgCanny3c, resultf);
    resultf.convertTo(result, CV_8UC3);

    cv::imshow("Result", result);
};

int main(int argc, const char** argv)
{
    // std::cout << cv::getBuildInformation();
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("Phototool v1");
    if (parser.has("help")) 
    {
        parser.printMessage();
        return 0;
    }

    cv::String imgFile = parser.get<cv::String>(0);
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    img = cv::imread(imgFile);
    cv::namedWindow("Input");

    cv::createButton("Show histogram", showHistoCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    cv::createButton("Equalize histogram", equalizeCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    cv::createButton("Lomography effect", lomoCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    cv::createButton("Cartoonize effect", cartoonCallback, NULL, cv::QT_PUSH_BUTTON, 0);

    cv::imshow("Input", img);
    cv::waitKey(0);

    return 0;
}