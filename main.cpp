#include <iostream>
#include <string>

#include "math.h"
#include "image.h"
#include "ray.h"
#include "geometry.h"

using namespace std;

float3 background(const Ray& ray) {
    float3 direction = normalized(ray.direction());
    float yinterp = (direction.y() + 1.0)/2.0;
    return lerp(float3(1.0, 1.0, 1.0), float3(0.5, 0.7, 1.0), yinterp);
}

int main() {
    const string outputFile = "out.tga";
    const int width = 1280;
    const int height = 720;
    Image img(width, height);

    auto aspect_ratio = (float)width / height;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = float3(0, 0, 0);
    auto horizontal = float3(viewport_width, 0, 0);
    auto vertical = float3(0, viewport_height, 0);
    auto bottomLeft = origin - horizontal/2 - vertical/2 - float3(0, 0, focal_length);

    HittableList scene;
    scene.Add(make_shared<Sphere>(float3(0,0,-1), 0.5));
    scene.Add(make_shared<Sphere>(float3(0,-100.5,-1), 100));

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            float u = (float)x / (width-1);
            float v = (float)(height - y - 1) / (height-1);
            Ray r(origin, bottomLeft + u*horizontal + v*vertical - origin);

            float3 color = background(r);
            RayHit h;
            if(scene.Intersects(r, 0, 10000, h)) {
                float3 N = h.frontFace ? h.normal : -h.normal;
                color = 0.5*float3(N.x()+1, N.y()+1, N.z()+1);
            }            
            
            img.SetPixel(x, y, color);
        }
    }
    

    //stbi_write_png(outputFile.c_str(), width, height, bytesPerPixel, img, width * bytesPerPixel);
    img.WriteTGA(outputFile);
    std::cout << "wrote " << outputFile << std::endl;
     
    
}