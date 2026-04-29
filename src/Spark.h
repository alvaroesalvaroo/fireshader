//
// Created by alvaro on 20/04/2026.
//

#ifndef SPARK_H
#define SPARK_H
#include "LightEmissor.h"
#include "detail/type_vec.hpp"



class Spark : public LightEmissor{
public:
    Spark();

    void reset();

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