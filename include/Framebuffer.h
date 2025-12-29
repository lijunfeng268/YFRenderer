#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>

struct Color {
    unsigned char r, g, b, a;
    
    Color() : r(0), g(0), b(0), a(255) {}
    Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255)
        : r(r_), g(g_), b(b_), a(a_) {}
};

class Framebuffer {
public:
    Framebuffer(int width, int height);
    
    void clear(const Color& color);
    void setPixel(int x, int y, const Color& color);
    Color getPixel(int x, int y) const;
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    const unsigned char* getData() const { return reinterpret_cast<const unsigned char*>(data_.data()); }

private:
    int width_;
    int height_;
    std::vector<Color> data_;
};

#endif // FRAMEBUFFER_H
