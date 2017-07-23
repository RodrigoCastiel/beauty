#include "RenderingConfig.h"

#include <fstream>

namespace renderer
{

bool RenderingConfig::LoadFromFile(const std::string & filePath)
{
    std::ifstream file(filePath, std::ifstream::in);
    if (!file.is_open())
        return false;

    std::string property;
    int value;

    while (file >> property)
    {
        if (!(file >> value))
            continue;

        if (property == "w")
            mWidth = value;
        else if (property == "h")
            mHeight = value;
        else if (property == "depth")
            mRecursionDepth = (value > 0) ? value : 1;
        else if (property == "anti-aliasing")
            mAntiAliasing = (value != 0);
        else if (property == "num-threads")
            mNumThreads = (value > 0) ? value : 1;
        else if (property == "use-kd-tree")
            mUseKdTree = (value != 0);
    }

    return true;
}

bool RenderingConfig::SaveIntoFile(const std::string & filePath)
{
    std::ofstream file(filePath, std::ofstream::out);
    if (!file.is_open())
        return false;

    file << "w " << mWidth  << std::endl;
    file << "h " << mHeight << std::endl;
    file << "depth " << mRecursionDepth << std::endl;
    file << "anti-aliasing " << mAntiAliasing << std::endl;
    file << "num-threads " << mNumThreads << std::endl;
    file << "use-kd-tree " << mUseKdTree  << std::endl;

    return true;
}


}  // namespace renderer.