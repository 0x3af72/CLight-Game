#include <iostream>
#include <unordered_map>

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"

#pragma once

std::unordered_map<std::string, SDL_Texture*> cached_textures;

// Private function for loading a texture.
SDL_Texture* _loadTexture(SDL_Renderer* renderer, const char* path){

    SDL_Surface* temp_surface = IMG_Load(path);

    if (!temp_surface){
        std::cout << "Unable to load image: " << SDL_GetError() << "\n";
    }

    SDL_Texture* loaded_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    SDL_FreeSurface(temp_surface);
    return loaded_texture;

}

// Load a texture from an image path or from cache and cache it.
SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string image_path){
    
    // check if image has been cached
    if (cached_textures.find(image_path) != cached_textures.end()){
        return cached_textures[image_path];
    } else {

        // load texture
        SDL_Texture* returned_texture = _loadTexture(renderer, image_path.c_str());
        SDL_SetTextureBlendMode(returned_texture, SDL_BLENDMODE_BLEND);

        // cache texture
        cached_textures[image_path] = returned_texture;

        return returned_texture;

    }

}

// Center a SDL_Rect.
void centerRect(SDL_Rect& rect){
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
}