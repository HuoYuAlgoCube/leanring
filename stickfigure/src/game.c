#include"raylib.h"
#include"game.h"
#include"player.h"
#include<math.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>

void updateFrame(Player* player,float frameTime){
    
    AnimFlip* anim = player->anims+player->curAnim;
    anim->timer += frameTime;
    if (anim->timer > anim->duration[anim->curFrame]){
        anim->timer -= anim->duration[anim->curFrame];
        anim->curFrame ++;
    }
    
    if (anim->curFrame >= anim->frameCount){
        anim->curFrame = 0;
        if (anim->loop != 1) {
            player->curAnim = anim->defaultAnim;
        }
    }
    
    if (player->direction == 1){
        player->box.x += anim->translation[anim->curFrame].x;
    }
    else if (player->direction == 0){
        player->box.x -= anim->translation[anim->curFrame].x;
    }
    player->box.y += anim->translation[anim->curFrame].y;
}

void drawAnim(Player player,Color color){
    if (player.visible != 1){
        return;
    }
    AnimFlip anim = player.anims[player.curAnim];
    Rectangle frame = anim.frames[anim.curFrame];
    if (player.direction == 1){
        frame.width = - fabs(frame.width);
    }
    else if (player.direction == 0){
        frame.width = fabs(frame.width);
    }
    DrawTexturePro(player.image,frame,player.box,(Vector2){0,0},0,color);
}

void setFrames(Rectangle* frames,int fw,int fh,int count,int row){
    for (int i=0;i<count;i++){
        frames[i] = (Rectangle){
            i*fw,row*fh,fw,fh
        };
    }
}

void fillf(float* ptr,int count,float f){
    for (int i=0;i<count;i++){
        ptr[i] = f;
    }
}

void fillr(Rectangle* ptr,int count,int x, int y,int w,int h){
    for (int i=0;i<count;i++){
        ptr[i] = (Rectangle){x,y,w,h};
    }
}

void gravity(Player* player){
    if (player->box.y < player->ground - player->box.height){
        player->box.y += player->gravity;
        player->gravity += player->GRAVITY;
        if (player->gravity>player->MAX_GRAVITY){
            player->gravity=player->MAX_GRAVITY;
        }
        if (player->box.y > player->ground - player->box.height){
            player->box.y = player->ground - player->box.height;
            player->gravity = 0;
        }
    }
}

void fly(Player* player){
    player->box.y -= player->fly;
    player->fly += player->FLY;
    if (player->fly > player->MAX_FLY){
        player->fly = player->MAX_FLY;
    }
}

void checkAttack(Player* player1,Player* player2,float frameTime){
    if (player1->state == STATE_ATTACK){
        player1->attackTimer += frameTime;
        AnimFlip anim = player1->anims[player1->curAnim];
        if (CheckCollisionRecs(player1->box,player2->box) && player1->attackTimer > player1->attackDuration){
            player1->attackTimer = 0;
            int curFrame = anim.curFrame;
            player2->health -= anim.attack[curFrame];
            player2->curAnim = STATE_HIT;
            if (player1->box.x > player2->box.x){
                player2->box.x -= anim.hit[curFrame].x;
                player2->box.y += anim.hit[curFrame].y;
            }
            else if (player1->box.x < player2->box.x){
                player2->box.x += anim.hit[curFrame].x;
                player2->box.y += anim.hit[curFrame].y;
            }
        }
    }
}

const int runArea = 25;
const int attackArea = 30;
const int flyArea = 50;

void ai(Player player,Player* enermy){
    enermy->curAnim = STATE_STAND;
    if (player.box.x > enermy->box.x){
        enermy->direction = 1;
    }
    if (player.box.x < enermy->box.x){
        enermy->direction = 0;
    }
    int distance = fabs(player.box.x - enermy->box.x);
    if (distance > runArea && player.curAnim != STATE_FLY){
        enermy->curAnim = STATE_RUN;
    }

    if (enermy->box.y - player.box.y > flyArea){
        enermy->curAnim = STATE_FLY;
        enermy->state = STATE_FLY;
    }
    
    if (distance < attackArea && enermy->state != STATE_FLY){
        enermy->curAnim = STATE_ATTACK;
    }

    if (enermy->state == STATE_FLY && enermy->curAnim == STATE_FLY){
        if (player.curAnim != STATE_FLY){
            enermy->state = -1;
            enermy->fly = 0;
        }
        fly(enermy);
    }
}

Player tco;
Player bullets[MAX_BULLETS];
int curBullet = 0;
const float bulletDuration = 0.1;
float playerBulletTimer = 0;
const int bulletWidth = 80;
const int bulletHeight = 40;

void initBullet(){
    Image image2 = LoadImage("resources/laser.png");
    ImageColorReplace(&image2, (Color){255, 255, 255, 255}, (Color){0, 0, 0, 0});
    Texture2D texture = LoadTextureFromImage(image2);

    for (int i=0;i<MAX_BULLETS;i++){
        bullets[i].curAnim = 0;
        bullets[i].box = (Rectangle){0,0,bulletWidth,bulletHeight};
        bullets[i].direction = 0;
        bullets[i].image = texture;
        bullets[i].animCount = 1;
        bullets[i].anims[0].frameCount = 5;
        setFrames(bullets[i].anims[0].frames,226,203,5,0);
        fillf(bullets[i].anims[0].duration,5,0.2);
        fillr(bullets[i].anims[0].translation,5,10,0,0,0);
        fillf(bullets[i].anims[0].attack,5,0.05);
        fillr(bullets[i].anims[0].hit,5,1,-1,0,0);
        bullets[i].anims[0].curFrame = 0;
        bullets[i].anims[0].timer = 0;
        bullets[i].anims[0].loop = 1;
        bullets[i].visible = 0;
        bullets[i].attackTimer = 0;
        bullets[i].attackDuration = 0.1;
        bullets[i].state = STATE_ATTACK;
    }

    UnloadImage(image2);
}

void startBullet(Player player,float frameTime){    
    if (player.isPlayer == 1){
        playerBulletTimer += frameTime;
        if (playerBulletTimer > bulletDuration){
            playerBulletTimer = 0;
        }
        else {
            return;
        }
    }
    if (curBullet > MAX_BULLETS){
        curBullet = 0;
        initBullet();
    }
    
    bullets[curBullet].visible = 1;
    bullets[curBullet].direction = player.direction;
    bullets[curBullet].isPlayer = player.isPlayer;
    if (player.direction == 0){
        bullets[curBullet].box.x = player.box.x-30;
    }
    else if (player.direction == 1){
       bullets[curBullet].box.x = player.box.x+50; 
    }
    bullets[curBullet].box.y = player.box.y;
    curBullet ++;
}

void drawBullets(){
    for (int i=0;i<MAX_BULLETS;i++){
        if (bullets[i].isPlayer == 1){
            drawAnim(bullets[i],WHITE);
        }
        else {
            drawAnim(bullets[i],GREEN);
        }
    }
}

void updateBullets(float frameTime){
    for (int i=0;i<MAX_BULLETS;i++){
        updateFrame(bullets+i,frameTime);
        if (bullets[i].box.x < 0){
            bullets[i].visible = 0;
            bullets[i].box = (Rectangle){0,0,bulletWidth,bulletHeight};
        }
        else if(bullets[i].box.x > 800 - bullets[i].box.width){
            bullets[i].visible = 0;
            bullets[i].box = (Rectangle){0,0,bulletWidth,bulletHeight};
        }
    }
}

void freeBullet(){
    for (int i=0;i<MAX_BULLETS;i++){
        UnloadTexture(bullets[i].image);
    }
}

void checkBulletAttack(Player* player,float frameTime){
    for (int i=0;i<MAX_BULLETS;i++){
        if (bullets[i].visible != 1){
            continue;
        }
        if (bullets[i].isPlayer != player->isPlayer){
            checkAttack(bullets+i,player,frameTime);
        }
    }
}

void initTco(){
    tco.box = (Rectangle){100,100,80,80};
    tco.curAnim = STATE_STAND;
    tco.health = 100;
    tco.visible = 1;
    tco.isPlayer = 1;

    int run = 5;
    int stand = 3;
    int fly = 3;
    int attack = 7;
    int hit = 2;
    
    Image image = LoadImage("resources/tco.png");
    ImageColorReplace(&image, (Color){255, 255, 255, 255}, (Color){0, 0, 0, 0});
    tco.image = LoadTextureFromImage(image);
    tco.animCount = 6;
    tco.GRAVITY = 2;
    tco.MAX_GRAVITY = 15;
    tco.FLY = 3;
    tco.MAX_FLY = 20;
    tco.fly = 0;
    tco.gravity = 0;
    tco.direction = 1;
    tco.ground = 415;
    tco.attackTimer = 0;
    tco.attackDuration = 0.5;

    int fw = 64;
    int fh = 64;

    tco.anims[STATE_STAND].frameCount = stand;
    tco.anims[STATE_RUN].frameCount = run;
    tco.anims[STATE_FLY].frameCount = fly;
    tco.anims[STATE_ATTACK].frameCount = attack;
    tco.anims[STATE_HIT].frameCount = hit;
    tco.anims[STATE_LASER].frameCount = 3;

    

    for (int i=0;i<tco.animCount;i++){
        int count = tco.anims[i].frameCount;

        tco.anims[i].frameWidth = fw;
        tco.anims[i].frameHeight = fh;
        tco.anims[i].cancelAnim = STATE_STAND;
        tco.anims[i].defaultAnim = STATE_STAND;
        tco.anims[i].curFrame = 0;
        tco.anims[i].timer = 0;
        tco.anims[i].loop = 1;
        
        if (i == STATE_STAND){
            fillf(tco.anims[i].duration,count,0.3);
            fillr(tco.anims[i].translation,count,0,0,0,0);
        }
        else if (i == STATE_RUN){
            fillf(tco.anims[i].duration,count,0.1);
            fillr(tco.anims[i].translation,count,5,0,0,0);
        }
        else if (i == STATE_FLY){
            fillf(tco.anims[i].duration,count,0.2);
            fillr(tco.anims[i].translation,count,0,0,0,0);
        }
        else if (i == STATE_ATTACK){
            memcpy(tco.anims[i].duration,(float[]){0.1,0.15,0.2,0.1,0.15,0.1,0.15},count*sizeof(float));
            fillr(tco.anims[i].translation,count,0,0,0,0);
            tco.anims[i].translation[4] = (Rectangle){5,0,0,0};
            tco.anims[i].translation[6] = (Rectangle){5,0,0,0};
            fillf(tco.anims[i].attack,count,0);
            tco.anims[i].attack[4] = 5;
            tco.anims[i].attack[6] = 5;
            fillr(tco.anims[i].hit,count,0,0,0,0);
            tco.anims[i].hit[4] = (Rectangle){3,-4,0,0};
            tco.anims[i].hit[6] = (Rectangle){3,-4,0,0};
        }
        else if (i == STATE_HIT){
            fillf(tco.anims[i].duration,count,0.4);
            fillr(tco.anims[i].translation,count,0,0,0,0);
            tco.anims[i].loop = 0;
            tco.anims[i].cancelAnim = -1;
        }
        else if (i == STATE_LASER){
            fillf(tco.anims[i].duration,count,0.2);
            fillr(tco.anims[i].translation,count,0,0,0,0);
        }
        setFrames(tco.anims[i].frames,fw,fh,count,i); 
    }
    
    UnloadImage(image);
}

Player getTco(){
    return tco;
}

void printTco(Player player){
    for (int i=0;i<player.animCount;i++){
        AnimFlip anim = player.anims[i];
        printf("anim %d:\n",i);
        for (int j=0;j<anim.frameCount;j++){
            printf("fw %d fh %d\n",anim.frameWidth,anim.frameHeight);
            printf("cancel %d\n",anim.cancelAnim);
            printf("default %d\n",anim.defaultAnim);
    
            printf("frame count %d\n",anim.frameCount);
            printf("cur frame %d\n",anim.curFrame);
            printf("timer %f\n",anim.timer);
            printf("loop %d\n",anim.loop);
            for (int n=0;n<anim.frameCount;n++){
                printf("frame %d:\n",n);
                printf("x %f y %f w %f h %f \n",anim.frames[i].x,anim.frames[i].y,anim.frames[i].width,anim.frames[i].height);
                printf("duration %f\n",anim.duration[i]);
                printf("tx %f ty %f tw %f th %f \n",anim.translation[i].x,anim.translation[i].y,anim.translation[i].width,anim.translation[i].height);
            }
        }
    }
    return;
}
