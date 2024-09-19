#include "file.h"

// Function to open a file and return a FILE*
FILE *AEReadEntireFile(const char *fileName) {
    FILE *file = fopen(fileName, "rb"); 
    
    if (!file) {
        return nullptr;
    }
    
    return file;
}

// Function to free the memory allocated for a file
void AEFreeFileMemory(void *memory) {
    if (memory) {
        VirtualFree(memory, 0, MEM_RELEASE);
    }
}

// Function to write memory to an entire file
bool AEWriteEntireFile(char *fileName, uint32_t memorySize, void *memory) {
    bool result = false;
    HANDLE fileHandle = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (fileHandle != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        if (WriteFile(fileHandle, memory, memorySize, &bytesWritten, NULL) && bytesWritten == memorySize) {
            result = true;
        }
        CloseHandle(fileHandle);
    }
    
    return result;
}
