#include <stdio.h>
#include <stdlib.h>
#include <SDL3\SDL.h>
#define MAX_COL 16
#define MAX_ROW 16

SDL_Surface *sprite[MAX_COL] = {NULL};
SDL_Surface *sprite_sheet = NULL;
int max_width = 0;
int max_heigth = 0;

void loadsprites(const char* directory, const size_t row, const size_t start, const size_t stop)
{
    char filename[80];

    for (size_t i = 0; i <= stop-start; i++) {
        unsigned int file = i + start;
        sprintf (filename, "%s%04u.png", directory, file);        
        printf ("Loading %s\n", filename);
        sprite[i] = SDL_LoadPNG(filename);
        if (sprite[i] == NULL) {
            printf ("Erro loading %s\n", filename);
            break;
        }
        if (sprite[i]->w > max_width) max_width = sprite[i]->w;
        if (sprite[i]->h > max_heigth) max_heigth = sprite[i]->h;
    }
}

int main(int argc, char *argv[]) {

    if (argc !=6 ){
        printf ("Usage: input_directory row starting_number ending_number output.png\n");
        return 0;
    }

    int row = atoi(argv[2]);
    int start = atoi(argv[3]);
    int stop = atoi(argv[4]);
    char* output = argv[5];

    if (start > stop) {
        printf ("Erro: stop must be equal o greater than start\n");
        return -1;
    }


    loadsprites(argv[1], row, start,stop); 
    sprite_sheet = SDL_LoadPNG(output);
    
    if (sprite_sheet == NULL) {
        sprite_sheet = SDL_CreateSurface(MAX_COL*max_width, MAX_ROW*max_heigth, SDL_PIXELFORMAT_RGBA8888);
    }

    if (sprite_sheet == NULL) {
        printf ("Erro creating sprite sheet\n");
    } else
    {
        printf ("Generating sprite sheet\n");
        for (size_t j = 0; j <= stop-start; j++) {
            SDL_Rect dst;
            dst.x = j*max_width;
            dst.y = row*max_heigth;   
            if ((sprite[j] != NULL) && (!SDL_BlitSurface(sprite[j], NULL, sprite_sheet,&dst))) {
                    printf ("Error generating sprite sheet: %s \n", SDL_GetError());
            }                       
        }
    }

    if (!SDL_SavePNG(sprite_sheet, output)) {
        printf ("Error saving sprite sheet: %s \n", SDL_GetError());
    }

    for (size_t j=0; j <=stop-start; j++) {
        if (sprite[j]  != NULL) {
            SDL_DestroySurface(sprite[j]);
        }        
    }

    if (sprite_sheet != NULL) {
        SDL_DestroySurface(sprite_sheet);
    }
    printf ("Sprite sheet successful created!\n");
    return 0;
}
