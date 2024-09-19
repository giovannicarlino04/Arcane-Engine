#include <windows.h>
#include <Xinput.h>
#include <dsound.h>
#include <math.h>
#include <cstdint>
#include <cstdio>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define internal static
#define global_variable static
#define local_persist static

#include "sound.h"
#include "graphics.h"
#include "helpers.h"

#include <string>
#include <chrono>
#include <winuser.h>
#include <thread>

int playerX = 0;
int playerY = 0;
global_variable Texture playerTexture;

global_variable bool keyStates[256] = {};

void RenderTest(win32_offscreen_buffer* buffer, int BlueOffset, int GreenOffset)
{
    uint8_t* Row = (uint8_t*)buffer->memory;

    for (int32_t Y = 0; Y < buffer->height; Y++) {
        uint32_t* Pixel = (uint32_t*)Row;
        for (int32_t X = 0; X < buffer->width; X++) {
            uint8_t Blue = static_cast<uint8_t>(X + BlueOffset);
            uint8_t Green = static_cast<uint8_t>(Y + GreenOffset);
            *Pixel++ = ((Green << 8) | Blue);
        }
        Row += buffer->pitch;
    }
}

void GameUpdateAndRender(win32_offscreen_buffer* buffer, int BlueOffset, int GreenOffset)
{
    RenderTest(buffer, BlueOffset, GreenOffset);
}


global_variable float PI32 = 3.14159265f;

global_variable bool running;
global_variable LPDIRECTSOUNDBUFFER secondaryBuffer;
global_variable win32_offscreen_buffer globalBackBuffer;


internal void Win32ResizeDIBSection(win32_offscreen_buffer* buffer, int width, int height) {
    if (buffer->memory) {
        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }
    buffer->width = width;
    buffer->height = height;
    buffer->bytesPerPixel = 4;

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;

    int bitmapMemorySize = (buffer->width * buffer->height) * buffer->bytesPerPixel;
    buffer->memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    buffer->pitch = buffer->width * buffer->bytesPerPixel;
}

internal void Win32UpdateWindow(HDC hdc, int WindowWidth, int WindowHeight, win32_offscreen_buffer buffer) {
    float aspectRatio = (float)buffer.width / (float)buffer.height;

    int targetWidth = WindowWidth;
    int targetHeight = (int)(WindowWidth / aspectRatio);

    if (targetHeight > WindowHeight) {
        targetHeight = WindowHeight;
        targetWidth = (int)(WindowHeight * aspectRatio);
    }

    int offsetX = (WindowWidth - targetWidth) / 2;
    int offsetY = (WindowHeight - targetHeight) / 2;

    StretchDIBits(hdc, offsetX, offsetY, targetWidth, targetHeight,
        0, 0, buffer.width, buffer.height,
        buffer.memory, &buffer.info, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK MainWindowCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT Result = 0;

    switch (uMsg) {
    case WM_SIZE:
    {
        win32_window_dimension hWndDim = GetWindowDimension(hWnd);
        Win32ResizeDIBSection(&globalBackBuffer, hWndDim.width, hWndDim.height);
    }
    break;

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    {
        uint32_t VKCode = wParam;
        if (VKCode < ARRAY_COUNT(keyStates)) {
            keyStates[VKCode] = true; // Mark the key as pressed
        }
    }
    break;

    case WM_KEYUP:
    {
        uint32_t VKCode = wParam;
        if (VKCode < ARRAY_COUNT(keyStates)) {
            keyStates[VKCode] = false; // Mark the key as released
        }
    }
    break;

    case WM_DESTROY:
    {
        running = false;
    }
    break;

    case WM_CLOSE:
    {
        running = false;
    }
    break;

    case WM_ACTIVATEAPP:
    {
        OutputDebugString("WM_ACTIVATEAPP\n");
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT paint;
        HDC hdc = BeginPaint(hWnd, &paint);

        win32_window_dimension hWndDim = GetWindowDimension(hWnd);
        Win32UpdateWindow(hdc, hWndDim.width, hWndDim.height, globalBackBuffer);

        EndPaint(hWnd, &paint);
    }
    break;

    default:
    {
        Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    break;
    }

    return Result;
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSA windowClass = {};
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = "RengineWindowClass";
    

    if (RegisterClassA(&windowClass)) {
        HWND hWnd = CreateWindowExA(
            0,
            windowClass.lpszClassName,
            "Rengine",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0);

        if (hWnd) {
            running = true;

            // Resize the global back buffer.
            win32_window_dimension hWndDim = GetWindowDimension(hWnd);
            Win32ResizeDIBSection(&globalBackBuffer, hWndDim.width, hWndDim.height);


            while (running) {
                MSG msg;
                while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                    if (msg.message == WM_QUIT) {
                        running = false;
                    }

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                if (keyStates['W']) {
                    playerY++; // Move up
                }
                if (keyStates['S']) {
                    playerY--; // Move down
                }
                if (keyStates['A']) {
                    playerX--; // Move left
                }
                if (keyStates['D']) {
                    playerX++; // Move right
                }

                int windowWidth = globalBackBuffer.width;
                int windowHeight = globalBackBuffer.height;
                // Clamp player position
                if (playerX < 0) {
                    playerX = 0;
                }
                if (playerY < 0) {
                    playerY = 0;
                }
                if (playerX + playerTexture.width > windowWidth) {
                    playerX = windowWidth - playerTexture.width;
                }
                if (playerY + playerTexture.height > windowHeight) {
                    playerY = windowHeight - playerTexture.height;
                }


                ClearBuffer(&globalBackBuffer);
                RenderTest(&globalBackBuffer, 0, 0);

                LoadTexture("./res/texture/player.png", &playerTexture);
                RenderTexture(&globalBackBuffer, &playerTexture, playerX, playerY);

                HDC deviceContext = GetDC(hWnd);
                win32_window_dimension dim = GetWindowDimension(hWnd);
                Win32UpdateWindow(deviceContext, dim.width, dim.height, globalBackBuffer);
                ReleaseDC(hWnd, deviceContext);
                
            }
        }
    }
    return 0;
}
