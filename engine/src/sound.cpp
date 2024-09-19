#include <dsound.h>
#include <cstdio>
#include <cstring>
#include "sound.h"

bool InitializeDirectSound(HWND hwnd, LPDIRECTSOUND8* pDirectSound) {
    HRESULT result = DirectSoundCreate8(NULL, pDirectSound, NULL);
    if (FAILED(result)) {
        printf("Failed to initialize DirectSound!\n");
        return false;
    }

    result = (*pDirectSound)->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if (FAILED(result)) {
        printf("Failed to set cooperative level!\n");
        return false;
    }

    return true;
}

bool CreateSecondaryBufferFromWAV(LPDIRECTSOUND8 DirectSound, wav_data* wave, LPDIRECTSOUNDBUFFER* secondaryBuffer) {
    if (!DirectSound) {
        printf("DirectSound not initialized!\n");
        return false;
    }

    WAVEFORMATEX waveFormat = {};
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = wave->header.numChannels;
    waveFormat.nSamplesPerSec = wave->header.sampleRate;
    waveFormat.wBitsPerSample = wave->header.bitsPerSample;
    waveFormat.nBlockAlign = wave->header.blockAlign;
    waveFormat.nAvgBytesPerSec = wave->header.byteRate;
    waveFormat.cbSize = 0;

    DSBUFFERDESC bufferDesc = {};
    bufferDesc.dwSize = sizeof(bufferDesc);
    bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = wave->header.subchunk2Size;
    bufferDesc.lpwfxFormat = &waveFormat;

    HRESULT result = DirectSound->CreateSoundBuffer(&bufferDesc, secondaryBuffer, NULL);
    if (FAILED(result)) {
        printf("Failed to create secondary sound buffer!\n");
        return false;
    }

    return true;
}

void FillSoundBufferWithWAVData(LPDIRECTSOUNDBUFFER secondaryBuffer, wav_data* wave) {
    if (!secondaryBuffer || !wave->audioData) {
        printf("Invalid buffer or audio data!\n");
        return;
    }

    VOID* region1;
    VOID* region2;
    DWORD region1Size;
    DWORD region2Size;

    HRESULT result = secondaryBuffer->Lock(0, wave->header.subchunk2Size, &region1, &region1Size, &region2, &region2Size, 0);
    if (SUCCEEDED(result)) {
        memcpy(region1, wave->audioData, region1Size);
        if (region2) {
            memcpy(region2, wave->audioData + region1Size, region2Size);
        }
        secondaryBuffer->Unlock(region1, region1Size, region2, region2Size);
    } else {
        printf("Failed to lock sound buffer!\n");
    }
}

bool LoadWAVFile(const char* filename, wav_data* wave) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open WAV file!\n");
        return false;
    }

    // Read the WAV header
    fread(&wave->header, sizeof(wav_header), 1, file);
    if (memcmp(wave->header.chunkID, "RIFF", 4) != 0 ||
        memcmp(wave->header.format, "WAVE", 4) != 0 ||
        memcmp(wave->header.subchunk1ID, "fmt ", 4) != 0 ||
        memcmp(wave->header.subchunk2ID, "data", 4) != 0) {
        printf("Invalid WAV file format!\n");
        fclose(file);
        return false;
    }

    wave->audioData = new uint8_t[wave->header.subchunk2Size];
    fread(wave->audioData, wave->header.subchunk2Size, 1, file);
    
    if (ferror(file)) {
        printf("Error reading WAV file data!\n");
        fclose(file);
        delete[] wave->audioData;
        return false;
    }

    fclose(file);
    return true;
}

void PlayWAVFile(LPDIRECTSOUNDBUFFER secondaryBuffer, wav_data* wave) {
    if (!secondaryBuffer) {
        printf("Invalid sound buffer!\n");
        return;
    }

    HRESULT result = secondaryBuffer->Play(0, 0, 0);
    if (FAILED(result)) {
        printf("Failed to play sound!\n");
    } else {
        // Calculate duration in seconds
        float duration = static_cast<float>(wave->header.subchunk2Size) /
                         (wave->header.sampleRate * wave->header.numChannels * (wave->header.bitsPerSample / 8));
        printf("Playing sound duration: %.2f seconds\n", duration);
    }
}

bool playSound(const char* soundName, HWND hwnd) {
    LPDIRECTSOUNDBUFFER secondaryBuffer = nullptr;
    wav_data wave = {};

    LPDIRECTSOUND8 DirectSound = nullptr;
    if (!InitializeDirectSound(hwnd, &DirectSound)) {
        printf("Failed to initialize DirectSound!\n");
        return false;
    }

    if (LoadWAVFile(soundName, &wave)) {
        if (CreateSecondaryBufferFromWAV(DirectSound, &wave, &secondaryBuffer)) {
            FillSoundBufferWithWAVData(secondaryBuffer, &wave);
            PlayWAVFile(secondaryBuffer, &wave);
        }
        printf("Sound played\n");
    }
    delete[] wave.audioData; // Don't forget to clean up the allocated memory
    return true;
}
