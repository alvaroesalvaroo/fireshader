//
// Created by alv18 on 02/05/2025.
//

#include "../src/Backup/Camera3D.h"

// Math stuff
#include <cmath>
#include <bits/stl_algo.h>
#include <glad/glad.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

Camera3D::Camera3D()
{
    Camera3D(0.05f, 0.05f, 45.0f);
}

Camera3D::Camera3D(float sensitivityTurn, float cameraSpeed, float fov) {
    mSensitivityTurn = sensitivityTurn;
    mCameraSpeed = cameraSpeed;
    mFov = fov;
    mYaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    mPitch = 0.0f;

    mCameraFly = true;
    mGround = 0.0f;

    //Init Camera vectors
    mCameraPos = glm::vec3(0.0f, 2.0f, 3.0f);
    mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Default screen size (will be corrected if setScreenSize is called after constructor)
    mScreenHeight = 600;
    mScreenWidth = 800;
}

Camera3D::~Camera3D() = default;

void Camera3D::setScreenSize(int width, int height) {
    mScreenWidth = width;
    mScreenHeight = height;
}
void Camera3D::updatePosition(bool *moveArray) { // forward(w)-backward(s)-left(a)-right(d)-up(e)-down(q)
    if (moveArray[0])	//forward
    {
        mCameraPos += mCameraSpeed * mCameraFront;
    }
    if (moveArray[1])	//backward
    {
        mCameraPos -= mCameraSpeed * mCameraFront;
    }
    if (moveArray[2]) // left
    {
        mCameraPos -= glm::normalize(glm::cross(mCameraFront, mCameraUp)) * mCameraSpeed;
    }
    if (moveArray[3])	//right
    {
        mCameraPos += glm::normalize(glm::cross(mCameraFront, mCameraUp)) * mCameraSpeed;
    }
    if (moveArray[4]) { // up
        mCameraPos += mCameraSpeed * mCameraUp;
    }
    if (moveArray[5]) { // down
        mCameraPos -= mCameraSpeed * mCameraUp;
    }

    // TODO: remove this.
    // No fly goes to ground
    if (!mCameraFly) {
        mCameraPos.y = mGround;
    }
}

void Camera3D::turn(float xTurn, float yTurn) {

    xTurn *= mSensitivityTurn;
    yTurn *= mSensitivityTurn;

    mYaw += xTurn;
    mPitch += yTurn;
    /* Do not clamp
    if (mPitch > 89.0f)
        mPitch = 89.0f;
    if (mPitch < -89.0f)
        mPitch = -89.0f;
        */

    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mCameraFront = glm::normalize(front);

}


void Camera3D::setPosition(float X, float Y, float Z)
{
    mCameraPos = glm::vec3(X, Y, Z);
}

void Camera3D::setPosition(glm::vec3 position) {
    mCameraPos = position;
}

glm::vec3 Camera3D::getPosition() {
    return mCameraPos;
}

void Camera3D::setCameraFront(float X, float Y, float Z)
{
    mCameraFront = glm::vec3(X, Y, Z);
}

void Camera3D::setCameraFront(glm::vec3 front) {
    mCameraFront = front;
}

void Camera3D::setCameraUp(float X, float Y, float Z)
{
    mCameraUp = glm::vec3(X, Y, Z);
}

void Camera3D::setCameraUp(glm::vec3 up) {
    mCameraUp = up;
}

void Camera3D::setZoom(float fov) {
    mFov = glm::clamp(fov, MAX_FOV, MIN_FOV);
}

void Camera3D::updateViewUniform(int uniformID) {
    mViewMatrix = glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);
    glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
}


void Camera3D::updateProjectionUniform(int uniformID) {
    float screenWidth = static_cast<float>(mScreenWidth);
    float screenHeight = static_cast<float>(mScreenHeight);
    mProjectionMatrix = glm::perspective(glm::radians(mFov), screenWidth / screenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
}



