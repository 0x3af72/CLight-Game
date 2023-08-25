#include <iostream>
#include <vector>
#include <string>

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"

#include "vector_functions.hpp"
#include "vector.hpp"
#include "sdl_functions.hpp"
#include "line.hpp"
#include "line_functions.hpp"
#include "load_playground.hpp"

int main(int argc, char* argv[]){
    
    // setup sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("CLight Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // fps capping
    const int FPS = 60  ;
    const float FRAME_DELAY = 1000 / FPS;
    Uint32 framestart;
    int frametime;

    // game state
    bool running = true;

    // line creation
    bool creating_line = false;
    Vector first_point;
    Vector second_point;

    // torch
    SDL_Texture* torch_texture = loadTexture(renderer, "assets/torch.png");
    int torch_x = 300;
    int torch_y = 550;
    int torch_angle;
    SDL_Rect torch_rect;

    // load playground
    std::string playground_name;
    std::cout << "Enter playground name (Enter nothing for empty playground): ";
    std::getline(std::cin, playground_name);
    playground_name = (playground_name.length()) ? "playgrounds/" + playground_name + ".plg" : "playgrounds/_EMPTY_PLAYGROUND.plg";
    std::cout << "Loading " << playground_name << "\n";

    // line pixels and normals
    std::vector<Line> lines = loadPlayground(playground_name);

    // light rays
    Vector light_vel;
    std::vector<std::pair<Vector, Vector>> light_rays;
    light_rays.reserve(2000); // reserve space

    // torch states
    bool torch_on = true;
    bool torch_edge_bounce = false;

    while(running){

        // get mouse position
        int mousex, mousey;
        SDL_GetMouseState(&mousex, &mousey);

        // torch angle
        torch_angle = angle(torch_x, torch_y, mousex, mousey) + 90;

        // torch position
        torch_rect = {torch_x, torch_y, 40, 75};
        centerRect(torch_rect);

        // torch movement
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_W]){
            torch_y -= 2 * (torch_y > 0);
        }
        if (keystates[SDL_SCANCODE_S]){
            torch_y += 2 * (torch_y < 600);
        }
        if (keystates[SDL_SCANCODE_A]){
            torch_x -= 2 * (torch_x > 0);
        }
        if (keystates[SDL_SCANCODE_D]){
            torch_x += 2 * (torch_x < 600);
        }

        // sdl events
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0){;
            if (event.type == SDL_QUIT){
                running = false;
            } else if (event.type == SDL_KEYDOWN){

                SDL_Keycode key = event.key.keysym.sym;

                if (key == SDLK_g){

                    // check if creating line
                    if (creating_line){
                        second_point = Vector(mousex, mousey);
                        lines.push_back(Line(first_point, second_point));
                        creating_line = false;
                    } else {
                        first_point = Vector(mousex, mousey);
                        creating_line = true;
                    }

                } else if (key == SDLK_h){
                    // switch for torch
                    torch_on = (!torch_on);
                } else if (key == SDLK_f){
                    torch_edge_bounce = (!torch_edge_bounce);
                }

            }

        }

        // clear light rays
        light_rays.clear();

        // original velocity of light
        float light_rad = atan2(mousey - torch_y, mousex - torch_x);
        light_vel = {float(cos(light_rad)), float(sin(light_rad))};

        // calculate all pixels that light covers and reflect
        light_rays.push_back(std::make_pair(Vector(torch_x, torch_y), Vector(torch_x, torch_y)));

        // only trace ray if torch is on
        for (int i = 0; i != torch_on * (100000 * (!torch_edge_bounce) + 3000); i++){
            
            // last ray
            Vector &last_ray = light_rays[light_rays.size()-1].second;

            last_ray.x += light_vel.x;
            last_ray.y += light_vel.y;

            // check for collision with any line
            Line hit_line = inLine(last_ray, lines);
            if (!hit_line.empty){

                // make new pair of points
                light_vel = calculateReflection(light_vel, lineNormal(hit_line.point1, hit_line.point2));
                light_rays.push_back(std::make_pair(Vector(last_ray.x, last_ray.y), Vector(last_ray.x, last_ray.y)));

            }

            // check if light is out of screen
            if (last_ray.x < 0 || last_ray.x > 600){

                if (torch_edge_bounce){
                    light_vel.x *= -1;
                    light_rays.push_back(std::make_pair(Vector(last_ray.x, last_ray.y), Vector(last_ray.x, last_ray.y)));
                } else {
                    break;
                }

            }

            if (last_ray.y < 0 || last_ray.y > 600){

                if (torch_edge_bounce){
                    light_vel.y *= -1;
                    light_rays.push_back(std::make_pair(Vector(last_ray.x, last_ray.y), Vector(last_ray.x, last_ray.y)));
                } else {
                    break;
                }

            }

        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // render all created lines
        for (Line line: lines){
            SDL_RenderDrawLine(renderer, line.point1.x, line.point1.y, line.point2.x, line.point2.y);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // render light if torch is on
        if (torch_on){
            for (auto [point1, point2]: light_rays){
                SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
            }
        }

        // show torch
        SDL_RenderCopyEx(renderer, torch_texture, NULL, &torch_rect, torch_angle, NULL, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);

        // cap fps
        frametime = SDL_GetTicks() - framestart;
        if (FRAME_DELAY > frametime){
            SDL_Delay((FRAME_DELAY - frametime) * (FRAME_DELAY - frametime < 50));
        } else {
            std::cout << "lagging: " << light_rays.size() << " rays\n";
        }
        framestart = SDL_GetTicks();

    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();

    return 0;

}