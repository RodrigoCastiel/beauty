#include "RayTracer.h"

namespace renderer
{

void RayTracer::Render(const Scene & scene, ImgBuffer && imgBuffer)
{
    // TODO(Rodrigo Castiel).
    Camera& camera = *(scene.GetCamera());

    for (int y = 0; y < imgBuffer.Height(); y++)
    {
        for (int x = 0; x < imgBuffer.Width(); x++)
        {
            //printf("(x, y) = (%d, %d)\n", x, y);
            uchar* pixel = imgBuffer.GetPixel(x, y);
            glm::vec3 O = camera.GetCenterCoordinates();
            glm::vec3 r = camera.CastRay((float)x, (float)y, (
                float)imgBuffer.Width(), (float)imgBuffer.Height());
            glm::vec3 rgb = scene.TraceRay(r, O, mConfiguration.GetRecursionDepth());

            pixel[channel::R] = static_cast<uchar>(std::min(rgb[0], 1.0f) * 255);
            pixel[channel::G] = static_cast<uchar>(std::min(rgb[1], 1.0f) * 255);
            pixel[channel::B] = static_cast<uchar>(std::min(rgb[2], 1.0f) * 255);
            pixel[channel::A] = 255;
        }
    }
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