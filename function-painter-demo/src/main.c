#include "tigr.h"
#include <stdio.h>
#include <math.h>

typedef struct{
    float l,r;
    float step;
    TPixel color;
    float* w;
    int s;
    float rate;
}Function;

void paint(Tigr* tigr,Function f){
    for (float x=f.l;x<=f.r;x+=f.step){
        float y = 0;
        for (int i=0;i<=f.s;i++){
            y += pow(x,i)*f.w[i];
        }
        y *= f.rate;
        float sx = x+0.5*tigr->w;
        float sy = tigr->h-(y+0.5*tigr->h);
        tigrPlot(tigr,sx,sy,f.color);
    }
}

int main(void){
    setbuf(stdout,NULL);

    int w = 400,h = 400;
    float W[] = {-2,12,-40};
    Function f = {-200,200,0.1,tigrRGB(0xff,0x00,0xff),W,2,1};

    Tigr* window = tigrWindow(w,h,"function",TIGR_FIXED);

    while (!tigrClosed(window)){
        tigrClear(window,tigrRGB(0x55,0x55,0x55));

        paint(window,f);
        
        tigrUpdate(window);
    }
    tigrFree(window);
}
