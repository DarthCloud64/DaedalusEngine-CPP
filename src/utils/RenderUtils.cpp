#include "../../include/utils/RenderUtils.h"
#include <fstream>

bgfx::ShaderHandle createShaderHandle(std::string filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    std::cout << filePath << std::endl;
    if (!file.is_open())
    {
        printf("whoops\n");
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    const bgfx::Memory *memory = bgfx::alloc(size);
    file.read((char *)memory->data, size);

    return bgfx::createShader(memory);
}

bgfx::ProgramHandle createProgram(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    bgfx::ShaderHandle vertexShaderHandle = createShaderHandle(vertexShaderPath);
    bgfx::ShaderHandle fragmentShaderHandle = createShaderHandle(fragmentShaderPath);

    return bgfx::createProgram(vertexShaderHandle, fragmentShaderHandle, true);
}
