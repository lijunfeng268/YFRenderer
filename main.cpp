#include "Window.h"
#include "Framebuffer.h"
#include <iostream>
#include <cmath>
#include<string>
#include<ctime>
#include"include/model.h"
#include<tuple>

// 绘制一些测试像素
Color white(255, 255, 255, 255);
Color red(255, 0, 0, 255);
Color green(0, 255, 0, 255);
Color blue(0, 0, 255, 255);
Color yellow  = {  0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, Framebuffer& framebuffer, Color color = white) {
    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax>bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.setPixel(y, x, color);
        else
            framebuffer.setPixel(x, y, color);
        ierror += 2 * std::abs(by-ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
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

void draw(std::string windowName,Framebuffer& framebuffer,int width, int height)
{
    Window window(windowName.c_str(), width, height);
    // 主循环
    while (!window.shouldClose()) {
        // 处理窗口事件
        window.pollEvents();

        // 绘制framebuffer到窗口
        window.drawFramebuffer(framebuffer);

        // 简单的延时，避免CPU占用过高
        Sleep(16);  // 约60 FPS
    }
}

void lineTest()
{
    constexpr int width  = 640;
    constexpr int height = 640;
    std::string windowName = "Software Renderer";
    //Window window("Software Renderer", width, height);
    //Framebuffer framebuffer = test(width, height);

    Framebuffer framebuffer(width, height);
    //framebuffer.clear(white);
    int ax =  70, ay =  30;
    int bx = 120, by = 370;
    int cx = 620, cy = 530;
    // line(ax, ay, bx, by, framebuffer, red);
    // line(cx, cy, bx, by, framebuffer, green);
    // line(cx, cy, ax, ay, framebuffer, yellow);
    //line(ax, ay, cx, cy, framebuffer, red);
    //line(0, 0, 100, 100, framebuffer, red);
    
    std::srand(std::time({}));
    for (int i=0; i<(1<<10); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        line(ax, ay, bx, by, framebuffer,
            { static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255) });
    }

    std::cout << "Window created: " << width << "x" << height << std::endl;
    std::cout << "Press ESC or close window to exit" << std::endl;

    draw(windowName,framebuffer,width,height);

    std::cout << "Window closed" << std::endl;
}

std::tuple<int,int> project(Vector3 v,int width, int height) { // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
    return { width-(v.x + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
             height-(v.y + 1.) * height/2 }; // we want to shift the vector (x,y) and then scale it to span the entire screen.
}

void objTest()
{
    constexpr int width  = 640;
    constexpr int height = 640;
    Framebuffer framebuffer(width, height);
    std::string modelFile = "D:/github/YFRender/asset/diablo3_pose.obj";
    Model model(modelFile);
    for (int i=0; i<model.nfaces(); i++) { // iterate through all triangles
        auto [ax, ay] = project(model.Position[model.Faces[i].vtxIdx[0]], width, height);
        auto [bx, by] = project(model.Position[model.Faces[i].vtxIdx[1]], width, height);
        auto [cx, cy] = project(model.Position[model.Faces[i].vtxIdx[2]], width, height);
        // OBJFace face = model.Faces[i];
        // Vector3 a = model.Position[face.vtxIdx[0]];
        // Vector3 b = model.Position[face.vtxIdx[1]];
        // Vector3 c = model.Position[face.vtxIdx[2]];
        line(ax, ay, bx, by, framebuffer, red);
        line(bx, by, cx, cy, framebuffer, red);
        line(cx, cy, ax, ay, framebuffer, red);
    }
    draw("Model Viewer",framebuffer,width,height);
}

int main(int argc, char** argv) {
    
    //lineTest();
    objTest();
    return 0;
}
