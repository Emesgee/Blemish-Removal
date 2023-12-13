#include <opencv2/opencv.hpp>

using namespace cv;

// UserData to hold image and mask data
struct UserData {
    Mat image; 
    Mat mask; 
};

// Mouse callback function
void onMouse(int event, int x, int y, int flags, void* userdata) {
    UserData* data = static_cast<UserData*>(userdata);

    if (event == EVENT_LBUTTONDOWN) {
        // Define the radius of the circular mask
        int radius = 10;

        // Create a circular mask for the clicked point
        Mat mask = Mat::zeros(data->image.size(), CV_8U);
        circle(mask, Point(x, y), radius, Scalar(255), FILLED);

        // Inpaint the blemish
        Mat inpainted;
        inpaint(data->image, mask, inpainted, 3, INPAINT_TELEA);

        // Update the original image with inpainted region
        inpainted.copyTo(data->image, mask);

        // Display the updated image
        imshow("Original Image", data->image);
    }
}

int main() {
    // Read the input image
    Mat image = imread("blemish.png");

    if (image.empty()) {
        std::cerr << "Error: Could not read the image." << std::endl;
        return -1;
    }

    // Create a window
    namedWindow("Original Image");

    // Display the original image
    imshow("Original Image", image);

    // Create UserData structure
    UserData userdata{ image, Mat() };

    // Set the initial mouse callback
    setMouseCallback("Original Image", onMouse, &userdata);

    // Wait for the user to press any key to exit
    waitKey(0);

    // Save the inpainted image
    imwrite("outputImage.jpg", userdata.image);

    return 0;
}
