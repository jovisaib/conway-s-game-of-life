#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <SDL.h>

const int blockSize = 4;
const int S_WIDTH = 480;
const int S_HEIGHT = 480;

int world [S_WIDTH/blockSize][S_HEIGHT/blockSize];

int mouseX;
int mouseY;
bool quit;
bool start;

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Event e;

int init(){
    quit = false;
    start = false;

    for(int i=0; i<S_WIDTH/blockSize; i++)
        for(int j=0; j<S_HEIGHT/blockSize; j++)
            world[i][j] = 0;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return -1;

    window = SDL_CreateWindow("Conway's Game of Life",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        S_WIDTH, S_HEIGHT,
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL || window == NULL)
        return -1;

    return 0;
}

int numNeighbours(int x,int y){
    int sum=0;
    for(int i=1; i>=-1; i--){
        for(int j=1; j>=-1; j--){
            if(i==0 && j==0)
                continue;
            if(world[x+i][y+j] == 1 || world[x+i][y+j] == 3)
                sum++;
        }
    }
    return sum;
}

void drawGrid(){
    int nBlocksW = S_WIDTH/blockSize;
    int nBlocksH = S_HEIGHT/blockSize;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);

    for(int i=0; i<nBlocksW; i++)
        SDL_RenderDrawLine(renderer, 0, blockSize*i, S_WIDTH, blockSize*i);

    for(int i=0; i<nBlocksH; i++)
        SDL_RenderDrawLine(renderer, blockSize*i, 0, blockSize*i, S_HEIGHT);
}

void drawCells(){
    for(int i=0; i<S_WIDTH/blockSize; i++){
        for(int j=0; j<S_HEIGHT/blockSize; j++){
            if(world[i][j] == 1 || world[i][j] == 3){
                SDL_Rect fillRect = {i*blockSize, j*blockSize, blockSize, blockSize};
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }
}

void render(){
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    drawCells();
    drawGrid();

    SDL_RenderPresent(renderer);
}

void update(){
    for (int x=0; x<S_WIDTH/blockSize; x++){
        for (int y=0; y<S_HEIGHT/blockSize; y++){
            if(world[x][y]==2)
                world[x][y]=1;

            if(world[x][y]==3)
                world[x][y]=0;
        }
    }

    for (int x=0; x<S_WIDTH/blockSize; x++) {
        for (int y=0; y<S_HEIGHT/blockSize; y++) {

            int num = numNeighbours(x,y);

            if (world[x][y] == 1){
                if ( num < 2 || num > 3 )
                    world[x][y] = 3; //muere
            }else{
                if (num == 3)
                    world[x][y] = 2;//nace
            }
        }
    }
}

void clean(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
	SDL_Quit();
}

void event(){
    while(SDL_PollEvent(&e) != 0){
        if(e.type == SDL_QUIT)
            quit = true;

        if (e.type == SDL_KEYDOWN){
            if(e.key.keysym.sym == SDLK_a)
                start = !start;
            if(e.key.keysym.sym == SDLK_r){
                //srand(time(NULL));
                for (int x=0; x<S_WIDTH/blockSize; x++){
                    for (int y=0; y<S_HEIGHT/blockSize; y++){
                        world[x][y] = rand()%2;
                    }
                }
            }
        }

        if( e.type == SDL_MOUSEMOTION ){
            mouseX = e.motion.x;
            mouseY = e.motion.y;
        }

        if(e.type == SDL_MOUSEBUTTONDOWN){
            if(e.button.button == SDL_BUTTON_LEFT){
                int tapX = e.button.x/blockSize;
                int tapY = e.button.y/blockSize;

                if(world[tapX][tapY] == 0)
                    world[tapX][tapY] = 1;
                else
                    world[tapX][tapY] = 0;
            }
        }
    }
}

int main(int argc, char* args[]){
    if(init()<0)
        return -1;

    while(!quit){
        event();
        if(start)
            update();
        render();
        SDL_Delay(50);
    }
    clean();

    return 0;
}
