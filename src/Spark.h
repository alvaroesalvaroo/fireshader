//
// Created by alvaro on 20/04/2026.
//

#ifndef SPARK_H
#define SPARK_H
#include "LightEmissor.h"
#include "detail/type_vec.hpp"

#define SPARKS_RANDOMNESS 12.0f
#define TIME_TO_UPDATE_SPARKS 1.f
// float timeSinceUpdateSparks = 0;

class Spark : public LightEmissor{
public:
    Spark();
    void update(float dt) override;

    glm::vec3 mAcceleration = glm::vec3(0, 0, 0);
    glm::vec3 mVelocity;
    float mAccTimer;
    float mAccUpdateRate = 1.f;
    float mLifetime;
    float ySpeed;

    const float mBaseYSpeed        = 0.7f;
    const float mNoiseStrength = 800.f;
    const float mDamping       = 0.6f;
    const float maxY = 3.0f;

};


#endif //SPARK_H