#include "../include/HeaderDX11.hpp"
#include "../include/resource.hpp"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

bool FindChunk(BYTE* pFile, DWORD sizeRes, DWORD fourcc, DWORD* sizeChunk, DWORD* posChunkData) {
    DWORD offset = 0;
    while (offset < sizeRes) {
        DWORD typeChunk;
        memcpy(&typeChunk, pFile + offset, sizeof(DWORD));
        offset += sizeof(DWORD);

        DWORD sizeChunkTmp;
        memcpy(&sizeChunkTmp, pFile + offset, sizeof(DWORD));
        offset += sizeof(DWORD);

        if (typeChunk == fourcc) {
            *sizeChunk = sizeChunkTmp;
            *posChunkData = offset;
            return true;
        }

        if (typeChunk == fourccRIFF)
            offset += sizeof(DWORD);
        else
            offset += sizeChunkTmp;
    }

    return false;
}

AudioManager::AudioManager() : pXAudio(nullptr), pMVoice(nullptr) {}

AudioManager::~AudioManager() {
    if (pMVoice != nullptr)
        pMVoice->DestroyVoice();
    CoUninitialize();
}

bool AudioManager::init() {
    try {
        if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
            throw "Failed to CoInitializeEx.";
        if (FAILED(XAudio2Create(pXAudio.GetAddressOf())))
            throw "Failed to create XAudio2.";
        if (FAILED(pXAudio->CreateMasteringVoice(&pMVoice)))
            throw "Failed to create MasteringVoice.";
    } catch (const char* e) {
        MessageBoxA(nullptr, e, "Warning", MB_OK);
        return false;
    }
    return true;
}

bool AudioManager::createAudio(unsigned int id, Audio* pAudio) {
    try {
        HRSRC hFileSrc = FindResource(nullptr, MAKEINTRESOURCE(id), "WAVE");
        if (!hFileSrc)
            throw "Failed to find wave resource.";

        HGLOBAL hFileGlb = LoadResource(nullptr, hFileSrc);
        if (!hFileGlb)
            throw "Failed to load wave resource.";

        BYTE* pLock = (BYTE*)LockResource(hFileGlb);
        if (!pLock)
            throw "Failed to lock wave resource.";

        DWORD sizeRes = SizeofResource(nullptr, hFileSrc);
        if (sizeRes == 0)
            throw "Failed to get size of wave resource.";

        DWORD sizeChunk;
        DWORD posChunk;

        DWORD typeFile;
        if (!FindChunk(pLock, sizeRes, fourccRIFF, &sizeChunk, &posChunk))
            throw "Failed to find RIFF chunk.";
        memcpy(&typeFile, pLock + posChunk, sizeof(DWORD));
        if (typeFile != fourccWAVE)
            throw "File type is different from WAVE.";

        ADPCMWAVEFORMAT wfx;
        ZeroMemory(&wfx, sizeof(ADPCMWAVEFORMAT));
        if (!FindChunk(pLock, sizeRes, fourccFMT, &sizeChunk, &posChunk))
            throw "Failed to find FMT  chunk.";
        wfx.wSamplesPerBlock = 128;
        wfx.wNumCoef = 7;
        ADPCMCOEFSET aCoef[7] = {{256, 0}, {512, -256}, {0, 0}, {192, 64}, {240, 0}, {460, -208}, {392, -232}};
        memcpy(&wfx.aCoef, aCoef, sizeof(aCoef));
        memcpy(&wfx.wfx, pLock + posChunk, sizeChunk);

        if (!FindChunk(pLock, sizeRes, fourccDATA, &sizeChunk, &posChunk))
            throw "Failed to find DATA chunk.";
        BYTE* pDataBuffer = new BYTE[sizeChunk];
        memcpy(pDataBuffer, pLock + posChunk, sizeChunk);

        ZeroMemory(&pAudio->buffer, sizeof(XAUDIO2_BUFFER));
        pAudio->buffer.Flags = XAUDIO2_END_OF_STREAM;
        pAudio->buffer.AudioBytes = sizeChunk;
        pAudio->buffer.pAudioData = pDataBuffer;

        if(FAILED(pXAudio->CreateSourceVoice(&pAudio->pSVoice, (WAVEFORMATEX*)&wfx)))
            throw "Failed to create source voice.";

        if(FAILED(pAudio->pSVoice->SubmitSourceBuffer(&pAudio->buffer)))
            throw "Failed to submit source buffer.";

    } catch (const char* error) {
        MessageBoxA(nullptr, error, "Error", MB_OK);
        return false;
    }

    return true;
}
