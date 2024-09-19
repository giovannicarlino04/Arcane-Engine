#ifndef AC_GRAPHICS_H
#define AC_GRAPHICS_H
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <png.h>

struct win32_window_dimension {
    int width;
    int height;
};

struct win32_offscreen_buffer {
    BITMAPINFO info;
    void* memory;
    int width;
    int height;
    int pitch;
    int bytesPerPixel;
};

struct Texture {
    int width;
    int height;
    uint8_t* data; // Pointer to pixel data
};

bool LoadTexture(const char* filename, Texture* texture);
void RenderTexture(win32_offscreen_buffer* buffer, Texture* texture, int x, int y);
win32_window_dimension GetWindowDimension(HWND hWnd);
void ClearBuffer(win32_offscreen_buffer* buffer);

#endif