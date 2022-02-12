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
int main(void)
{
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Poopnut");
    Font defaultfont = GetFontDefault();
    Texture2D logo =  LoadTexture("assets/LaserSwords.png");
    Texture2D background = LoadTexture("assets/Stars.png");

    //ui things
    Button Start({340, 80}, {screenWidth/2, screenHeight/2 + 200}, DARKGRAY, BLANK, YELLOW, "START", 70, defaultfont);
    Button Tutorial({340, 80}, {screenWidth/2, screenHeight/2 + 280}, DARKGRAY, BLANK, YELLOW, "TUTORIAL", 70, defaultfont);

    
    
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 0, 0, WHITE);
        DrawTextureEx(logo, {screenWidth/2 - 16 * 34, screenHeight/2 - 20 * 31.5}, 0, 35, WHITE);
        //Start Menu Garbage
        Start.DrawButton(defaultfont);
        Tutorial.DrawButton(defaultfont);
        Start.UpdateButton();
        Tutorial.UpdateButton();
        

       
        EndDrawing();
    }
    CloseWindow();

    return 0;
}