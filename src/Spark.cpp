//
// Created by alvaro on 20/04/2026.
//

#include "Spark.h"

Spark::Spark() {
    // RandomVel
    glm::vec3 randomVel = glm::vec3(
    ((rand() / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f) * SPARKS_RANDOMNESS,
    0.0f,
    ((rand() / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f) * SPARKS_RANDOMNESS);
    this->mVelocity = randomVel;

    this->mVelocity.y = mBaseYSpeed *( 1.3 - 0.6 * (rand() / static_cast<float>(RAND_MAX)));

    this->mPosition = glm::vec3(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX));
}

void Spark::update(float dt) {

    this->mAccTimer += dt;
    if (this->mAccTimer >= this->mAccUpdateRate) {
        this->mAccTimer = 0;
        this->mAccUpdateRate = 1.f + (rand() / static_cast<float>(RAND_MAX));
        // std::cout << "update spark acc" << std::endl;

        glm::vec3 randomAccel = glm::vec3(
            ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * mNoiseStrength,
            0.0f,
            ((rand() / (float)RAND_MAX) * 2.0f - 1.0f) * mNoiseStrength
        );
        this->mAcceleration = randomAccel;
    }

    this->mVelocity.x = this->mVelocity.x * mDamping + this->mAcceleration.x * dt;
    // this->mVelocity.y = mBaseYSpeed;
    this->mVelocity.z = this->mVelocity.z * mDamping + this->mAcceleration.z * dt;

    this->mPosition += this->mVelocity * dt;
    this->mLifetime += dt;

    if (this->mPosition.y > maxY) {
        this->mPosition = this->mPosition = 0.1f * glm::vec3(rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX), rand() / static_cast<float>(RAND_MAX));

        this->mVelocity = glm::vec3(rand() / (float)RAND_MAX, this->mVelocity.y, rand() / (float)RAND_MAX);
        this->mLifetime = 0.0f;
    }
}
