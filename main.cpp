#include <iostream>
#include <string>

#include "math.h"
#include "image.h"
#include "ray.h"
#include "rayhit.h"
#include "geometry.h"
#include "camera.h"
#include "spatial.h"

using namespace std;

float3 ColorFromNormal(const float3 &N) {
    return 0.5*float3(N.x()+1, N.y()+1, N.z()+1);
}

float3 background(const Ray& ray) {
    float3 direction = normalized(ray.direction());
    float yinterp = (direction.y() + 1.0)/2.0;
    return lerp(float3(1.0, 1.0, 1.0), float3(0.5, 0.7, 1.0), yinterp);
}

float3 TraceRay(const Ray& ray, const Hittable &scene, int depth) {
    if(depth <= 0) {
        return float3(0,0,0);
    }
    RayHit h;
    if(scene.Intersects(ray, 0.001, 10000, h)) {
        Ray bounce;
        float3 atten;
        if(h.material->Scatter(ray, h, atten, bounce)) {
            return atten * TraceRay(bounce, scene, depth - 1);
        } else {
            return float3(0,0,0);
        }
    } 

    return background(ray);   
}

using lambertian = LambertMat;
using metal = MetalMat;
using dielectric = DielectricMat;
using sphere = Sphere;

int main() {
    const string outputFile = "out.tga";
    const int width = 640;
    const int height = 360;
    Image img(width, height);

point3 lookfrom(13,2,3);
point3 lookat(0,0,0);
vec3 vup(0,1,0);
auto dist_to_focus = 10.0;
auto aperture = 0.0;

    Camera camera(120, (float)width / height, lookfrom,  lookat - lookfrom, vup, aperture, dist_to_focus);
    HittableList scene;
int samplesPerPixel = 50;
int maxBounces = 10;

    /*
    scene.Add(make_shared<Sphere>(float3(0,0,-1), 0.5, make_shared<LambertMat>(float3(0.7, 0.3, 0.3))));
    scene.Add(make_shared<Sphere>(float3(0,-100.5,-1), 100,make_shared<LambertMat>(float3(0.8, 0.8, 0.0))));
    scene.Add(make_shared<Sphere>(float3(1,0,-1), 0.5, make_shared<MetalMat>(float3(.8,.6,.2), 0.3)));
    scene.Add(make_shared<Sphere>(float3(-1,0,-1), 0.5, make_shared<MetalMat>(float3(.8,.8,.8), 1.0)));
*/


    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    scene.Add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            auto choose_mat = randomf();
            point3 center(a + 0.9*randomf(), 0.2, b + 0.9*randomf());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::Random() * color::Random();
                    sphere_material = make_shared<lambertian>(albedo);
                    scene.Add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::Random(0.5, 1);
                    auto fuzz = randomf(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    scene.Add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    scene.Add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }


    auto material1 = make_shared<dielectric>(1.5);
    scene.Add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    scene.Add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    scene.Add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    
BVHNode bvh(scene);
   std::clock_t start;
    double duration;

    start = std::clock();

    for(int y = 0; y < height; y++) {
        std::cerr << "\rScanlines remaining: " << (height - y) << ' ' << std::flush;
        for(int x = 0; x < width; x++) {

            float3 color(0,0,0);
            for(int sample = 0; sample < samplesPerPixel; sample++) {
                float u = (float)(x + randomf()) / (width-1);
                float v = (float)(height - (y + randomf()) - 1) / (height-1);
                Ray r = camera.GetRay(u, v);
                color += TraceRay(r, bvh, maxBounces);
            }
            color = color/samplesPerPixel;
            //gamma correct
            color = float3(sqrt(color.x()), sqrt(color.y()), sqrt(color.z()));
            img.SetPixel(x, y, color);
        }
    }
    std::cerr << "\nDone.\n";
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cerr <<"Duration " << duration <<'\n';
    

    //stbi_write_png(outputFile.c_str(), width, height, bytesPerPixel, img, width * bytesPerPixel);
    img.WriteTGA(outputFile);
    std::cout << "wrote " << outputFile << std::endl;
     
    
}