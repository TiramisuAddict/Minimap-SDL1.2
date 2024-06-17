void init_minimap(Minimap *m) {

    m->mini_image = IMG_Load("minimap_image.jpg");
    m->mini_position.x = 90;
    m->mini_position.y = 0;
    m->mini_position.w = 720;
    m->mini_position.h = 120;
    m->camera.x = 0;
    m->camera.y = 0;
    m->camera.w = 720;
    m->camera.h = 120;

    m->player_image = IMG_Load("player_image.png");
    m->player_position.x = 0;
    m->player_position.y = 0;
    m->minimap_player_position.x = m->mini_position.x + m->player_position.x;
    m->minimap_player_position.y = m->mini_position.y + m->player_position.y;
}

void MAJMinimap(SDL_Rect posJoueur, Minimap *m, SDL_Rect camera, int redimensionnement) {
    SDL_Rect posJoueurABS;
    posJoueurABS.x = 0;
    posJoueurABS.y = 0;

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

    SDL_FreeSurface(m->mini_image);
    SDL_FreeSurface(m->player_image);
}

void affichertemps(int temps) {
    SDL_Color color_temp = {0, 0, 0};
    TTF_Font *police_time = NULL;
    police_time = TTF_OpenFont("your_font.ttf", 24);

    char temp[100];
    int heures = temps / 3600;
    int minutes = (temps % 3600) / 60;
    int secondes = temps % 60;

    sprintf(temp, "%02d:%02d:%02d", heures, minutes, secondes);
    
    SDL_Surface *temps_surface = TTF_RenderText_Solid(police_time, temp, color_temp);
    SDL_Rect pos_temp;
    pos_temp.x = 600;
    pos_temp.y = 0;
    SDL_BlitSurface(temps_surface, NULL, SDL_GetVideoSurface(), &pos_temp);
    TTF_CloseFont(police_time);
    SDL_FreeSurface(temps_surface);
}

void affichertempsen(Uint32 startTime) {
    SDL_Color color_temp = {0, 0, 0};
    TTF_Font *police_time = NULL;

    police_time = TTF_OpenFont("your_font.ttf", 24);

    char temp[100];
    int temps = 60000 - (SDL_GetTicks() - startTime);
    if (temps < 0){
        temps = 0;
    }

    int secondes = temps / 1000;
    sprintf(temp, "%02d:%02d", 0, secondes);

    SDL_Surface *temps_surface = NULL;
    temps_surface = TTF_RenderText_Solid(police_time, temp, color_temp);
    SDL_Rect pos_temp;
    pos_temp.x = 20;
    pos_temp.y = 20;

    SDL_BlitSurface(temps_surface, NULL, SDL_GetVideoSurface(), &pos_temp);

    TTF_CloseFont(police_time);
    SDL_FreeSurface(temps_surface);
}

SDL_Color GetPixel(SDL_Surface* pSurface, int x, int y) {
    SDL_Color color = {0, 0, 0, 0};

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

int collisionPP(SDL_Rect player_pos, SDL_Surface *background, SDL_Color color){

    int posX[8] = {player_pos.x, player_pos.x + player_pos.w / 2, player_pos.x + player_pos.w, player_pos.x, player_pos.x, player_pos.x + player_pos.w / 2, player_pos.x + player_pos.w, player_pos.x + player_pos.w};
    int posY[8] = {player_pos.y, player_pos.y, player_pos.y, player_pos.y + player_pos.h / 2, player_pos.y + player_pos.h, player_pos.y + player_pos.h, player_pos.y + player_pos.h, player_pos.y + player_pos.h / 2};

    for (int i = 0; i < 8; i++)
    {
        SDL_Color pixelColor = GetPixel(background, posX[i], posY[i]);
        if (pixelColor.r == color.r && pixelColor.g == color.g && pixelColor.b == color.b){
            return 1;
        }
    }

    return 0;
}

void sauvegarder(Personne p, background b, char * nomfichier) {
    mySave newSave;
    FILE *f;

    newSave.position_x = p.position.x;
    newSave.position_y = p.position.y;

    newSave.camera_x = b.camera2.x;
    newSave.camera_y = b.camera2.y;

    f = fopen(nomfichier,"w");

    fprintf(f, "%d %d %d %d\n", newSave.position_x, newSave.position_y, newSave.camera_x, newSave.camera_y);

    fclose(f);
}

void charger(Personne * p, background *b , char * nomfichier) {
    mySave loadedData;
    FILE *f;

    f = fopen(nomfichier,"r");

    fscanf(f, "%d %d %d %d\n", &loadedData.position_x, &loadedData.position_y, &loadedData.camera_x, &loadedData.camera_y);

    p->position.x = loadedData.position_x;
    p->position.y = loadedData.position_y;

    b->camera2.x = loadedData.camera_x;
    b->camera2.y = loadedData.camera_y;

    fclose(f);
}