#ifndef FONCTION_H
#define FONCTION_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_H 600
#define SCREEN_W 900

typedef struct {
    SDL_Surface* mini_image;
    SDL_Rect mini_position;
    SDL_Surface* player_image;
    SDL_Rect player_position;
    SDL_Rect minimap_player_position; 
    SDL_Rect camera;

} Minimap;

typedef struct {
    int position_x;
    int position_y;
    
    int camera_x;
    int camera_y;
    
} mySave;

void init_minimap(Minimap* m);
void MAJMinimap(SDL_Rect posJoueur,  Minimap * m, SDL_Rect camera, int redimensionnement);
void afficher_minimap(Minimap m, SDL_Surface* screen);
void liberer_minimap(Minimap* m);

void affichertempsen(Uint32 startTime);
void affichertemps(int temps);

SDL_Color GetPixel(SDL_Surface* pSurface, int x, int y);
int collisionPP(SDL_Rect player_pos, SDL_Surface *background, SDL_Color color);

void sauvegarder (Personne p, background b, char * nomfichier);
void charger(Personne * p, background *b, char * nomfichier);

#endif
