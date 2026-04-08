/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm.hpp>

#include "GameObject2D.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"
#include "GameObject2D.h"


class Texture2D;
class Shader;

// Represents a single particle and its state
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
    // Constructor
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    // Update all particles
    void Update(float dt, GameObject2D &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    // Render all particles
    void Draw();
private:
    // State
    std::vector<Particle> particles;
    unsigned int initialAmount;
    const float timeToInstantiate = 0.01f;
    float timeSinceLastParticle = timeToInstantiate;
    glm::vec4 baseColor = glm::vec4(0.3, 0.0, 1, 0.7);
    // Render state
    Shader shader;
    Texture2D texture;
    unsigned int VAO;
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle &particle, GameObject2D &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif