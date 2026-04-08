//
// Created by alvaro on 16/12/2025.
//

#include "../src/SoundEngine.h"
#include <SDL2/SDL_mixer.h>
#include <iostream>

// SDL_mixer constnats
const int MIXER_FREQUENCY = 44100;
const int MIXER_CHANNELS = 2; // Stereo
const int MIXER_CHUNK_SIZE = 2048;
const int MIXER_FLAGS = MIX_INIT_OGG | MIX_INIT_MP3; // OGG y MP3

SoundEngine::SoundEngine() = default;

SoundEngine::~SoundEngine()
{
    UnloadAll();
}

bool SoundEngine::Init()
{
    if (m_initialized) return true;

    // Inicializar SDL Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL Audio no pudo inicializarse: " << SDL_GetError() << std::endl;
        return false;
    }

    // Inicializar el módulo SDL_mixer
    if (Mix_Init(MIXER_FLAGS) != MIXER_FLAGS) {
        std::cerr << "SDL_mixer no pudo inicializarse: " << Mix_GetError() << std::endl;
        return false;
    }

    // Abrir el dispositivo de audio
    if (Mix_OpenAudio(MIXER_FREQUENCY, MIX_DEFAULT_FORMAT, MIXER_CHANNELS, MIXER_CHUNK_SIZE) < 0) {
        std::cerr << "Mix_OpenAudio falló: " << Mix_GetError() << std::endl;
        return false;
    }

    // Reservar 8 canales para efectos de sonido (opcional, pero buena práctica)
    Mix_AllocateChannels(8);

    m_initialized = true;
    return true;
}

void SoundEngine::UnloadAll()
{
    // Limpiar todos los recursos
    for (auto const& [key, music] : m_musicCache) {
        Mix_FreeMusic(music);
    }
    m_musicCache.clear();

    for (auto const& [key, chunk] : m_audioCache) {
        Mix_FreeChunk(chunk);
    }
    m_audioCache.clear();

    if (m_initialized) {
        Mix_CloseAudio();
        Mix_Quit();
        SDL_QuitSubSystem(SDL_INIT_AUDIO); // Limpiar solo el subsistema de audio
    }
    m_initialized = false;
}