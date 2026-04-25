//
// Created by alv18 on 02/05/2025.
//

#ifndef CAMERA3D_H
#define CAMERA3D_H
#include "../glfw/deps/linmath.h"
#include "glm.hpp"



class Camera3D {
public:
    Camera3D();
    Camera3D(float sensitivityTurn, float cameraSpeed, float fov);
    ~Camera3D();
    void updatePosition(bool *moveArray, float deltaTime); // 4position up-down-left-right movement values
    void turn(float xTurn, float yTurn, float deltaTime);
    void setZoom(float fov);

    // Setters
    void setScreenSize(int width, int height);
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 position);
    glm::vec3 getPosition(); // Needed for ilumination
    void setCameraFront(float x, float y, float z);
    void setCameraFront(glm::vec3 front);
    void setCameraUp(float x, float y, float z);
    void setCameraUp(glm::vec3 up);

    void updateViewUniform(int uniformID);

    void updateProjectionMatrix();

    void updateProjectionUniform(int uniformID);

    glm::mat4 getViewProjectionProductMatrix();
private:
    glm::mat4	mViewMatrix;
    glm::mat4	mProjectionMatrix;

    int mScreenWidth;
    int mScreenHeight;

    //Camera vectors and configuration vars
    glm::vec3	mCameraPos;
    glm::vec3	mCameraFront;
    glm::vec3	mCameraUp;
    float		mCameraSpeed;
    float		mYaw;
    float		mPitch;
    float		mFov;
    const float MAX_FOV = 45.0f;
    const float MIN_FOV = 1.0f;
    bool		mCameraFly;
    float		mGround;

    //Head
    float		mSensitivityTurn;


};



#endif //CAMERA3D_H
