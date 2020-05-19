#include <iostream>
#include <string>

#include "math.h"
#include "image.h"

using namespace std;

int main() {
    const string outputFile = "out.tga";
    const int width = 256;
    const int height = 256;
    Image img(width, height);

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            float3 color((float)x / width, (float)y / height, 0.5);
            img.SetPixel(x, y, color);
        }
    }

    //stbi_write_png(outputFile.c_str(), width, height, bytesPerPixel, img, width * bytesPerPixel);
    img.WriteTGA(outputFile);
    std::cout << "wrote " << outputFile << std::endl;
     
    
}