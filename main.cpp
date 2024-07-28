#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


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
    cv::calcHist(&bgr[1], 1, 0, cv::Mat(), b_hist, 1, &binnum, &histoRange);
    cv::calcHist(&bgr[2], 1, 0, cv::Mat(), b_hist, 1, &binnum, &histoRange);

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
             cv::Scalar(255, 0 , 0)
        );
        line(histImg, cv::Point(step*(i-1), hei - cvRound(r_hist.at<float>(i-1))),
                      cv::Point(step*i, hei - cvRound(r_hist.at<float>(i))),
             cv::Scalar(255, 0 , 0)
        );
    }

    cv::imshow("Histogram", histImg);
};

void equalizeCallback(int state, void* userData){};
void lomoCallback(int state, void* userData){};
void cartoonCallback(int state, void* userData){};

int main(int argc, const char** argv)
{
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("Phototool v1");
    if (parser.has("help")) 
    {
        parser.printMessage();
        return 0;
    }

    cv::String imgFile = parser.get<cv::String>(0);
    // checking if parameters were parsed correclty
    if (!parser.check())
    {
        parser.printErrors();
        return 0;
    }

    // load image to process
    cv::Mat img = cv::imread(imgFile);
    // create window
    cv::namedWindow("Input");

    // ui buttons
    cv::createButton("Show histogram", showHistoCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    // cv::createButton("Equalize histogram", equalizeCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    // cv::createButton("Lomography effect", lomoCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    // cv::createButton("Cartoonize effect", cartoonCallback, NULL, cv::QT_PUSH_BUTTON, 0);

    // Show image
    cv::imshow("Input", img);
    cv::waitKey(0);

    return 0;
}