#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <dsound.h>
#include <stdint.h>
#include "helpers.h"

#ifdef BUILD_ARCANE_DLL
#define ARCANE_API __declspec(dllexport)
#else
#define ARCANE_API __declspec(dllimport)
#endif

#pragma pack(push, 1)
struct wav_header {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
};
#pragma pack(pop)

struct wav_data {
    wav_header header;
    uint8_t* audioData;
};

#ifdef __cplusplus
extern "C" {
#endif

ARCANE_API bool AEInitializeDirectSound(HWND hwnd, LPDIRECTSOUND8* pDirectSound);
ARCANE_API bool AECreateSecondaryBufferFromWAV(LPDIRECTSOUND8 DirectSound, wav_data* wave, LPDIRECTSOUNDBUFFER* secondaryBuffer);
ARCANE_API void AEFillSoundBufferWithWAVData(LPDIRECTSOUNDBUFFER secondaryBuffer, wav_data* wave);
ARCANE_API bool AELoadWAVFile(const char* filename, wav_data* wave);
ARCANE_API void AEPlayWAVFile(LPDIRECTSOUNDBUFFER secondaryBuffer, wav_data* wave);
ARCANE_API bool AEplaySound(const char* soundName, HWND hwnd);

#ifdef __cplusplus
}
#endif

#endif // SOUND_H
