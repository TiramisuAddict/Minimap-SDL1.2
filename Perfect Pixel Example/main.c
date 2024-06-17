#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define RECT_WIDTH 50
#define RECT_HEIGHT 50

SDL_Color GetPixel(SDL_Surface* pSurface, int x, int y) {
    SDL_Color color = {255, 120, 145, 0};

    if (pSurface == NULL) {
        return color;
    }

    if (x < 0 || x >= pSurface->w || y < 0 || y >= pSurface->h) {
        return color;
    }

    Uint32 col = 0;
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);

    if (pPosition < (char*)pSurface->pixels || pPosition >= ((char*)pSurface->pixels + pSurface->pitch * pSurface->h)) {
        return color;
    }

    memcpy(&col, pPosition, pSurface->format->BytesPerPixel);
    SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b);

    return color;
}

int collisionPP(SDL_Rect player_pos, SDL_Surface *background, SDL_Color color) {
    int posX[8] = {player_pos.x, player_pos.x + player_pos.w / 2, player_pos.x + player_pos.w, player_pos.x,
                   player_pos.x, player_pos.x + player_pos.w / 2, player_pos.x + player_pos.w, player_pos.x + player_pos.w};
    int posY[8] = {player_pos.y, player_pos.y, player_pos.y, player_pos.y + player_pos.h / 2,
                   player_pos.y + player_pos.h, player_pos.y + player_pos.h, player_pos.y + player_pos.h, player_pos.y + player_pos.h / 2};

    for (int i = 0; i < 8; i++) {
        SDL_Color pixelColor = GetPixel(background, posX[i], posY[i]);
        printf("Pixel at (%d, %d): R=%d, G=%d, B=%d\n", posX[i], posY[i], pixelColor.r, pixelColor.g, pixelColor.b); // Debugging
        if (pixelColor.r == color.r && pixelColor.g == color.g && pixelColor.b == color.b) {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_SWSURFACE);
    SDL_WM_SetCaption("Rectangle Movement", NULL);

    SDL_Surface *background = IMG_Load("Game_Background.jpg");
    if (!background) {
        printf("Error loading background image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_BlitSurface(background, NULL, screen, NULL);
    SDL_Flip(screen);

    SDL_Surface *image = IMG_Load("Collision_Background.jpg");
    if (!image) {
        printf("Error loading image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Rect imageRect = {0, 0, image->w, image->h};
    SDL_BlitSurface(image, NULL, screen, &imageRect);
    SDL_Flip(screen);

    SDL_Rect rect = {WINDOW_WIDTH / 2 - RECT_WIDTH / 2, WINDOW_HEIGHT / 2 - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT};
    SDL_Rect prevRect = rect;

    SDL_Event event;
    int running = 1;
    Uint8 *keystate;
    while (running) {
	prevRect = rect; // Store the previous position

	SDL_PumpEvents(); // Update the state of the input devices
	keystate = SDL_GetKeyState(NULL); // Get the current state of the keyboard

	while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

	if (keystate[SDLK_UP]) {
	    rect.y -= 2.5;
	    if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255})) // If collision, revert to previous position
	        rect = prevRect;
	}
	if (keystate[SDLK_DOWN]) {
	    rect.y += 2.5;
	    if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255}))
	        rect = prevRect;
	}
	if (keystate[SDLK_LEFT]) {
	    rect.x -= 2.5;
	    if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255}))
	        rect = prevRect;
	}
	if (keystate[SDLK_RIGHT]) {
	    rect.x += 2.5;
	    if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255}))
	        rect = prevRect;
	}

        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255, 255, 255));

        SDL_Flip(screen);
        //SDL_Delay(20);
    }

    SDL_FreeSurface(background);
    SDL_FreeSurface(image);
    SDL_Quit();
    return 0;
}
