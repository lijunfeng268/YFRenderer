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

void line(Vector2i a, Vector2i b, Framebuffer& framebuffer, Color color = white) {
    bool steep = std::abs(a.x-b.x) < std::abs(a.y-b.y);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(a.x, a.y);
        std::swap(b.x, b.y);
    }
    if (a.x>b.x) { // make it left−to−right
        std::swap(a.x, b.x);
        std::swap(a.y, b.y);
    }
    int y = a.y;
    int ierror = 0;
    for (int x=a.x; x<=b.x; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.setPixel(y, x, color);
        else
            framebuffer.setPixel(x, y, color);
        ierror += 2 * std::abs(b.y-a.y);
        y += (b.y > a.y ? 1 : -1) * (ierror > b.x - a.x);
        ierror -= 2 * (b.x-a.x)   * (ierror > b.x - a.x);
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
        line(Vector2i{ax, ay}, Vector2i{bx, by}, framebuffer,
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

std::tuple<Vector2i,real> project(Vector3 v,int width, int height) { // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
    return { Vector2i{ width- (Int32)(v.x + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
                       height-(Int32)(v.y + 1.) * height/2 }, // we want to shift the vector (x,y) and then scale it to span the entire screen.
            v.z };
}

void objTest()
{
    constexpr int width  = 640;
    constexpr int height = 640;
    Framebuffer framebuffer(width, height);
    std::string modelFile = "D:/github/YFRender/asset/diablo3_pose.obj";
    Model model(modelFile);
    for (int i=0; i<model.nfaces(); i++) { // iterate through all triangles
        auto [a, az] = project(model.Position[model.Faces[i].vtxIdx[0]], width, height);
        auto [b, bz] = project(model.Position[model.Faces[i].vtxIdx[1]], width, height);
        auto [c, cz] = project(model.Position[model.Faces[i].vtxIdx[2]], width, height);
        // OBJFace face = model.Faces[i];
        // Vector3 a = model.Position[face.vtxIdx[0]];
        // Vector3 b = model.Position[face.vtxIdx[1]];
        // Vector3 c = model.Position[face.vtxIdx[2]];
        line(a, b, framebuffer, red);
        line(b, c, framebuffer, red);
        line(c, a, framebuffer, red);
    }
    draw("Model Viewer",framebuffer,width,height);
}

real signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

void triangle(Framebuffer& framebuffer,Vector2i t0, Vector2i t1, Vector2i t2,
    real az, real bz, real cz, Color color) 
{
    int bbminx = std::min(std::min(t0.x, t1.x), t2.x); // bounding box for the triangle
    int bbminy = std::min(std::min(t0.y, t1.y), t2.y); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(t0.x, t1.x), t2.x);
    int bbmaxy = std::max(std::max(t0.y, t1.y), t2.y);
    real total_area = signed_triangle_area(t0.x, t0.y, t1.x, t1.y, t2.x, t2.y);
    if (total_area<1.0) 
        return;

    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, t1.x, t1.y, t2.x, t2.y) / total_area;
            double beta  = signed_triangle_area(x, y, t2.x, t2.y, t0.x, t0.y) / total_area;
            double gamma = signed_triangle_area(x, y, t0.x, t0.y, t1.x, t1.y) / total_area;
            if (alpha<0 || beta<0 || gamma<0) continue; // negative barycentric coordinate => the pixel is outside the triangle
            unsigned char z = static_cast<unsigned char>(alpha * az + beta * bz + gamma * cz);
            framebuffer.setPixel(x, y, {z,z,z,z});
        }
    }
}

void triangleWithColor(Framebuffer& framebuffer,Vector2i* t, real* z, Color* color)
{
    int bbminx = std::min(std::min(t[0].x, t[1].x), t[2].x); // bounding box for the triangle
    int bbminy = std::min(std::min(t[0].y, t[1].y), t[2].y); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(t[0].x, t[1].x), t[2].x);
    int bbmaxy = std::max(std::max(t[0].y, t[1].y), t[2].y);
    real total_area = signed_triangle_area(t[0].x, t[0].y, t[1].x, t[1].y, t[2].x, t[2].y);
    if (total_area<1.0) 
        return;

    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, t[1].x, t[1].y, t[2].x, t[2].y) / total_area;
            double beta  = signed_triangle_area(x, y, t[2].x, t[2].y, t[0].x, t[0].y) / total_area;
            double gamma = signed_triangle_area(x, y, t[0].x, t[0].y, t[1].x, t[1].y) / total_area;
            if (alpha<0 || beta<0 || gamma<0) continue; // negative barycentric coordinate => the pixel is outside the triangle
            unsigned char a = static_cast<unsigned char>(alpha * z[0] + beta * z[1] + gamma * z[2]);
            Color newColor = {UInt8(color[0].r * alpha + color[1].r * beta + color[2].r * gamma),
                              UInt8(color[0].g * alpha + color[1].g * beta + color[2].g * gamma),
                              UInt8(color[0].b * alpha + color[1].b * beta + color[2].b * gamma),
                              255};

            framebuffer.setPixel(x, y, newColor);
        }
    }
}

void triangleTest()
{
    constexpr int width  = 640;
    constexpr int height = 640;
    Framebuffer framebuffer(width, height);
    // Vector2i t0[3] = { Vector2i{17, 4},   Vector2i{55, 39},  Vector2i{23, 59} };
    // Vector2i t1[3] = { Vector2i{70, 180}, Vector2i{150, 1}, Vector2i{180, 50}    };
    // Vector2i t2[3] = { Vector2i{130, 180}, Vector2i{120, 160}, Vector2i{180, 150} };
    // triangle(framebuffer, t0[0], t0[1], t0[2], 13, 128, 255, red);
    // triangle(framebuffer, t1[0], t1[1], t1[2], 13, 128, 255, white);
    // triangle(framebuffer, t2[0], t2[1], t2[2], 13, 128, 255, green);

    Vector2i t[3] = { Vector2i{70, 180}, Vector2i{150, 1}, Vector2i{180, 50} };
    real z[3] = { 13, 128, 255 };
    Color color[3] = { red, green, blue };
    triangleWithColor(framebuffer, t, z, color);

    draw("Triangle Rasterization",framebuffer,width,height);
}

void modelRansterization()
{
    constexpr int width  = 640;
    constexpr int height = 640;
    Framebuffer framebuffer(width, height);
    std::string modelFile = "D:/github/YFRender/asset/diablo3_pose.obj";
    Model model(modelFile);
    std::srand(std::time({}));
    for (int i=0; i<model.nfaces(); i++) { // iterate through all triangles
        auto [a, az] = project(model.Position[model.Faces[i].vtxIdx[0]], width, height);
        auto [b, bz] = project(model.Position[model.Faces[i].vtxIdx[1]], width, height);
        auto [c, cz] = project(model.Position[model.Faces[i].vtxIdx[2]], width, height);
        triangle(framebuffer, a, b, c, az, bz, cz,
        { static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255),
            static_cast<unsigned char>(rand() % 255) });
    }
    draw("Model Rasterization",framebuffer,width,height);
}

int main(int argc, char** argv) {
    
    //lineTest();
    //objTest();
    triangleTest();
    //modelRansterization();
    return 0;
}
