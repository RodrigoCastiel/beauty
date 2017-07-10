#include "RayTracer.h"

#include <thread>
#include <iostream>

namespace renderer
{

void RayTracer::Render(const Scene & scene, ImgBuffer & imgBuffer) const
{
    if (mConfiguration.GetNumThreads() == 1)
    {
        const Camera& camera = scene.GetCamera();

        for (int y = 0; y < imgBuffer.Height(); y++) {
            for (int x = 0; x < imgBuffer.Width(); x++) {
                uchar* pixel = imgBuffer.GetPixel(x, y);
                glm::vec3 O = camera.GetCenterCoordinates();
                glm::vec3 r = camera.CastRay((float)x, (float)y,
                    (float)imgBuffer.Width(), (float)imgBuffer.Height());
                glm::vec4 rgba = this->TraceRay(r, O, mConfiguration.GetRecursionDepth(), scene);

                pixel[channel::R] = static_cast<uchar>(std::min(rgba[0], 1.0f) * 255);
                pixel[channel::G] = static_cast<uchar>(std::min(rgba[1], 1.0f) * 255);
                pixel[channel::B] = static_cast<uchar>(std::min(rgba[2], 1.0f) * 255);
                pixel[channel::A] = static_cast<uchar>(std::min(rgba[3], 1.0f) * 255);
            }
        }
    }
    else
    {
        this->ParallelRender(scene, imgBuffer, mConfiguration.GetNumThreads());
    }

    if (mConfiguration.AntiAliasingOn())
    {
        this->AdaptiveAntiAliasing(scene, imgBuffer, mConfiguration.GetNumThreads());
    }
}

void RayTracer::ParallelRender(const Scene & scene, ImgBuffer & imgBuffer, int numThreads) const
{
    std::vector<std::thread> threads(numThreads);
    const Camera& camera = scene.GetCamera();

    //mProgressMessage = "Ray Tracing First Stage - Initial computation";

    //RayTracer::ClearProgressBar();
    auto Renderer = [this, &imgBuffer, &scene, &camera, numThreads](int id)
    {
        for (int y = id; y < imgBuffer.Height(); y += numThreads)
        {
            for (int x = 0; x < imgBuffer.Width(); x++)
            {
                uchar* pixel = imgBuffer.GetPixel(x, y);
                glm::vec3 O = camera.GetCenterCoordinates();
                glm::vec3 r = camera.CastRay((float)x, (float)y,
                    (float)imgBuffer.Width(), (float)imgBuffer.Height());
                glm::vec4 rgba = this->TraceRay(r, O, mConfiguration.GetRecursionDepth(), scene);

                pixel[channel::R] = static_cast<uchar>(std::min(rgba[0], 1.0f) * 255);
                pixel[channel::G] = static_cast<uchar>(std::min(rgba[1], 1.0f) * 255);
                pixel[channel::B] = static_cast<uchar>(std::min(rgba[2], 1.0f) * 255);
                pixel[channel::A] = static_cast<uchar>(std::min(rgba[3], 1.0f) * 255);
            }

            //RayTracer::IncrementProgressBar();
            //RayTracer::PrintProgressBar();
        }
    };

    std::chrono::high_resolution_clock::time_point start;
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; i++)
    {
        threads[i] = std::thread(Renderer, i);
    }

    for (int i = 0; i < numThreads; i++)
    {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double delta_t = duration / 1e9;

    std::cout << "Frame rendered. Elapsed time: " << delta_t << " seconds.\n";
}

void RayTracer::AdaptiveAntiAliasing(const Scene & scene, ImgBuffer & imgBuffer,
                                     int numThreads, int numSuperSample) const
{
    std::vector<std::thread> threads(numThreads);
    const Camera& camera = scene.GetCamera();

    //mProgressMessage = "Ray Tracing Anti-aliasing - Adaptive Supersampling";

    //RayTracer::ClearProgressBar();
    auto Renderer = [this, &imgBuffer, &scene, &camera, numThreads, numSuperSample](int id)
    {
        for (int y = id+1; y < imgBuffer.Height()-1; y += numThreads)
        {
            for (int x = 1; x < imgBuffer.Width()-1; x++)
            {
                unsigned char* pixel = imgBuffer.GetPixel(x, y);
                unsigned char* top = imgBuffer.GetPixel(x, y - 1);
                unsigned char* bot = imgBuffer.GetPixel(x, y + 1);
                unsigned char* lft = imgBuffer.GetPixel(x - 1, y);
                unsigned char* rgt = imgBuffer.GetPixel(x + 1, y);

                float dx = (((rgt[0] + rgt[1] + rgt[2]) / 3.0f) - ((lft[0] + lft[1] + lft[2]) / 3.0f)) / 255.0f;
                float dy = (((bot[0] + bot[1] + bot[2]) / 3.0f) - ((top[0] + top[1] + top[2]) / 3.0f)) / 255.0f;
                float gradient = sqrt(dx*dx + dy*dy);

                if (gradient > kGradientThreshold)
                {
                    glm::vec3 O = camera.GetCenterCoordinates();
                    glm::vec4 rgba(.0f);

                    for (int i = 0; i < numSuperSample; i++)
                    {
                        float theta = 2.0f * 3.141592f * static_cast<float>(i) / (numSuperSample);
                        glm::vec3 r = camera.CastRay(x + 0.5f*std::cosf(theta), y + 0.5f*std::sinf(theta), 
                            (float)imgBuffer.Width(), (float)imgBuffer.Height());
                        rgba += this->TraceRay(r, O, mConfiguration.GetRecursionDepth(), scene);
                    }
                    rgba *= 1.0 / (numSuperSample);

                    pixel[channel::R] = static_cast<uchar>(std::min(rgba[0], 1.0f) * 255);
                    pixel[channel::G] = static_cast<uchar>(std::min(rgba[1], 1.0f) * 255);
                    pixel[channel::B] = static_cast<uchar>(std::min(rgba[2], 1.0f) * 255);
                    pixel[channel::A] = static_cast<uchar>(std::min(rgba[3], 1.0f) * 255);
                }
            }
            //RayTracer::IncrementProgressBar();
            //RayTracer::PrintProgressBar();
        }
    };

    std::chrono::high_resolution_clock::time_point start;
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; i++)
    {
        threads[i] = std::thread(Renderer, i);
    }

    for (int i = 0; i < numThreads; i++)
    {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double delta_t = duration / 1000000000.0;

    std::cout << "Adaptive Anti-aliasing finished. Elapsed time: " << delta_t << " seconds.\n";
}

glm::vec4 RayTracer::TraceRay(const glm::vec3 & r, const glm::vec3 & O, 
                              int depth, const Scene & scene) const
{
    if (depth > 0)
    {
        float t_triangle, t_sphere;
        glm::vec3 intersection, barycentric;
        glm::vec3 intersectionSphere, normalSphere;

        int nearestTriangleIndex = scene.NearestTriangle(r, O, intersection, barycentric, t_triangle);
        int nearestSphereIndex = scene.NearestSphere(r, O, intersectionSphere, normalSphere, t_sphere);

        glm::vec3 f_pos, n, Kd, Ks;
        float alpha, n_refr = 1.0f;

        // Triangle only or triangle closer than sphere.
        if (((nearestTriangleIndex != -1) && (nearestSphereIndex == -1))
         || ((nearestTriangleIndex != -1) && (nearestSphereIndex != -1) && t_triangle <= t_sphere))
        {
            const Triangle& triangle = scene.GetTriangle(nearestTriangleIndex);
            const TriangleAttrib& attrib = scene.GetTriangleAttrib(nearestTriangleIndex);

            // Interpolate per-vertex properties.
            f_pos = triangle.v[0] * barycentric[0]
                  + triangle.v[1] * barycentric[1]
                  + triangle.v[2] * barycentric[2];
            n = glm::normalize(attrib.n * barycentric);
            Kd = attrib.Kd * barycentric;
            Ks = attrib.Ks * barycentric;
            alpha = glm::dot(attrib.shininess, barycentric);
            n_refr = attrib.n_refr;
        }
        // Sphere only or sphere closer than triangle.
        else if (((nearestTriangleIndex == -1) && (nearestSphereIndex != -1))
            ||  ((nearestTriangleIndex != -1)  && (nearestSphereIndex != -1) && t_sphere < t_triangle))
        {
            const SphereAttrib& attrib = scene.GetSphereAttrib(nearestSphereIndex);

            f_pos = intersectionSphere;
            n = normalSphere;
            Kd = attrib.Kd;
            Ks = attrib.Ks;
            alpha = attrib.shininess;
            n_refr = attrib.n_refr;
        }
        else // No intersection.
        {
            return scene.GetBackgroundColor();
        }

        glm::vec3 abs_color = scene.ComputePhongIllumination(f_pos, n, Kd, Ks, alpha);

        if (depth == 1)
        {
            return glm::vec4(abs_color * (glm::vec3(1) - Ks), 1.0f);
        }
        else
        {
            /* Calculating Reflection and Refraction */
            // Reference: 
            // http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf

            glm::vec3 refracted_color(0.0f);
            glm::vec3 reflected_color(0.0f);

            // Reflect if specular coefficient is non-zero.
            if (Ks[0] > kEpsilon && Ks[1] > kEpsilon && Ks[2] > kEpsilon)
            {
                glm::vec4 incoming_color = this->TraceRay(glm::normalize(glm::reflect(r, n)), 
                                                          f_pos, depth-1, scene);
                reflected_color = glm::vec3(incoming_color);
            }

            float n_1 = 1.00f;    // Air.
            float n_2 = n_refr;   // Index of refraction of the sphere.
                                  // Reflectance and transmittance.
            float R = 1.0f, T = 0.0f;

            if (std::abs(n_2 - n_1) > kEpsilon)  // Different indices of refraction.
            {
                float cos_ti = -glm::dot(r, n);

                if (cos_ti < 0.0f)  // From out to in.
                {
                    std::swap(n_1, n_2);
                }
                float ratio_n = n_1 / n_2;
                float sin2_tt = ratio_n*ratio_n*(1.0f - cos_ti*cos_ti);
                float cos_tt = (float)sqrt(1.0f - sin2_tt);

                float R0 = std::pow((n_1 - n_2) / (n_1 + n_2), 2.0f);

                // Reflectance.
                float one_minus_cos_ti = (1 - cos_ti);
                //float power = pow(one_minus_cos_ti, 3.0);
                R = R0 + (1.0f - R0)*one_minus_cos_ti;

                // Transmittance.
                T = 1.0f - R;

                glm::vec3 refr_r = glm::normalize(ratio_n*r + (ratio_n*cos_ti - cos_tt) * n);
                glm::vec4 incoming_color = this->TraceRay(refr_r, f_pos, depth-1, scene);
                refracted_color = glm::vec3(incoming_color);
            }

            return  glm::vec4(Ks * (R*reflected_color + T*refracted_color) + (glm::vec3(1.0f) - Ks) * abs_color, 1.0f);
        }
    }
    else
    {
        return scene.GetBackgroundColor();
    }
}

}  // namespace renderer.