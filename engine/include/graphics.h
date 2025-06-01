#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <functional>
#include "file.h"
#include <png.h>

#ifdef BUILD_ARCANE_DLL
#define ARCANE_API __declspec(dllexport)
#else
#define ARCANE_API __declspec(dllimport)
#endif

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

#ifdef __cplusplus
extern "C" {
#endif

ARCANE_API win32_window_dimension AEGetWindowDimension(HWND hWnd);
ARCANE_API void AEDrawRectangle(int x, int y, int width, int height, uint32_t color, win32_offscreen_buffer *globalBackBuffer);
ARCANE_API void AEResizeDIBSection(win32_offscreen_buffer* buffer, int width, int height);
ARCANE_API void AEUpdateWindow(HDC hdc, int WindowWidth, int WindowHeight, win32_offscreen_buffer buffer);
ARCANE_API bool AELoadTexture(const char* filename, Texture* texture);
ARCANE_API bool AEFreeTexture(Texture* texture);
ARCANE_API void AEClearBuffer(win32_offscreen_buffer* buffer);
ARCANE_API void AERenderTexture(win32_offscreen_buffer* buffer, const Texture* texture, int x, int y);

#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_H
