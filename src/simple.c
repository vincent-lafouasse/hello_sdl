#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 600
#define WIDTH 800
#define TARGET_FPS 100

#define IMG_PATH "src/crab.png"
#define IMG_WIDTH 230
#define IMG_HEIGHT 150

#define NICE_BLUE 33, 118, 174, 255

void cap_fps(uint32_t frame_beginning_tick, int target_fps);
void init_SDL(int width,
              int height,
              SDL_Window** return_window,
              SDL_Renderer** return_renderer);

int main(void)
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    init_SDL(WIDTH, HEIGHT, &window, &renderer);

    SDL_Surface* crab_surface = IMG_Load(IMG_PATH);
    SDL_Texture* crab_texture = SDL_CreateTextureFromSurface(renderer, crab_surface);
	SDL_Rect crab_rect = (SDL_Rect){69, 420, IMG_WIDTH, IMG_HEIGHT};

    SDL_Event event;
    uint32_t frame_beginning_tick;
    bool running = true;

    while (running)
	{
        frame_beginning_tick = SDL_GetTicks();
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
                break;
            }
        }

		SDL_SetRenderDrawColor(renderer, NICE_BLUE);
        SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, crab_texture, NULL, &crab_rect);

        SDL_RenderPresent(renderer);

        cap_fps(frame_beginning_tick, TARGET_FPS);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return (EXIT_SUCCESS);
}
void cap_fps(uint32_t frame_beginning_tick, int target_fps)
{
    int ms_per_frame = 1000.0 / target_fps;
    uint32_t frame_end_tick = SDL_GetTicks();
    int time_to_wait = ms_per_frame - (frame_end_tick - frame_beginning_tick);

    if (time_to_wait > 0)
    {
        SDL_Delay(time_to_wait);
    }
}

void init_SDL(int width,
              int height,
              SDL_Window** return_window,
              SDL_Renderer** return_renderer)
{
    const int SCREEN_X_POS = 0;
    const int SCREEN_Y_POS = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    *return_window = SDL_CreateWindow("a window", SCREEN_X_POS, SCREEN_Y_POS,
                                      width, height, SDL_WINDOW_OPENGL);
    if (*return_window == NULL)
    {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    *return_renderer =
        SDL_CreateRenderer(*return_window, -1, SDL_RENDERER_ACCELERATED);
    if (*return_renderer == NULL)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
