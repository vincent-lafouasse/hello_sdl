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

#define SQUARE_SIZE 50

#define NICE_BLUE 33, 118, 174, 255
#define LILAC 226, 207, 234, 255

void cap_fps(uint32_t frame_beginning_tick, int target_fps);
void init_SDL(int width,
              int height,
              SDL_Window** return_window,
              SDL_Renderer** return_renderer);
void move_square(SDL_Rect* square);
bool mouse_is_in_rect(SDL_Rect* rect);

int main(void)
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    init_SDL(WIDTH, HEIGHT, &window, &renderer);

    SDL_Surface* crab_surface = IMG_Load(IMG_PATH);
    SDL_Texture* crab_texture = SDL_CreateTextureFromSurface(renderer, crab_surface);
	SDL_Rect crab_rect = (SDL_Rect){69, 420, IMG_WIDTH, IMG_HEIGHT};
	bool render_crab = true;

	SDL_Rect square = (SDL_Rect){0, 0, SQUARE_SIZE, SQUARE_SIZE};

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
			if (event.type == SDL_MOUSEBUTTONUP && mouse_is_in_rect(&crab_rect))
			{
				render_crab = !render_crab;
			}
        }

		SDL_SetRenderDrawColor(renderer, NICE_BLUE);
        SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, LILAC);
		move_square(&square);
		SDL_RenderFillRect(renderer, &square);

		if (render_crab)
			SDL_RenderCopy(renderer, crab_texture, NULL, &crab_rect);

        SDL_RenderPresent(renderer);

        cap_fps(frame_beginning_tick, TARGET_FPS);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return (EXIT_SUCCESS);
}

bool mouse_is_in_rect(SDL_Rect* rect)
{
	int x;
	int y;
	SDL_GetMouseState(&x, &y);

	bool x_is_in = (x >= rect->x && x <= (rect->x + rect->w));
	bool y_is_in = (y >= rect->y && y <= (rect->y + rect->h));
	return x_is_in && y_is_in;
}

void move_square(SDL_Rect* square)
{
	static uint32_t last_move_time = 0;

	if (SDL_GetTicks() - last_move_time > 10)
	{
		last_move_time = SDL_GetTicks();
		square->x += 1;
		square->y += 1;
	}
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
