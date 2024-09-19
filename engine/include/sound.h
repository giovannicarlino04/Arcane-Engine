#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <dsound.h>
#include <stdint.h>
#include "helpers.h"

// WAV file header structures
#pragma pack(push, 1) // Ensure no padding
struct wav_header {
    char chunkID[4];        // "RIFF"
    uint32_t chunkSize;     // File size - 8
    char format[4];         // "WAVE"
    char subchunk1ID[4];    // "fmt "
    uint32_t subchunk1Size; // Size of fmt
    uint16_t audioFormat;    // Audio format (PCM = 1)
    uint16_t numChannels;    // Number of channels
    uint32_t sampleRate;     // Sample rate
    uint32_t byteRate;       // Byte rate
    uint16_t blockAlign;     // Block align
    uint16_t bitsPerSample;  // Bits per sample
    char subchunk2ID[4];     // "data"
    uint32_t subchunk2Size;  // Size of data
};
#pragma pack(pop)

struct wav_data {
    wav_header header;
    uint8_t* audioData; // Pointer to audio data
};

// Function declarations
bool InitializeDirectSound(HWND hwnd, LPDIRECTSOUND8* pDirectSound);
bool CreateSecondaryBufferFromWAV(LPDIRECTSOUND8 DirectSound, wav_data* wave, LPDIRECTSOUNDBUFFER* secondaryBuffer);
void FillSoundBufferWithWAVData(LPDIRECTSOUNDBUFFER secondaryBuffer, wav_data* wave);
bool LoadWAVFile(const char* filename, wav_data* wave);
void PlayWAVFile(LPDIRECTSOUNDBUFFER secondaryBuffer);
bool playSound(const char* soundName, HWND hwnd);

#endif // AC_SOUND_H
