#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define RECT_WIDTH 50
#define RECT_HEIGHT 50

typedef struct {
    SDL_Surface* mini_image;
    SDL_Rect mini_position;
    SDL_Surface* player_image;
    SDL_Rect player_position;
    SDL_Rect minimap_player_position; 
    SDL_Rect camera;
} Minimap;

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
        if (pixelColor.r == color.r && pixelColor.g == color.g && pixelColor.b == color.b) {
            return 1;
        }
    }

    return 0;
}

void init_minimap(Minimap *m) {
    m->mini_image = IMG_Load("mini_background.jpg");
    if (!m->mini_image) {
        printf("Failed to load minimap background: %s\n", IMG_GetError());
        return;
    }
    m->mini_position.x = 360;
    m->mini_position.y = 0;
    m->mini_position.w = 720;
    m->mini_position.h = 120;

    m->player_image = IMG_Load("mini_player.jpg");
    if (!m->player_image) {
        printf("Failed to load minimap player image: %s\n", IMG_GetError());
        return;
    }
    m->player_position.x = 0;
    m->player_position.y = 0;
    m->minimap_player_position.x = m->mini_position.x + m->player_position.x;
    m->minimap_player_position.y = m->mini_position.y + m->player_position.y;
}

void MAJMinimap(SDL_Rect posJoueur, Minimap *m, SDL_Rect camera, int redimensionnement) {
    SDL_Rect posJoueurABS;
    posJoueurABS.x = posJoueur.x + camera.x;
    posJoueurABS.y = posJoueur.y + camera.y;

    m->minimap_player_position.x = (posJoueurABS.x * redimensionnement / 100) + m->mini_position.x;
    m->minimap_player_position.y = (posJoueurABS.y * redimensionnement / 100) + m->mini_position.y;
}

void afficher_minimap(Minimap m, SDL_Surface *screen) {
    SDL_BlitSurface(m.mini_image, NULL, screen, &m.mini_position);
    SDL_BlitSurface(m.player_image, NULL, screen, &m.minimap_player_position);
}

void liberer_minimap(Minimap *m) {
    if (m->mini_image) {
        SDL_FreeSurface(m->mini_image);
    }
    if (m->player_image) {
        SDL_FreeSurface(m->player_image);
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_SWSURFACE);
    SDL_WM_SetCaption("Minimap Testing", NULL);

    SDL_Surface *background = IMG_Load("Game_Background.jpg");
    if (!background) {
        printf("Error loading background image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Surface *image = IMG_Load("Collision_Background.jpg");
    if (!image) {
        printf("Error loading image: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Rect rect = {WINDOW_WIDTH / 2 - RECT_WIDTH / 2, WINDOW_HEIGHT / 2 - RECT_HEIGHT / 2, RECT_WIDTH, RECT_HEIGHT};
    SDL_Rect prevRect = rect;

    Minimap m;
    init_minimap(&m);

    SDL_Event event;
    int running = 1;
    Uint8 *keystate;

    while (running) {
        SDL_BlitSurface(background, NULL, screen, NULL);
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255, 255, 255));

        afficher_minimap(m, screen);

        prevRect = rect;

        SDL_PumpEvents();
        keystate = SDL_GetKeyState(NULL);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        if (keystate[SDLK_UP]) {
            rect.y -= 2.5;
            if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255})) {
                rect = prevRect;
            }
        }
        if (keystate[SDLK_DOWN]) {
            rect.y += 2.5;
            if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255})) {
                rect = prevRect;
            }
        }
        if (keystate[SDLK_LEFT]) {
            rect.x -= 2.5;
            if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255})) {
                rect = prevRect;
            }
        }
        if (keystate[SDLK_RIGHT]) {
            rect.x += 2.5;
            if (collisionPP(rect, image, (SDL_Color){0, 0, 0, 255})) {
                rect = prevRect;
            }
        }

        MAJMinimap(rect, &m, m.camera, 20);

        SDL_Flip(screen);
    }

    liberer_minimap(&m);
    SDL_FreeSurface(background);
    SDL_FreeSurface(image);
    SDL_Quit();
    return 0;
}

