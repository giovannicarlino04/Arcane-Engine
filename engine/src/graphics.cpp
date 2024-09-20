#include "graphics.h"

win32_window_dimension AEGetWindowDimension(HWND hWnd) {
    win32_window_dimension Result;

    RECT rect;
    GetClientRect(hWnd, &rect);
    Result.width = rect.right - rect.left;
    Result.height = rect.bottom - rect.top;

    return 
    Result;
}


void AEResizeDIBSection(win32_offscreen_buffer* buffer, int width, int height) {
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

void AEUpdateWindow(HDC hdc, int WindowWidth, int WindowHeight, win32_offscreen_buffer buffer) {
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

void AEdrawRectangle(int x, int y, int width, int height, uint32_t color, win32_offscreen_buffer globalBackBuffer) {
    for (int row = y; row < y + height; ++row) {
        for (int col = x; col < x + width; ++col) {
            if (row >= 0 && row < globalBackBuffer.height && col >= 0 && col < globalBackBuffer.width) {
                uint32_t* pixel = (uint32_t*)((uint8_t*)globalBackBuffer.memory +
                                              row * globalBackBuffer.pitch + col * sizeof(uint32_t));
                *pixel = color;
            }
        }
    }
}

bool AELoadTexture(const char* filename, Texture* texture) {
    FILE *file = AEReadEntireFile(filename);
    if (!file) {
        printf("Failed to open texture file: %s\n", filename);
        return false;
    }

    png_byte header[8];
    fread(header, 1, 8, file);
    if (png_sig_cmp(header, 0, 8)) {
        fclose(file);
        printf("File %s is not recognized as a PNG file.\n", filename);
        return false;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(file);
        printf("Failed to create PNG read struct.\n");
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(file);
        printf("Failed to create PNG info struct.\n");
        return false;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(file);
        printf("Error during PNG creation.\n");
        return false;
    }

    png_init_io(png, file);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);

    texture->width = png_get_image_width(png, info);
    texture->height = png_get_image_height(png, info);

    // Set the format to RGBA
    png_set_expand(png);
    png_set_strip_16(png);
    png_set_add_alpha(png, 0xFFFF, PNG_FILLER_AFTER); // Add alpha channel

    png_read_update_info(png, info);

    size_t rowbytes = png_get_rowbytes(png, info);
    texture->data = new uint8_t[texture->height * rowbytes];

    png_bytep* row_pointers = new png_bytep[texture->height];
    for (int i = 0; i < texture->height; ++i) {
        row_pointers[texture->height - 1 - i] = texture->data + i * rowbytes; // PNG rows are bottom to top
    }

    png_read_image(png, row_pointers);

    delete[] row_pointers;
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(file);

    return true;
}

bool AEFreeTexture(Texture* texture){
    texture->data = nullptr;
    return true;
}

void AEClearBuffer(win32_offscreen_buffer* buffer) {
    uint8_t* Row = (uint8_t*)buffer->memory;
    for (int32_t Y = 0; Y < buffer->height; Y++) {
        uint32_t* Pixel = (uint32_t*)Row;
        for (int32_t X = 0; X < buffer->width; X++) {
            *Pixel++ = 0x00000000; // Set each pixel to black (ARGB)
        }
        Row += buffer->pitch;
    }
}

void AERenderTexture(win32_offscreen_buffer* buffer, Texture* texture, int x, int y) {
    uint8_t* Row = (uint8_t*)buffer->memory;

    for (int32_t Y = 0; Y < texture->height; Y++) {
        uint32_t* Pixel = (uint32_t*)(Row + (y + Y) * buffer->pitch + x * 4); // Calculate the pixel position
        uint8_t* textureRow = texture->data + Y * texture->width * 4; // Pointer to texture row
        for (int32_t X = 0; X < texture->width; X++) {
            uint8_t Blue = textureRow[X * 4];     // Assuming BGRA format
            uint8_t Green = textureRow[X * 4 + 1];
            uint8_t Red = textureRow[X * 4 + 2];
            uint8_t Alpha = textureRow[X * 4 + 3];
            *Pixel++ = ((Alpha << 24) | (Red << 16) | (Green << 8) | Blue); // Combine into a single pixel
        }
    }
}
