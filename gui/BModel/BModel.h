//  +-----------------------------------------------------+
//  |   File: BModel.h                                    |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: June 18, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <QImage>

#include <scene/Scene.h>
#include <ray_tracer/RayTracer.h>
#include <parsing/RenderingConfig.h>

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

    // Sets/Returns rendering configuration for internal renderers.
    void SetRenderingConfig(const renderer::RenderingConfig & config);
    const renderer::RenderingConfig & GetRenderingConfig() const;

private:
    // Renderers:
    renderer::RayTracer mRayTracer;

    // Buffer:
    QImage mImgBuffer;
};