#include"raylib.h"
#include"game.h"
#include"player.h"
#include<stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define FPS 60
#define BGCOLOR (Color){100,100,100,255}
#define TITLE "stickfigure"

int main(void){
    setbuf(stdout,NULL);
    InitWindow(WIDTH,HEIGHT,TITLE);
    
    Texture2D background = LoadTexture("resources/background.png");    
    
    SetTargetFPS(FPS);
    
    initTco();
    initBullet();
    
    Player player = getTco();
    Player enermy = getTco();
    enermy.box.x = 600;
    enermy.direction = 0;
    enermy.isPlayer = 0;

    while(!WindowShouldClose()){

        if (player.anims[player.curAnim].cancelAnim == STATE_STAND){
            player.curAnim = STATE_STAND;
        }

        enermy.curAnim = STATE_STAND;

        if (IsKeyDown(KEY_A)){
            player.curAnim = STATE_RUN;
            player.direction = 0;
        }

        if (IsKeyDown(KEY_D)){
            player.curAnim = STATE_RUN;
            player.direction = 1;
        }

        if (IsKeyDown(KEY_LEFT)){
            enermy.curAnim = STATE_RUN;
            enermy.direction = 0;
        }

        if (IsKeyDown(KEY_RIGHT)){
            enermy.curAnim = STATE_RUN;
            enermy.direction = 1;
        }

        if (IsKeyDown(KEY_F)){
            player.curAnim = STATE_ATTACK;
            player.state = STATE_ATTACK;
        }

        if (IsKeyDown(KEY_SPACE)){
            player.state = STATE_FLY;
            player.curAnim = STATE_FLY;
        }

        if (player.state == STATE_FLY){
            if (!IsKeyDown(KEY_SPACE)){
                player.state = -1;
                player.fly = 0;
            }
            fly(&player);
        }

        if (player.box.x < 0){
            player.box.x = 0;
        }
        if (player.box.y < 0){
            player.box.y = 0;
        }
        if (player.box.x > WIDTH - player.box.width){
            player.box.x = WIDTH - player.box.width;
        }

        if (enermy.box.x < 0){
            enermy.box.x = 0;
        }
        if (enermy.box.y < 0){
            enermy.box.y = 0;
        }
        if (enermy.box.x > WIDTH - enermy.box.width){
            enermy.box.x = WIDTH - enermy.box.width;
        }

        if (IsKeyDown(KEY_E)){
            startBullet(player,GetFrameTime());
            player.curAnim = STATE_LASER;
        }

        checkAttack(&player,&enermy,GetFrameTime());
        checkAttack(&enermy,&player,GetFrameTime());
        checkBulletAttack(&enermy,GetFrameTime());

        gravity(&player);
        gravity(&enermy);
        ai(player,&enermy);
        
        updateFrame(&player,GetFrameTime());
        updateFrame(&enermy,GetFrameTime());
        updateBullets(GetFrameTime());

        const char* playerHealth = TextFormat("Player Health: %d", player.health);
        const char* enermyHealth = TextFormat("Enemy Health: %d", enermy.health);
        
        BeginDrawing();
        ClearBackground(BGCOLOR);
        DrawTexturePro(background,(Rectangle){0,0,543,339},(Rectangle){0,0,WIDTH,HEIGHT},(Vector2){0,0},0,WHITE);
        DrawText(playerHealth,0,0,30,RED);
        DrawText(enermyHealth,400,0,30,RED);
        drawBullets();
        drawAnim(player,WHITE);
        drawAnim(enermy,GREEN);
        EndDrawing();
    }
    
    CloseWindow();
    
    UnloadTexture(player.image);
    UnloadTexture(enermy.image);
    UnloadTexture(background);
    freeBullet();
}
