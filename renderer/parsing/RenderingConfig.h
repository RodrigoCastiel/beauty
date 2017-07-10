//  +-----------------------------------------------------+
//  |   File: RenderingConfig.h                           |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: June 18, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <string>

namespace renderer
{

class RenderingConfig
{
public:
    bool LoadFromFile(const std::string & filePath);
    bool SaveIntoFile(const std::string & filePath);

    // Getters / Setters.
    int GetImgWidth() const { return mWidth; }
    int GetImgHeight() const { return mHeight; }
    int GetRecursionDepth() const { return mRecursionDepth; }
    bool AntiAliasingOn() const { return mAntiAliasing; }
    int GetNumThreads() const { return mNumThreads; }

    void SetImgWidth(int width) { mWidth = width; }
    void SetImgHeight(int height) { mHeight = height; }
    void SetRecursionDepth(int depth) { mRecursionDepth = depth; }
    void SetAntiAliasingOn(bool on) { mAntiAliasing = on; }
    void SetNumThreads(int numThreads) { mNumThreads = numThreads; }

private:
    // Image dimensions.
    int mWidth   { 800 };
    int mHeight  { 600 };

    // Multi-threading.
    int mNumThreads { 1 };

    // Recursion.
    int mRecursionDepth { 4 };
    
    // Image enhancement.
    bool mAntiAliasing { false };
};

}  // namespace renderer.