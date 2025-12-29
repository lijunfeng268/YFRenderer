#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

class Framebuffer;

class Window {
public:
    Window(const char* title, int width, int height);
    ~Window();

    bool shouldClose() const { return should_close_; }
    void drawFramebuffer(const Framebuffer& framebuffer);
    void pollEvents();
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    HWND hwnd_;
    HDC memory_dc_;
    unsigned char* surface_buffer_;
    int width_;
    int height_;
    bool should_close_;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    void createSurface();
};

#endif // WINDOW_H
