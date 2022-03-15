#include <algorithm>
#include <cstdlib>
#include <raylib.h>
#include <ctime>
#include <iostream>
#include <raymath.h>
#include <vector>
#include <deque>
#include <string>
using namespace std;
class Button {
    Vector2 size;
    Vector2 position;
    Vector2 realpos;
    Color currentcolor;
    Color selectcolor;
    Color defaultcolor;
    Color textcolor;
    string text;
    bool isbuttonpressed;
    Vector2 textsize;
    int fontsize;

    public:
    Rectangle ButtonToRec() {
        float positionx = realpos.x;
        float positiony = realpos.y;
        float sizex = size.x;
        float sizey = size.y;
        Rectangle button = { positionx, positiony, sizex, sizey };
        return button;
    }

    void SetColor(Color c) {
        currentcolor = c;
    }

    void UpdateButton() {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonToRec())){
            SetColor(selectcolor);
        }else {
            SetColor(defaultcolor);
        }
    }

    bool IsButtonPressed () {
        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), ButtonToRec())){
            return true;
        } else {
            return false;
        }
    }
    
    Vector2 GetSize(){
        return size;
    }

    Vector2 GetPos() {
        return position;
    }
    
    void DrawButton(Font font) {
        DrawRectangleV(realpos, size, currentcolor);
        DrawTextEx(font, text.c_str(), {realpos.x + (size.x - textsize.x)/2, realpos.y + (size.y - textsize.y)/2}, fontsize, 1, textcolor);
    }

    Button(Vector2 s, Vector2 p, Color sc, Color dc, Color tc, string t, int fs, Font f) {
        size = s;
        position = p;
        realpos = {position.x - size.x/2, position.y - size.y/2};
        defaultcolor = dc;
        currentcolor = defaultcolor;
        selectcolor = sc;
        textcolor = tc;
        text = t;
        isbuttonpressed = false;
        fontsize = fs;
        textsize = MeasureTextEx(f, text.c_str(), fontsize, 1);
    }

};
class Player {
    Vector2 position;
    Vector2 hitboxsize;
    Vector2 vel;
    float speed;
    Color color;
    int healthvalue;
    int coinammt;
    float weaponlength;
    Color weaponcolor;
                                                        

    public:

    void DrawPlayer(){
        DrawRectangleV(position, hitboxsize, color);
    }
    void DrawWeapon() {
        Vector2 weaponhitbox = Vector2Divide(hitboxsize, {1, 3});
        Vector2 dir = Vector2Subtract(GetMousePosition(), position);
        Vector2 pos = position;
        if(abs(dir.x) > abs(dir.y)){
            float temp = weaponhitbox.x;
            weaponhitbox.x = weaponhitbox.y;
            weaponhitbox.y = temp;
            if(dir.x >  0){
                pos.x += hitboxsize.x + 5;
            } else {
                pos.x -= weaponhitbox.x + 5;
            }
        } else {
           if(dir.y >  0){
                pos.y += hitboxsize.y + 5;
            } else {
                pos.y -= weaponhitbox.y + 5;
            } 
        }
        DrawRectangleV(pos, weaponhitbox, weaponcolor);
    }
    
    void UpdatePosition(float dt){
       position = Vector2Add(position, Vector2Scale(vel, speed * dt));
    }
    void AddVel(Vector2 add){
       vel = Vector2Add(vel, add); 
    }
    void SetVel(Vector2 set){
        vel = set;
    }
    Vector2 GetVel(){
        return vel;
    }
    void AddSpeed(float add, int maxspeed){
        speed += add;
        if(speed > maxspeed) {
            speed = maxspeed; 
        }
    }
    void SubSpeed(float sub){
        speed -= sub;
        if(speed < 0){
            speed = 0;
        }
    }
    Vector2 GetPos() {
        return position;
    }

    
    Player(Vector2 sp, Vector2 s, Color c, Color wc, int h, int ca) {
        position = sp;
        hitboxsize = s;
        color = c;
        healthvalue = h;
        coinammt = ca;
        speed = 0;
        vel = {0, 0};
        weaponcolor = wc;
        weaponlength = 45;
    }

};
class Enemy {
    Vector2 size;
    Vector2 position;
    Vector2 vel;
    int health;
    int type;
    Color color;

    public:

    void DrawEnemy() {
        DrawRectangleV(position, size, color);
    }
    Vector2 GetPos(){
        return position;
    }
    
    void MoveEnemy(Vector2 target, float dt) {
        Vector2 dir = Vector2Subtract(target, position);
        if(target.x >= position.x){
            position.x += vel.x * dt;
        }
        else if(target.x <= position.x){
            position.x -= vel.x * dt;
        }
        else{
            position.x += 0;
        }
        if(target.y >= position.y){
            position.y += vel.y * dt;
        } else if(target.y <= position.y){
            position.y -= vel.y * dt;
        }
        else{
            position.x += 0;
        }
    }
    
    Enemy(Vector2 p, int t) {
        size = {50, 50};
        position = p;
        type = t;
        health = t + 1;
        color = BLUE;
        vel = {200, 200};
    }

};

class SpriteSheet {
    Texture2D sprite;
    Vector2 pos;
    Rectangle framerec;
    int framescounter;
    int framesspeed;
    int numframes;
    bool visable;

    public:

    int GetFrameSpeed() {
        return framesspeed;
    }

    void IncrementFrameCount(){
        framescounter++;
    }
    void ResetFrameCount(){
        framescounter = 0;
    }
    void SetVisable(bool tf){
        visable = tf;
    }
    bool GetVisable(){
        return visable;
    }
    void DrawSprite();

    SpriteSheet(Texture2D s, Vector2 p, int fc, int fs, int nf) {
        sprite = s;
        pos = p;
        framescounter = fc;
        framesspeed = fs;
        numframes = nf;
        framerec = {0, 0, (float)sprite.width/numframes, (float)sprite.height};
        visable = false;
    }

};
int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;
    const int maxspeed = 800;
    InitWindow(screenWidth, screenHeight, "Poopnut");
    Font defaultfont = GetFontDefault();
    Texture2D logo =  LoadTexture("assets/LaserSwords.png");
    Texture2D background = LoadTexture("assets/Stars.png");
    int scenenumber = 0;

    //ui things
    Button Start({340, 80}, {screenWidth/2, screenHeight/2 + 200}, DARKGRAY, BLANK, YELLOW, "START", 70, defaultfont);
    Button Tutorial({340, 80}, {screenWidth/2, screenHeight/2 + 280}, DARKGRAY, BLANK, YELLOW, "TUTORIAL", 70, defaultfont);
    Button Back({170, 40}, {87, 22}, DARKGRAY, BLANK, BLACK, "BACK", 35, defaultfont);

    Player P({0, 0}, {50, 50}, RED, BLUE, 3, 0);
    Enemy E({screenWidth/2, screenHeight/2}, 1);

    
    
    SetTargetFPS(60);
    SetExitKey(0);
    while (!WindowShouldClose())
    {
        float deltatime = GetFrameTime();
        BeginDrawing();

        //Scene changes
        //Start Menu
        if(scenenumber == 0){
            ClearBackground(BLACK);
            DrawTexture(background, 0, 0, WHITE);
            DrawTextureEx(logo, {screenWidth/2 - 16 * 34, screenHeight/2 - 20 * 31.5}, 0, 35, WHITE);
            if(Start.IsButtonPressed()){
                scenenumber = 1;
            }else if(Tutorial.IsButtonPressed()){
                scenenumber = 2;
            }
            Start.DrawButton(defaultfont);
            Tutorial.DrawButton(defaultfont);
            Start.UpdateButton();
            Tutorial.UpdateButton();
        //Game scene
        } else if(scenenumber == 1){
            ClearBackground(WHITE);
            Back.DrawButton(defaultfont);
            Back.UpdateButton();

            bool didmove = false;
            P.SetVel({0, 0});

            if(IsKeyDown(KEY_W)){
                P.AddVel({0, -1});
                didmove = true;

            }
            if(IsKeyDown(KEY_S)){
                P.AddVel({0, 1});
                didmove = true;

            }
            if(IsKeyDown(KEY_A)){
                P.AddVel({-1, 0});
                didmove = true;

            }
            if(IsKeyDown(KEY_D)){
                P.AddVel({1, 0});
                didmove = true;

            }
            P.SetVel(Vector2Normalize(P.GetVel()));

            if(didmove){
                P.AddSpeed(maxspeed/.125 * deltatime, maxspeed);
            }
            else{
                P.SubSpeed(maxspeed/.125 * deltatime);
            }

            P.UpdatePosition(deltatime);
            P.DrawPlayer();

            E.MoveEnemy(P.GetPos(), deltatime);
            SetMousePosition((int)E.GetPos().x, (int)E.GetPos().y);
            E.DrawEnemy();

            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                P.DrawWeapon();
            }

            //Tutorial scene
        } else if(scenenumber == 2){
            ClearBackground(PURPLE);
            Back.DrawButton(defaultfont);
            Back.UpdateButton();
        }

        //buttton logic

        if((Back.IsButtonPressed() || IsKeyPressed(KEY_ESCAPE)) && scenenumber != 0){
            scenenumber = 0;
        }
        

       
        EndDrawing();
    }
    CloseWindow();

    return 0;
}