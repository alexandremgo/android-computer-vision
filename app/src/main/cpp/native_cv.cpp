/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <media/NdkImage.h>

#include "format_image.h"
#include "process_image.h"

/**
 * - row stride: distance between the start of two consecutive rows of pixels in the image.
 * For planes with uint_8 elements, row stride = width of source image
 * - pixel stride: distance between two consecutive pixel values in a row of pixels.
 * It may be larger than the size of a single pixel to account for interleaved image data or padded formats
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_mgo_computervision_Camera2ProcessFragment_processImage(
        JNIEnv* env,
        jobject /* this */,
        jobject dstSurface,
        jobject srcBuffer_y,
        jobject srcBuffer_u,
        jobject srcBuffer_v,
        jint srcRowStride_y,
        jint srcRowStride_uv,
        jint srcPixelStride_uv,
        jint srcRect_top,
        jint srcRect_bottom,
        jint srcRect_left,
        jint srcRect_right) {

    // Gets pointers to the 3 planes (YUV) of the source.
    // The 3 planes are ByteBuffer containing the frame data. The underlying data is be mapped
    // as a pointer in JNI without doing any copies with GetDirectBufferAddress
    uint8_t *src_y = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(srcBuffer_y));
    if (src_y == NULL) {
        return NULL;
    }

    uint8_t *src_u = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(srcBuffer_u));
    if (src_u == NULL) {
        return NULL;
    }

    uint8_t *src_v = reinterpret_cast<uint8_t *>(env->GetDirectBufferAddress(srcBuffer_v));
    if (src_v == NULL) {
        return NULL;
    }

    // Gets a ptr to the destination window. Will need to release it.
    ANativeWindow *dstWin = ANativeWindow_fromSurface(env, dstSurface);

    // Acquires a reference on the given ANativeWindow object.
    // This prevents the object from being deleted until the reference is removed.
    ANativeWindow_acquire(dstWin);

    // Gets the buffer on which will draw our processed image
    // ANativeWindow_Buffer format is guaranteed to be
    //  - WINDOW_FORMAT_RGBX_8888 = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM
    //  - WINDOW_FORMAT_RGBA_8888 = AHARDWAREBUFFER_FORMAT_R8G8B8X8_UNORM
    ANativeWindow_Buffer dstBuf;
    if(ANativeWindow_lock(dstWin, &dstBuf, NULL) < 0) {
        ANativeWindow_release(dstWin);
        return env->NewStringUTF("Error: impossible to lock the native window");
    }

    assert(dstBuf.format == WINDOW_FORMAT_RGBX_8888 ||
           dstBuf.format == WINDOW_FORMAT_RGBA_8888);

    uint32_t *dst = static_cast<uint32_t *>(dstBuf.bits);

    // Rotation of 90
    int32_t src_height = srcRect_bottom - srcRect_top;
    int32_t src_width = srcRect_right - srcRect_left;
    int32_t height = MIN(dstBuf.width, (src_height));
    int32_t width = MIN(dstBuf.height, (src_width));

    // To fit correctly the source image in the surface
    int32_t ratio_height = src_height/height;
    int32_t ratio_width = src_width/width;

    dst += height - 1;
    for (int32_t y = 0; y < height; y++) {
        const int32_t relative_y = ratio_height * y;
        const uint8_t *pY = src_y + srcRowStride_y * (relative_y + srcRect_top) + srcRect_left;

        int32_t uv_row_start = srcRowStride_uv * ((relative_y + srcRect_top) >> 1);;
        const uint8_t *pU = src_u + uv_row_start + (srcRect_left >> 1);
        const uint8_t *pV = src_v + uv_row_start + (srcRect_left >> 1);

        for (int32_t x = 0; x < width; x++) {
            const int32_t relative_x = ratio_width * x;
            const int32_t uv_offset = (relative_x >> 1) * srcPixelStride_uv;

            // ANativeWindow_Buffer stride: the number of pixels that a line in the buffer
            // takes in memory. x --> x * dstBuf.stride = [x, y]--> [-y, x]
            dst[x * dstBuf.stride] = YUV2RGB(pY[relative_x], pU[uv_offset], pV[uv_offset]);
        }

        // Moves to the next column
        dst -= 1;
    }

    std::string result = "Spec from native: \n | buf h = " + std::to_string(dstBuf.height)
                        + " & buf w = " + std::to_string(dstBuf.width)
                        + " & buf row stride = " + std::to_string(dstBuf.stride)
                        + "\n | final h = " + std::to_string(height)
                        + " & w = " + std::to_string(width)
                        + "\n | Crop Rect b = " + std::to_string(srcRect_bottom)
                        + " t = " + std::to_string(srcRect_top)
                        + " l = " + std::to_string(srcRect_left)
                        + " r = " + std::to_string(srcRect_right)
                        + "\n | row stride y = " + std::to_string(srcRowStride_y)
                        + " & row stride uv = " + std::to_string(srcRowStride_uv)
                        + " & pixel stride uv = " + std::to_string(srcPixelStride_uv);

    uint32_t *processedImg = static_cast<uint32_t *>(dstBuf.bits);

    applyCanny(processedImg, width, height, 30, 3, 3);

    ANativeWindow_unlockAndPost(dstWin);
    ANativeWindow_release(dstWin);

    return env->NewStringUTF(result.c_str());
}
