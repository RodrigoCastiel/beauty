//  +-----------------------------------------------------+
//  |   File: RayTracer.h                                 |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 06, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include "../scene/Scene.h"
#include "../img_buffer/ImgBuffer.h"
#include "../parsing/RenderingConfig.h"

namespace renderer
{

class RayTracer
{
public:
    // +++++ Main Methods +++++ -----------------------------------------------

    // Performs Ray Tracing algorithm and outputs to input image buffer.
    // imgBuffer dimensions must be at least w x h (in rendering config).
    // imgBuffer must follow the 32-bit format '0xAARRGGBB'.
    void Render(const Scene & scene, ImgBuffer && imgBuffer);

    // Performs a parallel Ray Tracing algorithm using 'numThreads'.
    void ParallelRender(const Scene & scene, ImgBuffer && imgBuffer, 
                        int numThreads = 8);

    // Performs an adaptive anti-aliasing Ray Tracing on input buffer to
    // enhance details. The image must be rendered prior to this call.
    void AdaptiveAntiAliasing(const Scene & scene, ImgBuffer && imgBuffer,
                              int numThreads = 8, int numSuperSample = 8);

    // +++++ Getters / Setters +++++ ------------------------------------------

    // Sets current rendering settings.
    void SetRenderingConfig(const RenderingConfig & config) { mConfiguration = config; }

    // Returns a non-constant reference to rendering settings.
    RenderingConfig & GetRenderingConfig() { return mConfiguration; }
    const RenderingConfig & GetRenderingConfig() const { return mConfiguration; }

private:  // Methods.

private:  // Attributes.

    // Rendering settings, such as resolution, anti-aliasing...
    RenderingConfig mConfiguration;
};

}  // namespace renderer.