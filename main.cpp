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
        line(Vector2i{ax, ay}, Vector2i{bx, by}, framebuffer, red);
        line(Vector2i{bx, by}, Vector2i{cx, cy}, framebuffer, red);
        line(Vector2i{cx, cy}, Vector2i{ax, ay}, framebuffer, red);
    }
    draw("Model Viewer",framebuffer,width,height);
}

void triangle(Framebuffer& framebuffer,Vector2i t0, Vector2i t1, Vector2i t2, Color color) 
{
//三角形面积为0
    if (t0.y==t1.y && t0.y==t2.y) return;
    //根据y的大小对坐标进行排序
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    //以高度差作为循环控制变量，此时不需要考虑斜率，因为着色完后每行都会被填充
    for (int i=0; i<total_height; i++) {
        //根据t1将三角形分割为上下两部分
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; 
        //计算A,B两点的坐标
        Vector2i A =               t0 + (t2-t0)*alpha;
        Vector2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        //根据A,B和当前高度对tga着色
        for (int j=A.x; j<=B.x; j++) {
            framebuffer.setPixel(j, t0.y+i, color);
        }
    }
}

void triangleTest()
{
    constexpr int width  = 640;
    constexpr int height = 640;
    Framebuffer framebuffer(width, height);
    Vector2i t0[3] = { Vector2i{10, 70},   Vector2i{50, 160},  Vector2i{70, 80} };
    Vector2i t1[3] = { Vector2i{180, 50},  Vector2i{150, 1},   Vector2i{70, 180} };
    Vector2i t2[3] = { Vector2i{180, 150}, Vector2i{120, 160}, Vector2i{130, 180} };
    triangle(framebuffer, t0[0], t0[1], t0[2], red);
    triangle(framebuffer, t1[0], t1[1], t1[2], white);
    triangle(framebuffer, t2[0], t2[1], t2[2], green);

    draw("Triangle Rasterization",framebuffer,width,height);
}
int main(int argc, char** argv) {
    
    //lineTest();
    //objTest();
    triangleTest();
    return 0;
}
