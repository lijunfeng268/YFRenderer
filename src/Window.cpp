#include "Window.h"
#include "Framebuffer.h"
#include <cassert>
#include <cstring>

static const char* WINDOW_CLASS_NAME = "RendererWindowClass";
static const char* WINDOW_PROP_NAME = "WindowInstance";
static bool class_registered = false;

Window::Window(const char* title, int width, int height)
    : hwnd_(nullptr)
    , memory_dc_(nullptr)
    , surface_buffer_(nullptr)
    , width_(width)
    , height_(height)
    , should_close_(false)
{
    // 注册窗口类（只需要注册一次）
    if (!class_registered) {
        WNDCLASSA wc = {};
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszClassName = WINDOW_CLASS_NAME;
        
        ATOM atom = RegisterClassA(&wc);
        assert(atom != 0);
        class_registered = true;
    }

    // 创建窗口
    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT rect = { 0, 0, width, height };
    AdjustWindowRect(&rect, style, FALSE);
    
    int window_width = rect.right - rect.left;
    int window_height = rect.bottom - rect.top;

    hwnd_ = CreateWindowA(
        WINDOW_CLASS_NAME,
        title,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        window_width, window_height,
        NULL, NULL,
        GetModuleHandle(NULL),
        NULL
    );
    
    assert(hwnd_ != NULL);

    // 关联窗口实例
    SetPropA(hwnd_, WINDOW_PROP_NAME, this);

    // 创建surface用于绘制
    createSurface();

    // 显示窗口
    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
}

Window::~Window() {
    if (memory_dc_) {
        DeleteDC(memory_dc_);
    }
    if (hwnd_) {
        RemovePropA(hwnd_, WINDOW_PROP_NAME);
        DestroyWindow(hwnd_);
    }
}

void Window::createSurface() {
    // 创建内存DC用于双缓冲
    HDC window_dc = GetDC(hwnd_);
    memory_dc_ = CreateCompatibleDC(window_dc);
    ReleaseDC(hwnd_, window_dc);

    // 创建DIB section
    BITMAPINFOHEADER bi_header = {};
    bi_header.biSize = sizeof(BITMAPINFOHEADER);
    bi_header.biWidth = width_;
    bi_header.biHeight = -height_;  // 负值表示top-down
    bi_header.biPlanes = 1;
    bi_header.biBitCount = 32;      // BGRA格式
    bi_header.biCompression = BI_RGB;

    HBITMAP dib_bitmap = CreateDIBSection(
        memory_dc_,
        (BITMAPINFO*)&bi_header,
        DIB_RGB_COLORS,
        (void**)&surface_buffer_,
        NULL, 0
    );
    
    assert(dib_bitmap != NULL);
    SelectObject(memory_dc_, dib_bitmap);
}

void Window::drawFramebuffer(const Framebuffer& framebuffer) {
    // 将framebuffer数据复制到surface
    const unsigned char* fb_data = framebuffer.getData();
    int fb_width = framebuffer.getWidth();
    int fb_height = framebuffer.getHeight();
    
    // 确保尺寸匹配
    assert(fb_width == width_ && fb_height == height_);
    
    // 复制数据（假设framebuffer是RGBA格式，需要转换为BGRA）
    for (int i = 0; i < width_ * height_; i++) {
        surface_buffer_[i * 4 + 0] = fb_data[i * 4 + 2];  // B
        surface_buffer_[i * 4 + 1] = fb_data[i * 4 + 1];  // G
        surface_buffer_[i * 4 + 2] = fb_data[i * 4 + 0];  // R
        surface_buffer_[i * 4 + 3] = fb_data[i * 4 + 3];  // A
    }

    // 将内存DC的内容绘制到窗口
    HDC window_dc = GetDC(hwnd_);
    BitBlt(window_dc, 0, 0, width_, height_, memory_dc_, 0, 0, SRCCOPY);
    ReleaseDC(hwnd_, window_dc);
}

void Window::pollEvents() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    Window* window = (Window*)GetPropA(hwnd, WINDOW_PROP_NAME);
    
    if (window == nullptr) {
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    switch (msg) {
        case WM_CLOSE:
            window->should_close_ = true;
            return 0;
            
        case WM_KEYDOWN:
            if (wparam == VK_ESCAPE) {
                window->should_close_ = true;
            }
            return 0;
            
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }
}
