#include "RayTracer.h"

namespace renderer
{

void RayTracer::Render(const Scene & scene, ImgBuffer && imgBuffer)
{
    // TODO(Rodrigo Castiel).
    // sample code!
    //imgBuffer.Clear(0);
    for (int x = 0; x < imgBuffer.Width(); x++) {
        for (int y = 0; y < imgBuffer.Height(); y++) {
            if (true || x*x + y*y < 70 * 70) {
                uchar* pixel = imgBuffer.GetPixel(x, y);
                pixel[channel::R] = 255;
                pixel[channel::G] = 0;
                pixel[channel::B] = 0;
                pixel[channel::A] = 255;
            }
        }
    }
    // ----
}

void RayTracer::ParallelRender(const Scene & scene, ImgBuffer && imgBuffer, int numThreads)
{
    // TODO(Rodrigo Castiel).
}

void RayTracer::AdaptiveAntiAliasing(const Scene & scene, ImgBuffer && imgBuffer, 
                                     int numThreads, int numSuperSample)
{
    // TODO(Rodrigo Castiel).
}

}  // namespace renderer.