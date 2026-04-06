//
// Created by alvaro on 16/12/2025.
//

#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>

class SoundEngine
{
public:
    SoundEngine();
    ~SoundEngine();

    bool Init();
    void UnloadAll();

    // Music Streaming for large file
    bool LoadMusic(const std::string& key, const std::string& filePath);
    void PlayMusicStream(const std::string& key, int loops = -1); // loops = -1 para loop infinito
    void PauseMusicStream();
    void ResumeMusicStream();
    void StopMusicStream();
    void SetMusicVolume(int volume);
    void UnloadMusic(const std::string& key);

    // Audio chunks
    bool LoadAudio(const std::string& key, const std::string& filePath);
    int PlayAudio(const std::string& key, int loops = 0, int channel = -1); // channel = -1 para usar el siguiente disponible
    void StopAudio(int channel);
    void SetAudioVolume(int volume); // 0-128
    void UnloadAudio(const std::string& key);

private:
    // Containers for cache
    std::map<std::string, Mix_Music*> m_musicCache;
    std::map<std::string, Mix_Chunk*> m_audioCache;

    bool m_initialized = false;
};

#endif
