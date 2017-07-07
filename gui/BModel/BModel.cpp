#include "BModel.h"

using renderer::RenderingConfig;

BModel::BModel()
{

}

BModel::~BModel()
{

}

void BModel::Preview()
{

}

void BModel::Render()
{

}

void BModel::SetRenderingConfig(const RenderingConfig & config)
{
    mRayTracer.SetRenderingConfig(config);

    // TODO(Rodrigo Castiel): update QImage size.
}

const RenderingConfig & BModel::GetRenderingConfig() const
{
    return mRayTracer.GetRenderingConfig();
}