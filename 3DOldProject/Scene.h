//
// Created by alv18 on 28/04/2025.
// The goal is to make scene independent of GLFW or SDL (not of glad)


#ifndef SCENE_H
#define SCENE_H
#include "../src/Camera3D.h"
inline Camera3D* gCamera = new Camera3D(0.05f, 0.001f, 45.0f);

void updateGeometry(double time, double deltaTime);
void render(double time, double deltaTime);

void initScene(int screenWidth, int screenHeight);

#endif //GEOMETRY_H
