//
// Created by alv18 on 16/12/2025.
//

#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H
#pragma once

#include <chrono>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <map>
#include <iostream>
#include <unordered_map>

typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;


class AudioEngine
{
public:

    static bool Init();
    static void Cleanup();

    static bool LoadMusic(const std::string& key, const std::string& filePath);
    static void PlayMusic(const std::string& key, int loops = -1);
    static void PauseMusic();
    static void ResumeMusic();
    static void StopMusic();
    static void SetMusicVolume(int volume); // 0-128
    static void UnloadMusic(const std::string& key);

    static bool LoadSound(const std::string& key, const std::string& filePath);

    static int PlaySound(const std::string& key, int loops = 0, int channel = -1);
    static void StopChannel(int channel);
    static void SetSoundVolume(int volume);
    static void UnloadSound(const std::string& key);

private:

    static std::map<std::string, Mix_Music*> m_musicCache;
    static std::map<std::string, Mix_Chunk*> m_soundCache;
    static bool m_isInitialized;

    // Last played sound
    static std::map<std::string, TimePoint> m_lastPlayed;
    static const std::chrono::milliseconds MIN_DELAY;
};
#endif //AUDIOENGINE_H
