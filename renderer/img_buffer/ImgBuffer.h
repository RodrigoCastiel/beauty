//  +-----------------------------------------------------+
//  |   File: ImgBuffer.h                                 |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 08, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

// -----------------------------------------------------------------------
// ImgBuffer is a light wrapper for pixel reading/writing in image buffer.
// -----------------------------------------------------------------------

#include <algorithm>

namespace renderer
{

using uchar = unsigned char;
const int k_ARGB32_Size = 4;

namespace channel  // Follows ARGB32 format.
{
    const int A = 3;
    const int R = 2;
    const int G = 1;
    const int B = 0;
}  // namespace comp.

class ImgBuffer
{
public:
    // Simplest constructor.
    ImgBuffer(uchar* buffer, int w, int h)
        : mBuffer(buffer)
        , mWidth(w)
        , mHeight(h)
    { }

    // Returns the address of pixel at (x, y).
    // Does not check boundaries.
    inline uchar* GetPixel(int x, int y) 
    {  
        return &mBuffer[k_ARGB32_Size*(mWidth*y + x)];
    }

    // Returns a reference to a specific channel of pixel at (x, y).
    // Use channel::R, channel::G, channel::B or channel::A.
    // Does not check boundaries.
    inline uchar & GetPixelChannel(int x, int y, int chann = channel::R)
    {
        return mBuffer[k_ARGB32_Size*(mWidth*y + x) + chann];
    }

    // Clears the entire buffer with desired value.
    void Clear(uchar value)
    {
        // TODO: review this code!
        std::fill(mBuffer, mBuffer+mWidth*mHeight*k_ARGB32_Size, value);
    }

    // Returns image buffer width.
    int Width() const { return mWidth; }

    // Returns image buffer height.
    int Height() const { return mHeight; }

private:
    int mWidth;
    int mHeight;
    uchar* mBuffer;
};

}  // namespace renderer.