/*
 * Copyright 2020 Alexandre Magaud
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "process_image.h"

bool applyCanny(uint32_t *img, int width, int height,
        int lowThreshold, int ratio, int kernel_size) {

    cv::Mat RGBImg = cv::Mat(width, height, CV_8UC4, img);
    cv::Mat grayImg = cv::Mat(width, height, CV_8UC1);
    cv::cvtColor(RGBImg, grayImg, cv::COLOR_RGBA2GRAY);

    cv::blur(grayImg, grayImg, cv::Size(3,3));

    // grayImg is also the output of the Canny Filter. It will contain non-zero values where
    // edges have been detected.
    // Hysteresis: Canny uses two thresholds (upper and lower):
    // - If a pixel gradient is higher than the upper threshold, the pixel is accepted as an edge
    // - If a pixel gradient value is below the lower threshold, then it is rejected.
    // - If the pixel gradient is between the two thresholds, then it will be accepted
    // only if it is connected to a pixel that is above the upper threshold.
    cv::Canny(grayImg, grayImg, lowThreshold, lowThreshold*ratio, kernel_size);

    // Creates a black img on which only the (colored) edges will be copied
    cv::Mat edges = cv::Mat(width, height, CV_8UC4, cv::Scalar(0, 0, 0, 0));
    // Copies the (colored) edges in the locations where grayImg has non-zero values
    RGBImg.copyTo(edges, grayImg);
    // Copies the result to the source img
    edges.copyTo(RGBImg);

    return true;
}

