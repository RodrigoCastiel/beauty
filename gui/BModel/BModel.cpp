#include "BModel.h"

#include <iostream>

using renderer::RenderingConfig;

BModel::BModel()
{
    int w = mRayTracer.GetRenderingConfig().GetImgWidth();
    int h = mRayTracer.GetRenderingConfig().GetImgHeight();
    mImgBuffer.resize(w * h * renderer::k_ARGB32_Size, 255);
    mImage = QImage(mImgBuffer.data(), w, h, QImage::Format_ARGB32);
}

BModel::~BModel()
{
}

void BModel::Preview()
{
    // TODO.
}

void BModel::Render()
{
    // Load and parse scene file.
    renderer::Scene scene;
    std::string outputMessage;
    bool success = scene.Load(mSceneFilePath, outputMessage);

    if (!success)
    {
        throw RenderingException(outputMessage.data());
    }

    // Render using current configurations.
    scene.Log(std::cout);
    int w = mRayTracer.GetRenderingConfig().GetImgWidth();
    int h = mRayTracer.GetRenderingConfig().GetImgHeight();
    renderer::ImgBuffer imgBuffer = {mImgBuffer.data(), w, h};
    mRayTracer.Render(scene, imgBuffer);
}

void BModel::SetRenderingConfig(const RenderingConfig & config)
{
    int last_w = mRayTracer.GetRenderingConfig().GetImgWidth();
    int last_h = mRayTracer.GetRenderingConfig().GetImgHeight();

    int new_w = config.GetImgWidth();
    int new_h = config.GetImgHeight();

    mRayTracer.SetRenderingConfig(config);

    // Reallocate buffer size if resolution was changed.
    if ((new_w != last_w) || (new_h != last_h))
    {
        mImgBuffer.resize(new_w * new_h * renderer::k_ARGB32_Size, 255);
        mImage = QImage(mImgBuffer.data(), new_w, new_h, QImage::Format_ARGB32);
    }
}

const RenderingConfig & BModel::GetRenderingConfig() const
{
    return mRayTracer.GetRenderingConfig();
}