//
// Created by Quinn on 28/09/2022.
//

#include "ResourceHandler.h"
/*
sf::Texture& ResourceHandler::getTextureRef(const std::string& name)
{
  if (textures.count(name) == 1)
  {
    return *textures[name];
  }

  return *textures["NULL_TEXTURE.png"];
}

sf::Font& ResourceHandler::getFontRef(const std::string& name)
{
  if (fonts.count(name) == 1)
  {
    return *fonts[name];
  }

  return *fonts["OpenSans-Bold.ttf"];
}

ResourceHandler::ResourceHandler(const std::string& assetDirectory)
{
  std::string filename;
  std::string texturePath = assetDirectory + "/Images/";
  std::string fontPath = assetDirectory + "/Fonts/";

  //Texture Loading
  for (const auto& dirEntry : recursive_directory_iterator(texturePath))
  {
    filename = dirEntry.path().filename().string();
    sf::Texture txtPtr;
    if(txtPtr.loadFromFile(texturePath + filename))
    {
      std::cout << "Loaded " + filename  << std::endl;
      textures[filename] = std::make_unique<sf::Texture>(txtPtr);
    }
    else
    {
      std::cout << "Failed to Load " + texturePath  << std::endl;
    }
  }

  //Font loading
  for (const auto& dirEntry : recursive_directory_iterator(fontPath))
  {
    filename = dirEntry.path().filename().string();
    sf::Font fntPtr;
    if (fntPtr.loadFromFile(fontPath + filename))
    {
      std::cout << "Loaded " + filename << std::endl;
      fonts[filename] = std::make_unique<sf::Font>(fntPtr);
    }
    else
    {
      std::cout << "Failed to Load " + filename << std::endl;
    }
  }

  std::cout << "Loading Complete" << std::endl;
}
*/