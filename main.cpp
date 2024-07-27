#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

const char* keys = 
{
    "{help h usage ? | | print this message}"
    "{@image | | Image to process}"
};

void showHistoCallback(int state, void* userData){};
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
    cv::createButton("Equalize histogram", equalizeCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    cv::createButton("Lomography effect", lomoCallback, NULL, cv::QT_PUSH_BUTTON, 0);
    cv::createButton("Cartoonize effect", cartoonCallback, NULL, cv::QT_PUSH_BUTTON, 0);

    // Show image
    cv::imshow("Input", img);
    cv::waitKey(0);
    return 0;
}