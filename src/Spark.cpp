//
// Created by alvaro on 20/04/2026.
//

#include "Spark.h"

Spark::Spark() {
    this->mVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    reset();
}

const float SPARKS_RANDOMNESS = 12.0f;
const float SPARKS_SYSTEM_RADIUS = 0.3f;;
const float SPARKS_SIZE = 0.2;
void Spark::reset() {
    // RandomVel on Y
    this->mVelocity.y = mBaseYSpeed *( 1.3 - 0.6 * (rand() / static_cast<float>(RAND_MAX)));


    // Random pos on XZ
    this->mPosition = glm::vec3(rand() / static_cast<float>(RAND_MAX),
                                -0.3,
                                rand() / static_cast<float>(RAND_MAX))
                                * SPARKS_SYSTEM_RADIUS;

    this->mVelocity = glm::vec3(rand() / (float)RAND_MAX, this->mVelocity.y, rand() / (float)RAND_MAX);

    // scale
    float scaleFactor = 0.8f + 0.2f * rand() / (float)RAND_MAX;
    // scaleFactor = 1.f;
    this->mScale = scaleFactor * glm::vec3(1.0f) * SPARKS_SIZE;

    // Rotation
    glm::vec3 randomAxis = glm::normalize(glm::vec3(
        (float)rand() / RAND_MAX * 2.0f - 1.0f,
        (float)rand() / RAND_MAX * 2.0f - 1.0f,
        (float)rand() / RAND_MAX * 2.0f - 1.0f
    ));
    float angle = ((float)rand() / RAND_MAX) * glm::pi<float>();
    this->mRotation = glm::angleAxis(angle, randomAxis);
    this->mLifetime = 0.0f;
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
        reset();
    }
}
