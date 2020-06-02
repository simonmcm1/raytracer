#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <iostream>
#include <string>
#include <thread>

#include "random.h"
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

float3 TraceRay(const Ray& ray, Random &rand, const Hittable &scene, int depth) {
    if(depth <= 0) {
        return float3(0,0,0);
    }
    RayHit h;
    if(scene.Intersects(ray, 0.001, 10000, h)) {
        Ray bounce;
        float3 atten;
        if(h.material->Scatter(ray, rand, h, atten, bounce)) {
            return atten * TraceRay(bounce, rand, scene, depth - 1);
        } else {
            return float3(0,0,0);
        }
    } 

    return background(ray);   
}

class RayTracer {
    public:

    RayTracer(int w, int h, const Camera &cam, Image &img, BVHNode &bvh) : width(w), height(h), camera(cam), image(img), scene(bvh) {
        samples = 10;
        maxBounces = 5;
    };

    void Render(int x0, int x1, int y0, int y1) {

        for(int y = y0; y <= y1; y++) {
            for(int x = x0; x <= x1; x++) {
                float3 color(0,0,0);
                for(int sample = 0; sample < samples; sample++) {
                    float u = (float)(x + rand.randomf()) / (width-1);
                    float v = (float)(height - (y + rand.randomf()) - 1) / (height-1);
                    Ray r = camera.GetRay(rand, u, v);
                    color += TraceRay(r, rand, scene, maxBounces);
                }
                color = color/samples;
                //gamma correct
                color = float3(sqrt(color.x()), sqrt(color.y()), sqrt(color.z()));
                image.SetPixel(x, y, color);
            }
        }
    }

    private:
        int width;
        int height;
        int samples;
        int maxBounces;
        const Camera &camera;
        Image &image;
        BVHNode &scene;
        Random rand;
        
};

#endif