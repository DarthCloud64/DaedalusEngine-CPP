#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include <bgfx/bgfx.h>
#include <iostream>

bgfx::ShaderHandle createShaderHandle(std::string filePath);
bgfx::ProgramHandle createProgram(std::string vertexShaderPath, std::string fragmentShaderPath);

#endif