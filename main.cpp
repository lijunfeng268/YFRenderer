#include "Window.h"
#include "Framebuffer.h"
#include <iostream>
#include <cmath>

// 绘制一些测试像素
Color white(255, 255, 255, 255);
Color red(255, 0, 0, 255);
Color green(0, 255, 0, 255);
Color blue(0, 0, 255, 255);

void line(int ax, int ay, int bx, int by, Framebuffer& framebuffer, Color color = white) {
    for (float t=0.; t<1.; t+=.02) {
        int x = std::round( ax + (bx-ax)*t );
        int y = std::round( ay + (by-ay)*t );
        framebuffer.setPixel(x, y, color);
    }
}

Framebuffer test(int width, int height)
{
    // 创建窗口和framebuffer
    Framebuffer framebuffer(width, height);

    // 清空framebuffer为黑色
    framebuffer.clear(Color(0, 0, 0, 255));

    // 在不同位置绘制彩色像素点
    for (int i = 0; i < 100; i++) {
        framebuffer.setPixel(100 + i, 100, white);
        framebuffer.setPixel(100 + i, 101, white);
    }

    // 绘制一个简单的三角形轮廓
    for (int i = 0; i < 200; i++) {
        framebuffer.setPixel(320 + i, 240 + i / 2, red);
        framebuffer.setPixel(320 - i, 240 + i / 2, green);
        framebuffer.setPixel(220 + i, 340, blue);
    }
    return framebuffer;
}

int main(int argc, char** argv) {
    
    constexpr int width  = 64;
    constexpr int height = 64;
    Window window("Software Renderer", width, height);
    //Framebuffer framebuffer = test(width, height);

    Framebuffer framebuffer(width, height);
    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;
    line(ax, ay, bx, by, framebuffer, red);

    std::cout << "Window created: " << width << "x" << height << std::endl;
    std::cout << "Press ESC or close window to exit" << std::endl;

    // 主循环
    while (!window.shouldClose()) {
        // 处理窗口事件
        window.pollEvents();

        // 绘制framebuffer到窗口
        window.drawFramebuffer(framebuffer);

        // 简单的延时，避免CPU占用过高
        Sleep(16);  // 约60 FPS
    }

    std::cout << "Window closed" << std::endl;
    return 0;
}
