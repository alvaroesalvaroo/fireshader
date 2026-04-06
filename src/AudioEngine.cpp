//
// Created by alv18 on 16/12/2025.
//

#include "AudioEngine.h"

#include <bits/stl_algo.h>

// Init static maps and variables
std::map<std::string, Mix_Music*> AudioEngine::m_musicCache;
std::map<std::string, Mix_Chunk*> AudioEngine::m_soundCache;
std::map<std::string, TimePoint> AudioEngine::m_lastPlayed;

const std::chrono::milliseconds AudioEngine::MIN_DELAY = std::chrono::milliseconds(50);
bool AudioEngine::m_isInitialized = false;

bool AudioEngine::Init()
{
    if (m_isInitialized) return true;

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "AUDIO ERROR: SDL Audio could not init: " << SDL_GetError() << std::endl;
        return false;
    }
    const int MIXER_FLAGS = MIX_INIT_OGG | MIX_INIT_MP3;
    if (Mix_Init(MIXER_FLAGS) != MIXER_FLAGS) {
        std::cerr << "AUDIO WARNING: SDL_mixer format init failed (MP3/OGG may fail): " << Mix_GetError() << std::endl;
    }

    const int MIXER_FREQUENCY = 44100;
    const int MIXER_CHANNELS = 2;
    const int MIXER_CHUNK_SIZE = 2048;

    if (Mix_OpenAudio(MIXER_FREQUENCY, MIX_DEFAULT_FORMAT, MIXER_CHANNELS, MIXER_CHUNK_SIZE) < 0) {
        std::cerr << "AUDIO CRITICAL ERROR: Mix_OpenAudio failed (Device not opened): " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_AllocateChannels(16);

    m_isInitialized = true;
    std::cout << "SDL Audio Mixer inited ok." << std::endl;
    return true;
}

void AudioEngine::Cleanup()
{
    if (!m_isInitialized) return;

    StopMusic();

    // Clean music cache
    for (auto const& [key, music] : m_musicCache) {
        Mix_FreeMusic(music);
    }
    m_musicCache.clear();

    // Clean sound (chunks) caché
    for (auto const& [key, chunk] : m_soundCache) {
        Mix_FreeChunk(chunk);
    }
    m_soundCache.clear();

    // Close
    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    m_isInitialized = false;
    std::cout << "AudioEngine unloaded." << std::endl;
}

bool AudioEngine::LoadMusic(const std::string& key, const std::string& filePath)
{
    if (m_musicCache.count(key)) return true;

    Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    if (!music) {
        std::cerr << "AUDIO ERROR: Music could not be loaded '" << filePath << "': " << Mix_GetError() << std::endl;
        return false;
    }
    m_musicCache[key] = music;
    return true;
}

void AudioEngine::PlayMusic(const std::string& key, int loops)
{
    if (m_musicCache.count(key)) {
        if (Mix_PlayMusic(m_musicCache[key], loops) == -1) {
            std::cerr << "AUDIO ERROR: Music could not be played: " << Mix_GetError() << std::endl;
        }
    } else {
        std::cerr << "AUDIO WARNING: Music not found in cache: " << key << std::endl;
    }
}

void AudioEngine::PauseMusic() {
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
        Mix_PauseMusic();
    }
}

void AudioEngine::ResumeMusic()
{
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void AudioEngine::StopMusic()
{
    if (Mix_PlayingMusic() || Mix_PausedMusic()) {
        Mix_HaltMusic();
    }
}

void AudioEngine::SetMusicVolume(int volume)
{
    Mix_VolumeMusic(std::clamp(volume, 0, MIX_MAX_VOLUME));
}
void AudioEngine::UnloadMusic(const std::string& key)
{
    if (m_musicCache.count(key)) {
        StopMusic();
        Mix_FreeMusic(m_musicCache[key]);
        m_musicCache.erase(key);
    }
}


// ---------- SOUNDS (audio chunks) -----------------
// --------------------------------------------------

bool AudioEngine::LoadSound(const std::string& key, const std::string& filePath)
{
    if (m_soundCache.count(key)) return true;


    Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
    if (!chunk) {
        std::cerr << "AUDIO ERROR: Cannot load sound '" << filePath << "': " << Mix_GetError() << std::endl;
        return false;
    }
    m_soundCache[key] = chunk;
    return true;
}

int AudioEngine::PlaySound(const std::string& key, int loops, int channel)
{
    // Cool down to avoid play same sound repeteadly
    auto now = std::chrono::steady_clock::now();
    if (m_lastPlayed.count(key)) {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastPlayed[key]);
        if (elapsed < MIN_DELAY) {
            // std::cout << "AUDIO WARNING: key " << key << " was attempted to be played repeatedly" << std::endl;
            return -1;
        }
    }
    if (!m_soundCache.count(key)) {
        std::cerr << "AUDIO WARNING: Sound not found in cache: " << key << std::endl;
        return -2;
    }

    int played_channel = Mix_PlayChannel(channel, m_soundCache[key], loops);
    if (played_channel == -1) {
        std::cerr << "AUDIO WARNING: Failed playing sound " << key << ": " << Mix_GetError() << std::endl;
        return -3;
    }

    m_lastPlayed[key] = now; // Audio succesfully played
    return played_channel;

}

void AudioEngine::StopChannel(int channel)
{
    Mix_HaltChannel(channel);
}

void AudioEngine::SetSoundVolume(int volume)
{
    // Establece el volumen global para todos los Chunks
    Mix_Volume(-1, std::clamp(volume, 0, MIX_MAX_VOLUME));
}

void AudioEngine::UnloadSound(const std::string& key)
{
    if (m_soundCache.count(key)) {
        Mix_FreeChunk(m_soundCache[key]);
        m_soundCache.erase(key);
    }
}
