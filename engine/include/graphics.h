#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <png.h>
#include "file.h"

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

win32_window_dimension AEGetWindowDimension(HWND hWnd);
void AEResizeDIBSection(win32_offscreen_buffer* buffer, int width, int height);
void AEUpdateWindow(HDC hdc, int WindowWidth, int WindowHeight, win32_offscreen_buffer buffer);
bool AELoadTexture(const char* filename, Texture* texture);
bool AEFreeTexture(Texture* texture);
void AEClearBuffer(win32_offscreen_buffer* buffer);
void AERenderTexture(win32_offscreen_buffer* buffer, Texture* texture, int x, int y);

#endif