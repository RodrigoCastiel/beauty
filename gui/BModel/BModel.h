//  +-----------------------------------------------------+
//  |   File: BModel.h                                    |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: June 18, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <QImage>
#include <vector>
#include <exception>

#include <scene/Scene.h>
#include <ray_tracer/RayTracer.h>
#include <parsing/RenderingConfig.h>

// Simple exception class for all possible rendering errors.
class RenderingException : public std::exception
{
public:
    RenderingException(const char* cause)
    : mCause(cause) {}

    const char * what() const throw() { return mCause; }

private:
    const char * mCause;
};

class BModel
{
public:
    // === Contructors/Destructor ===
    BModel();
    ~BModel();

    // === Main Methods ===

    // Implementing GUI preview/render methods.
    void Preview();
    void Render();

    // === Setters / Getters ===

    // Returns current image.
    const QImage & GetImage() const { return mImage; }

    // Sets/Returns rendering configuration for internal renderers.
    void SetRenderingConfig(const renderer::RenderingConfig & config);
    const renderer::RenderingConfig & GetRenderingConfig() const;

private:
    // Renderers:
    renderer::RayTracer mRayTracer;

    // Buffer:
    QImage mImage;
    std::vector<renderer::uchar> mImgBuffer;  // row-major/ARGB format.
};