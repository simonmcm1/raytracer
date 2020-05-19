#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

#include <iostream>
#include <vector>

using std::sqrt;

class Image {
    public:
        Image(int w, int h) : width(w), height(h) {
            img.reserve(width * height * BYTES_PER_PIXEL);
        }
        void SetPixel(int x, int y, float3 color) {
            int index = (x + y * width) * BYTES_PER_PIXEL;
            img[index] = static_cast<uint8_t>(color.x() * 255.0);
            img[index + 1] = static_cast<uint8_t>(color.y() * 255.0);
            img[index + 2] = static_cast<uint8_t>(color.z() * 255.0);
        }

        void WriteTGA(const std::string &path) {
            stbi_write_tga(path.c_str(), width, height, BYTES_PER_PIXEL, img.data());
        }
    private:
        int width;
        int height;
        const int BYTES_PER_PIXEL = 3;
        std::vector<uint8_t> img;
};

#endif