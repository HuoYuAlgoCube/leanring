#include <stdio.h>
#include <stdlib.h>
#include "tigr.h"

void convolve(Tigr* dest,Tigr* src,float kernel[],int dx[],int dy[],int size){
    for (int x=0;x<src->w;x++){
        for (int y=0;y<src->h;y++){
            float r = 0.f,g = 0.f,b = 0.f;
            for (int i=0;i<size*size;i++){
                int sx = x+dx[i];
                int sy = y+dy[i];
                if (sx < 0) sx = 0;
                if (sx >= src->w) sx = src->w - 1;
                if (sy < 0) sy = 0;
                if (sy >= src->h) sy = src->h - 1;
                TPixel p = src->pix[sy*src->w+sx];
                float k = kernel[i];

                r += p.r*k;
                g += p.g*k;
                b += p.b*k;
            }
            TPixel out;
            out.r = (unsigned char)(r < 0 ? 0 : (r > 255 ? 255 : r));
            out.g = (unsigned char)(g < 0 ? 0 : (g > 255 ? 255 : g));
            out.b = (unsigned char)(b < 0 ? 0 : (b > 255 ? 255 : b));

            dest->pix[y * dest->w + x] = out;
        }
    }
}

int main(void){

    printf("Hello Neuron NetWork!\n\n");

    Tigr* image = tigrLoadImage("res/lfh.png");
    Tigr *screen = tigrWindow(800,800, "TIGR Text & Image", 0);
    Tigr* image2 = tigrBitmap(800,800);
    float k[9] = {
        0, 1, 0,
       -1, 5,-1,
        0, 1, 0,
    };
    int dx[] = {-1,-1,-1, 0, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1,-1, 0, 1,-1, 0, 1};
    for (int i=0;i<20;i++){
        convolve(image2,image,k,dx,dy,3);
    }
    while (!tigrClosed(screen)){
        tigrClear(screen, tigrRGB(30, 30, 30));
        tigrBlit(screen,image2,0,0,0,0,800,800);
        tigrUpdate(screen);
    }
    tigrFree(image);
    tigrFree(image2);
    tigrFree(screen);
    return 0;
}
