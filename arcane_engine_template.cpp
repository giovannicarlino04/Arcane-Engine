// Engine Includes
#include "sound.h"
#include "helpers.h"
#include "file.h"
#include "graphics.h"

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#define internal static
#define global_variable static
#define local_persist static

global_variable bool keyStates[256] = {};
global_variable float PI32 = 3.14159265f;
global_variable bool running;
global_variable LPDIRECTSOUNDBUFFER secondaryBuffer;
global_variable win32_offscreen_buffer globalBackBuffer;

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

LRESULT CALLBACK MainWindowCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT Result = 0;

    switch (uMsg) {
    case WM_SIZE: {
        win32_window_dimension hWndDim = AEGetWindowDimension(hWnd);
        AEResizeDIBSection(&globalBackBuffer, hWndDim.width, hWndDim.height);
    } break;

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN: {
        uint32_t VKCode = wParam;
        if (VKCode < ARRAY_COUNT(keyStates)) {
            keyStates[VKCode] = true; // Mark the key as pressed
        }
    } break;

    case WM_KEYUP: {
        uint32_t VKCode = wParam;
        if (VKCode < ARRAY_COUNT(keyStates)) {
            keyStates[VKCode] = false; // Mark the key as released
        }
    } break;

    case WM_LBUTTONDOWN: {
    }

    case WM_DESTROY:
    case WM_CLOSE: {
        running = false;
    } break;

    case WM_PAINT: {
        PAINTSTRUCT paint;
        HDC hdc = BeginPaint(hWnd, &paint);
        AEUpdateWindow(hdc, globalBackBuffer.width, globalBackBuffer.height, globalBackBuffer);
        EndPaint(hWnd, &paint);
    } break;

    default: {
        Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
    } break;
    }

    return Result;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASSA windowClass = {};
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = MainWindowCallback;
    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = "ae_window_class";

    if (RegisterClassA(&windowClass)) {
        HWND hWnd = CreateWindowExA(
            0,
            windowClass.lpszClassName,
            "Arcane Engine Window",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0
        );

        if (hWnd) {
            running = true;

            // Resize the global back buffer.
            win32_window_dimension hWndDim = AEGetWindowDimension(hWnd);
            AEResizeDIBSection(&globalBackBuffer, hWndDim.width, hWndDim.height);


            while (running) {
                MSG msg;
                while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                    if (msg.message == WM_QUIT) {
                        running = false;
                    }

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                // Handle key states if necessary
                if (keyStates['W']) {
                    // Handle 'W' key press
                }
                if (keyStates['S']) {
                    // Handle 'S' key press
                }
                if (keyStates['A']) {
                    // Handle 'A' key press
                }
                if (keyStates['D']) {
                    // Handle 'D' key press
                }

                // Clear the back buffer
                AEClearBuffer(&globalBackBuffer);


                HDC deviceContext = GetDC(hWnd);
                win32_window_dimension dim = AEGetWindowDimension(hWnd);
                AEUpdateWindow(deviceContext, dim.width, dim.height, globalBackBuffer);
                ReleaseDC(hWnd, deviceContext);
            }
        }
    }
    return 0;
}
