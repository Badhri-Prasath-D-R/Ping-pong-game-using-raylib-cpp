#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{38,185,154,255};
Color Dark_Green = Color{20,160,133,255};
Color Light_Green = Color{129,204,184,255};
Color Yellow = Color{243,213,91,255};

int player_score = 0;
int cpu_score = 0;

class Ball
{
    public:
    float x,y;
    int speedx,speedy;
    int radius;
    void draw()
    {
        DrawCircle(x,y,radius,Yellow);
    }
    void update()
    {
        x += speedx;
        y += speedy;

        if(y+radius >= GetScreenHeight() || y-radius<=0)
        {
            speedy = speedy * -1;
        }
        if(x+radius >= GetScreenWidth())
        {
            player_score++;
            resetball();
        }
        if(x-radius <= 0)
        {
            cpu_score++;
            resetball();
        }
    }
    void resetball()
    {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1,1};
        speedx = speedx*speed_choices[GetRandomValue(0,1)];
        speedy = speedy*speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle
{
    protected:
    void limitmovement()
    {
        if(y<=0)
        {
            y=0;
        }
        if(y+height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
    public:
    float x,y;
    float width,height;
    int speed;

    void draw()
    {
        DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,BLACK);
    }

    void update()
    {
        if(IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
        limitmovement();
    }

};

class cpupaddle:public Paddle
{
    public:
    void update(int bally)
    {
        if(y+height/2 >  bally)
        {
            y = y-speed;
        }
        if(y+height/2 < bally)
        {
            y = y + speed;
        }
        limitmovement();
    }
};

int main()
{

    Ball ball;
    Paddle player;
    cpupaddle cpu;
    cout<<"starting the game"<<endl;

    //creating a window
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width,screen_height,"My Pong Game");

    //setting fps limit
    SetTargetFPS(60);
    
    //setting ball attributes
    ball.radius = 20;
    ball.x=screen_width/2;
    ball.y=screen_height/2;
    ball.speedx=7;
    ball.speedy=7;


    //setting player attributes (paddle on the left side of the screen)
    player.width = 25;
    player.height = 120;
    player.speed = 6;
    player.x = 10;
    player.y = screen_height/2 - player.height/2;

    //setting cpu paddle attributes(paddle on the right side of the screen)
    cpu.width = 25;
    cpu.height = 120;
    cpu.speed = 6;
    cpu.x = screen_width - player.width - 10;
    cpu.y = screen_height/2 - player.height/2;

    
    //Game loop
    while(WindowShouldClose()==false)
    {

        BeginDrawing();

        //updating ball speed
        ball.update();
        player.update();
        cpu.update(ball.y);
        ClearBackground(Dark_Green);

        DrawRectangle(0,0,screen_width/2,screen_height,Green);
        DrawCircle(screen_width/2,screen_height/2,150,Light_Green);
        //checking for collisions
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height}))
        {
            ball.speedx = ball.speedx*-1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height}))
        {
            ball.speedx = ball.speedx*-1;
        }

        //drawing the paddles
        cpu.draw();
        ball.draw();
        player.draw();

        //text for score
        DrawText(TextFormat("%i",player_score),screen_width/4-20,20,80,WHITE);
        DrawText(TextFormat("%i",cpu_score),3*screen_width/4-20,20,80,WHITE);

        //drawing the mid line
        DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);

        EndDrawing();
    }

    //closing the window
    CloseWindow();
    return 0;
}