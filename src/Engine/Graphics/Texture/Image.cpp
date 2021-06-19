#include "Engine/Graphics/Texture/Image.h"
#include "Engine/Graphics/Texture/ImageLoader.h"
#include "Engine/Core.h"

Image::Image(const std::string& filepath)
    : filepath(filepath) 
{ }

Image::~Image()
{
    if (dataLoaded)
        UnloadData();
}

unsigned char* Image::LoadData()
{
    dataPointer = ImageLoader::LoadImageData(filepath.c_str(), &width, &height, &bytesPerPixel);
    if (!dataPointer)
        Console::LogError("Warning! Image data failed to load from filepath '" + filepath + "'.");
    dataLoaded = true;
    return dataPointer;
}

void Image::UnloadData()
{
    ImageLoader::ClearImageData(dataPointer);
    dataLoaded = false;
}
