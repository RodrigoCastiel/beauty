#include "BModel.h"

using renderer::RenderingConfig;

BModel::BModel()
{

}

BModel::~BModel()
{

}

void BModel::SetRenderingConfig(const RenderingConfig & config)
{
	mRenderingConfig = config;
}

const RenderingConfig & BModel::GetRenderingConfig() const
{
	return mRenderingConfig;
}