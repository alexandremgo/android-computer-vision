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

#ifndef COMPUTERVISION_PROCESS_IMAGE_H
#define COMPUTERVISION_PROCESS_IMAGE_H

#include <cstdint>
#include "opencv2/imgproc.hpp"

/**
 * Applies the Canny Edge Detector on a RGB image.
 *
 * @param img
 * @param width
 * @param height
 * @param lowThreshold: used by Canny filter for hysteresis
 * @param ratio: ration between low and high threshold used by Canny filter
 * It is recommended to use a upper:lower ratio between 2:1 and 3:1
 * @param kernel_size: size of the kernel used by Canny filter
 * @return
 */
bool applyCanny(uint32_t *img, int width, int height, int lowThreshold, int ratio, int kernel_size);

#endif //COMPUTERVISION_PROCESS_IMAGE_H
