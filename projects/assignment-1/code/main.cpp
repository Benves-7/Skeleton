#include <iostream>
#include <fstream>
#include "MathLib.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include <thread>

struct result
{
    void set(int r, int g, int b) { ir = r; ig = g; ib = b; }
    int r() { return ir; }
    int g() { return ig; }
    int b() { return ib; }

    int ir, ig, ib;
};

struct thread_args
{
    thread_args() {}
    thread_args(int jj, int ii, int x, int y, int s, camera* c, hitable* w, Vector4D* co) : j(jj), i(ii), nx(x), ny(y), ns(s), cam(c), world(w), col(co) {}

    void setColor(Vector4D* c) {col = c;}
    void setSpace(camera* c, hitable* w) {cam = c; world = w;}
    void set(int jj, int ii, int x, int y, int s) {j = jj; i = ii; nx = x; ny = y; ns = s; }
    int j, i, nx, ny, ns;
    camera* cam;
    hitable* world;
    Vector4D* col;
    result res;
};

struct checker_args
{
    checker_args(int x, int y, int& j, int& i) : nx(x), ny(y), j(j), i(i) {}
    int& j;
    int& i;
    const int nx;
    const int ny;
};

Vector4D color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if (world->hit(r,0.001, MAXFLOAT, rec))
    {
        ray scattered;
        Vector4D attenuation;
        // number of bounces.
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth +1);
        }
        else
            return Vector4D(0,0,0);
    }
    else
    {
        Vector4D unit_direction = r.direction().Normalize();
        float t = (unit_direction.y() + 1.0) * 0.5;
        return Vector4D(1.0,1.0,1.0)*(1.0 - t) + Vector4D(0.5,0.7,1.0)*t;
    }
}

void* loop(void* arg)
{
    thread_args* args = ((thread_args*) arg);
    for (int s = 0; s < args->ns; ++s)
    {
        ray r = args->cam->get_ray(float(args->i + drand48()) / float(args->nx), float(args->j + drand48()) / float(args->ny));
        Vector4D p = r.point_at_parameter(2.0);
        args->col->addTo(color(r, args->world, 0));
    }
    args->col->divide(float(args->ns));
    args->col->Set(sqrt(args->col->r()), sqrt(args->col->g()), sqrt(args->col->b()), 0);
    args->res.set(int(255.99 * args->col->r()), int(255.99 * args->col->g()), int(255.99 * args->col->b()));
}

void* checker(void* arg) {
    checker_args *args = ((checker_args *) arg);
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();
    while (true)
    {
        std::string input;
        printf("Input: ");
        std::getline(std::cin, input);
        if (input == "-i")
        {
            auto t1 = Time::now();
            fsec fs = t1 - t0;

            float pixelDone = ((args->ny - (args->j + 1)) * args->nx + args->i);
            float pixelTotal = (args->nx * args->ny);
            float pixelLeft = pixelTotal - pixelDone;
            float pixelSpeed = pixelDone / fs.count();
            printf("%.0f pixels done. (%.0f pixels left) \n", pixelDone, pixelLeft);
            printf("%.1f pixels per sec.\n", pixelDone / fs.count());
            printf("%.2f percent total done. ", (pixelDone / pixelTotal) * 100);
            if (fs.count()/60.0 < 1.0)
                printf("%.1f sec(s) elapsed. ", fs.count());
            else if (fs.count()/3600 < 1.0)
                printf("%.0f min(s) %.1f sec(s) elapsed. ", floor(fs.count()/60), fmod(fs.count(), 60));
            else if (fs.count()/86400 < 1.0)
                printf("%.0f hour(s) %.0f min(s) %.1f sec(s) elapsed. ", floor(fs.count()/60/60), floor(fmod(fs.count(), 60*60)), fmod(fs.count(), 60));
            else
                printf("%.0f day(s) %.0f hour(s) %.0f min(s) %.1f sec(s) elapsed. ", floor(fs.count()/60/60/24), floor(fmod(fs.count(), 24*60*60)), floor(fmod(fs.count(), 60*60)), fmod(fs.count(), 60));
            printf("(estimated time : %.1fs left )\n", (pixelLeft / pixelSpeed));
        }
        else if (input == "-q")
            return 0;
        else
            printf("Wrong in input. \n-i for info.\n-q for quit.\n");
    }
}

int main()
{

    // Start number.
    const int nx = 1000;                // size X
    const int ny = 800;                // size y
    const int ns = 1000;                // number of rays per pixel.
    const int numSpheres = 256;         // number of spheres in the world.
    const int THREAD_NUM = 20;          // number of threads running parallel.

    Vector4D lookfrom(2,4,5), lookat(0,0,1.5);
    float dist_to_focus = ((lookfrom-lookat).Length()), aperture = 0.15;

    // file saving.
    std::ofstream outputfile;
    outputfile.open("Output/" + std::to_string(nx) +"x"+ std::to_string(ny) + "x" + std::to_string(ns) + ".p3");
    outputfile << "P3\n" << nx << " " << ny << "\n255\n";

    result* frame_buffer = new result[nx];

    const float maxPos = 50.0f;
    auto randCoord = [maxPos]() {
        return (1 - (drand48() * 2)) * maxPos;
    };
    const float maxSize = 3.0f;
    const float minSize = 0.1f;
    auto randSize = [maxSize, minSize]() {
        return minSize + (drand48() * maxSize);
    };
    // create world to render.
    hitable* list[numSpheres];
    list[0] = new sphere(Vector4D(0,-5000,0), 5000, new lambertian(Vector4D(0.5,0.5,0.5)));
    list[1] = new sphere(Vector4D(0,3,-1), 3, new metal(Vector4D(1.0, 1.0,1.0), 0.0));
    for (int a = 2; a < numSpheres; a++)
    {
        float choose_mat = drand48();
        float size = randSize();
        Vector4D center(randCoord(), size/2.0, randCoord());
        if (choose_mat < 0.7)           // diffuse
        {
            list[a] = new sphere(center, size/2.0, new lambertian(Vector4D(drand48(),drand48(),drand48())));
        }
        else if (choose_mat < 0.9)     // metal
        {
            list[a] = new sphere(center, size/2.0, new metal(Vector4D(drand48(),drand48(),drand48()), drand48()));
        }
        else                            // glass
        {
            list[a] = new sphere(center, size/2.0, new dielectric(1.5));
        }
    }
    hitable* world = new hitable_list(list,numSpheres);

    // create camera.
    camera cam(lookfrom, lookat, Vector4D(0,1,0), 90, float(nx)/float(ny), aperture, dist_to_focus);

    // setting up thread array, color array, and thread_arg array.
    pthread_t t[THREAD_NUM+1];
    Vector4D col[THREAD_NUM];
    thread_args arg[THREAD_NUM];
    for (int l = 0; l < THREAD_NUM; ++l)
    {
        arg[l].setColor(&col[l]);
        arg[l].setSpace(&cam, world);
    }

    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    // runs the main loop.
    int k = 0, rem = THREAD_NUM, pix = 0;
    int i = 0, j = ny-1;
    checker_args args(nx, ny, j, i);
    pthread_create(&t[THREAD_NUM],0,checker,(void*) &args);
    while (j >= 0)
    {
        i = 0;
        while(i < nx)
        {
            // make one thread per pixel. (start thread here)
            // each thread needs to take a refrence to a Vector4D color and store a RGB value, that then can be printed to the output.
            // they also need a x,y,nx,ny,ns,camera, world to do calculations.
            if (nx - i < THREAD_NUM && rem == THREAD_NUM)
                rem = nx - i;
            if (k < THREAD_NUM && k < rem) {
                arg[k].set(j, i, nx, ny, ns);
                if (pthread_create(&t[k], 0, loop, (void *) &arg[k])) {
                    return 1;
                }
                i++;
                k++;
            }
            if (k == THREAD_NUM)
            {
                for (int l = 0; l < THREAD_NUM; ++l)
                {
                    pthread_join(t[l], 0);
                    frame_buffer[pix] = arg[l].res;
                    pix++;
                }
                k = 0;
            }
            else if (k == rem)
            {
                for (int l = 0; l < rem; ++l)
                {
                    pthread_join(t[l], 0);
                    frame_buffer[pix] = arg[l].res;
                    pix++;
                }
                k = 0;
                rem = THREAD_NUM;
            }
        }
        for (int m = 0; m < pix; ++m)
        {
            outputfile << frame_buffer[m].r() << " " << frame_buffer[m].g() << " " << frame_buffer[m].b() << " ";
        }
        outputfile << "\n";
        pix = 0;
        j--;
    }
    pthread_cancel(t[THREAD_NUM]);

    auto t1 = Time::now();
    fsec fs = t1 - t0;
    std::cout << fs.count();

    for (int a = 0; a < numSpheres; a++)
        delete list[a];

    delete world;

    delete[] frame_buffer;

    return 0;
}