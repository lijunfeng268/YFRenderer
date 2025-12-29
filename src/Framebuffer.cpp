#include "Framebuffer.h"
#include <algorithm>

Framebuffer::Framebuffer(int width, int height)
    : width_(width)
    , height_(height)
    , data_(width * height, Color(0, 0, 0, 255))
{
}

void Framebuffer::clear(const Color& color) {
    std::fill(data_.begin(), data_.end(), color);
}

void Framebuffer::setPixel(int x, int y, const Color& color) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        data_[y * width_ + x] = color;
    }
}

Color Framebuffer::getPixel(int x, int y) const {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        return data_[y * width_ + x];
    }
    return Color();
}
