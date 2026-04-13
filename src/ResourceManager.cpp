/*******************************************************************
** This code has small modifications from Breakout by Joey de Vries
******************************************************************/
#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader &ResourceManager::LoadShader(std::string name)
{
    // First, checkout if is already loaded
    auto it = Shaders.find(name);
    if (it != Shaders.end()) {
        return it->second; // it->second es el objeto Shader
    }
    std::cout << "Loading shaders: " << name << std::endl;

    std::string vPathStr = "shaders/" + name + ".vs";
    std::string fPathStr = "shaders/" + name + ".fs";

    // Cast String -> const char*
    const char* vertexPath = vPathStr.c_str();
    const char* fragmentPath = fPathStr.c_str();
    Shaders[name] = loadShaderFromFile(vertexPath, fragmentPath, nullptr);
    Shaders[name].mName = name.c_str();
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "Error just after loading shader: " << name << std::endl;
    }
    return Shaders[name];
}

Shader &ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D &ResourceManager::LoadTexture(const char* name, const char* path)
{
    auto it = Textures.find(name);
    if (it != Textures.end()) {
        return it->second; // it->second es el objeto Textures
    }
    Textures[name] = loadTextureFromFile(path, false);
    return Textures[name];
}

Texture2D &ResourceManager::GetTexture(const char* name)
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

Shader ResourceManager::loadShaderFromFile(std::string vShaderFile, std::string fShaderFile, const char *gShaderFile)
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
        if (vertexCode.empty()) {
            std::cerr << "ERROR: " << vShaderFile << " no se ha podido cargar" << std::endl;
        }
        // if geometry shader path is present, also load a geometry shader
        // if (gShaderFile != nullptr)
        // {
        //     std::ifstream geometryShaderFile(gShaderFile); // ifstream acepta const char*
        //     std::stringstream gShaderStream;
        //     gShaderStream << geometryShaderFile.rdbuf();
        //     geometryShaderFile.close();
        //     geometryCode = gShaderStream.str();
        // }
    }
    catch (std::exception &e)
    {
        std::cerr << "ERROR::SHADER: Failed to read shader files " << e.what() << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *path, bool alpha)
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
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        std::cerr<<"Error loading texture from " << path << ". Data is null" << std::endl;
    }
    // now generate texture
    texture.Generate(width, height, data);
    if (texture.ID <= 0) {
        std::cerr<<"Error. Texture loaded from " << path << " has 0 or less ID " << std::endl;
    }
    // and finally free image data
    stbi_image_free(data);

    return texture;
}