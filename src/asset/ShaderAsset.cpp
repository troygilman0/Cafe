#include "Asset.h"
#include "../util/StringUtils.h"
#include <iostream>

UUID Asset::loadShaderAsset(AssetPack& assetPack, const char* vertexFilePath, const char* fragmentFilePath) {
    // Load the shader files as strings
    const char* vertexShaderAsString = StringUtils::loadFileAsString(vertexFilePath);
    const char* fragmentShaderAsString = StringUtils::loadFileAsString(fragmentFilePath);

    // Check if the shader files were loaded successfully
    if (vertexShaderAsString == nullptr || fragmentShaderAsString == nullptr) {
        std::cout << "Failed to load shader files\n";
        return false;
    }

    // Create the shader program
    ui32 programId = glCreateProgram();
    ui32 vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    ui32 fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile the vertex shader
    glShaderSource(vertexShaderId, 1, &vertexShaderAsString, nullptr);
    glCompileShader(vertexShaderId);

    // Check if the vertex shader compiled successfully
    i32 vertexCompileStatus;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &vertexCompileStatus);
    if (vertexCompileStatus == GL_FALSE) {
        std::cout << "Failed to compile vertex shader\n";
        return false;
    }

    // Compile the fragment shader
    glShaderSource(fragmentShaderId, 1, &fragmentShaderAsString, nullptr);
    glCompileShader(fragmentShaderId);

    // Check if the fragment shader compiled successfully
    i32 fragmentCompileStatus;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &fragmentCompileStatus);
    if (fragmentCompileStatus == GL_FALSE) {
        std::cout << "Failed to compile fragment shader\n";
        return false;
    }

    // Link the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glBindAttribLocation(programId, 0, "position");
    glBindAttribLocation(programId, 1, "textureCoord");
    glBindAttribLocation(programId, 2, "normal");
    glLinkProgram(programId);

    // Check if the shader program linked successfully
    i32 programLinkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &programLinkStatus);
    if (programLinkStatus == GL_FALSE) {
        std::cout << "Failed to link shader program\n";
        return false;
    }

    //  Create the shader asset
    ShaderAsset* asset = new ShaderAsset();

    // Set the shader uniform locations
    for (ShaderUniform uniform = (ShaderUniform) 0; uniform < shaderUniformNames.size(); uniform = (ShaderUniform)(uniform + 1)) {
        const char* uniformName = shaderUniformNames[uniform];
        int uniformLocation = glGetUniformLocation(programId, uniformName);
        asset->uniformLocations[uniform] = uniformLocation;
    }

    // Set the shader asset properties
    StringUtils::copyStringToBuffer(asset->vertexFilePath, vertexFilePath, CHAR_BUFFER_SIZE);
    StringUtils::copyStringToBuffer(asset->fragmentFilePath, fragmentFilePath, CHAR_BUFFER_SIZE);
    asset->programId = programId;
    asset->vertexShaderId = vertexShaderId;
    asset->fragmentShaderId = fragmentShaderId;

    // Add the shader asset to the asset pack
    UUID assetId = assetPack.nextShaderAssetId;
    asset->assetId = assetId;
    assetPack.shaderAssets[assetId] = asset;
    assetPack.numShaderAssets++;
    assetPack.nextShaderAssetId++;
    return assetId;
}