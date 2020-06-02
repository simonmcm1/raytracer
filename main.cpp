#include <iostream>
#include <string>

#include "math.h"
#include "image.h"
#include "ray.h"
#include "rayhit.h"
#include "geometry.h"
#include "camera.h"
#include "spatial.h"
#include "raytracer.h"

using namespace std;

using lambertian = LambertMat;
using metal = MetalMat;
using dielectric = DielectricMat;
using sphere = Sphere;

void Render(RayTracer &rt, int x0, int x1, int y0, int y1) {
    std::cerr << "Rendering " << y0 << " to " << y1 << endl;
    rt.Render(x0, x1, y0, y1);
    std::cerr << "Finished " << y0 << " to " << y1 << endl;
}

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
Random rand;
    Camera camera(120, (float)width / height, lookfrom,  lookat - lookfrom, vup, aperture, dist_to_focus);
    HittableList scene;
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
            auto choose_mat = rand.randomf();
            point3 center(a + 0.9*rand.randomf(), 0.2, b + 0.9*rand.randomf());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = rand.randomVector() * rand.randomVector();
                    sphere_material = make_shared<lambertian>(albedo);
                    scene.Add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = rand.randomVector(0.5, 1);
                    auto fuzz = rand.randomf(0, 0.5);
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
    RayTracer rt(width, height, camera, img, bvh);
    
    auto start = std::chrono::high_resolution_clock::now();
    int batches = 4;
    int linesPerBatch = height / batches;
    int y0 = 0;
    vector<std::thread> threads;
    while(y0 < height) { 
        int y1 = y0 + linesPerBatch - 1;
        if(y1 >= height) {
            y1 = height - 1;
        }   
        std::thread thread(Render, ref(rt), 0, width, y0, y1);
        threads.push_back(move(thread));
        y0 = y1 + 1;    
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
    }

    for(auto &t : threads){
        t.join();
    }

    std::cerr << "\nDone.\n";
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cerr <<"Duration " << elapsed.count() << endl;
    

    //stbi_write_png(outputFile.c_str(), width, height, bytesPerPixel, img, width * bytesPerPixel);
    img.WriteTGA(outputFile);
    std::cout << "wrote " << outputFile << std::endl;
     
    
}