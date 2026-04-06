/*******************************************************************
** This code has small modifications from Breakout by Joey de Vries
******************************************************************/
#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "Rendering/stb_image.h"

// Instantiate static variables
std::map<TextureName, Texture2D>    ResourceManager::Textures;
std::map<ShaderName, Shader>       ResourceManager::Shaders;


Shader &ResourceManager::LoadShader(ShaderName name)
{
    switch (name) {
        case ShaderName::Sprite:
            Shaders[name] = loadShaderFromFile("assets/shaders/sprite.vs", "assets/shaders/sprite.fs", nullptr);
            return Shaders[name];
        case ShaderName::Particle:
            Shaders[name] = loadShaderFromFile("assets/shaders/particle.vs", "assets/shaders/particle.fs", nullptr);
            return Shaders[name];
        case ShaderName::PostProcess:
            Shaders[name] = loadShaderFromFile("assets/shaders/postprocess.vs", "assets/shaders/postprocess.fs", nullptr);
            return Shaders[name];
        case ShaderName::Text:
            Shaders[name] = loadShaderFromFile("assets/shaders/text_2d.vs", "assets/shaders/text_2d.fs", nullptr);
            return Shaders[name];

        default:
            return Shaders[name];
    }
}

Shader ResourceManager::GetShader(ShaderName name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(TextureName name)
{
    switch (name) {

        case TextureName::Block:
            Textures[name] = loadTextureFromFile("assets/textures/block.png", false);
            return Textures[name];
        case TextureName::BlockSolid:
            Textures[name] = loadTextureFromFile("assets/textures/block_solid.png", false);
            return Textures[name];
        case TextureName::Background:
            Textures[name] = loadTextureFromFile("assets/textures/background.jpg", false);
            return Textures[name];
        case TextureName::Paddle:
            Textures[name] = loadTextureFromFile("assets/textures/paddle.png", true);
            return Textures[name];
        case TextureName::VerticalPaddle:
            Textures[name] = loadTextureFromFile("assets/textures/paddle-vertical.png", true);
            return Textures[name];
        case TextureName::Ball:
            Textures[name] = loadTextureFromFile("assets/textures/awesomeface.png", true);
            return Textures[name];
        case TextureName::Particle:
            Textures[name] = loadTextureFromFile("assets/textures/particle.png", true);
            return Textures[name];
        case TextureName::PowerSpeed:
            Textures[name] = loadTextureFromFile("assets/textures/powerup_speed.png", true);
            return Textures[name];
        case TextureName::PowerSticky:
            Textures[name] = loadTextureFromFile("assets/textures/powerup_sticky.png", true);
            return Textures[name];
        case TextureName::PowerIncrease:
            Textures[name] = loadTextureFromFile("assets/textures/powerup_increase.png", true);
            return Textures[name];
        case TextureName::PowerConfuse:
            Textures[name] = loadTextureFromFile("assets/textures/powerup_confuse.png", true);
            return Textures[name];
        case TextureName::PowerChaos:
            Textures[name] = loadTextureFromFile("assets/textures/powerup_chaos.png", true);
            return Textures[name];
        case TextureName::PowerPassthrough:
            Textures[name] = loadTextureFromFile("assets/textures/powerup_passthrough.png", true);
            return Textures[name];
        case TextureName::Arrows:
            Textures[name] = loadTextureFromFile("assets/textures/arrows.png", true);
            return Textures[name];
        case TextureName::Game1P:
            Textures[name] = loadTextureFromFile("assets/textures/Game1P.png", true);
            return Textures[name];
        case TextureName::Game2P:
            Textures[name] = loadTextureFromFile("assets/textures/Game2P.png", true);
            return Textures[name];

        default:
            return Textures[name];
    }
}

Texture2D &ResourceManager::GetTexture(TextureName name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders
    for (auto pair : Shaders)
        glDeleteProgram(pair.second.ID);
    // (properly) delete all textures
    for (auto pair : Textures)
        glDeleteTextures(1, &pair.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        std::cerr<<"Error loading texture from " << file << ". Data is null" << std::endl;
    }
    // now generate texture
    texture.Generate(width, height, data);
    if (texture.ID <= 0) {
        std::cerr<<"Error. Texture loaded from " << file << " has 0 or less ID " << std::endl;
    }
    // and finally free image data
    stbi_image_free(data);

    return texture;
}