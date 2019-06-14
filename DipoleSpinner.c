#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

Vector2 BallPosition1,BallPosition2;
Vector2 Center; 
double Angle, Radius, AngleSpeed=PI/180;
int Score;
char ScoreText[10];
bool gameOver;

struct Obstacle{
    double size;
    Color color;
    int shape;
    Vector2 position, velocity;
    bool active;
    Vector2 initialPosition; 
} Obstacles[4096];


int currentPosition=0; 
int framecount=0;

void GameInit(void);
void DrawGame(void);
void GenerateObstacle(void);
void ObstacleMove(int i);
void BallSpin(void);

int main(void)
{
    InitWindow(640, 480, "Dipole Spinner");
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    GameInit();
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       framecount++;
       if (framecount==90) 
       {
            GenerateObstacle();
            framecount=0;
       }
       
       BallSpin();
       
       for (int i=0; i<currentPosition; i++)
       {
            struct Obstacle* ob=&Obstacles[i]; //set the pointer ob to the adress of Obstacles[i] 
            
            if (ob->active)             // if the obstacle is active
            {
                if (ob->position.x>480) // if its x >480 (320+(2r=40)+(R=120)
                {
                    Score++;            
                    ob->active=false;   //set the active status to false
                }
                else if (ob->position.x>160)    // if it's inside the big circle
                {
                    if ((ob->position.y<80&&ob->initialPosition.y>240)||    //obstacle moves up & exceed the min y
                        (ob->position.y>400&&ob->initialPosition.y<=240))   //obstacle moves down & exceed the max y
                    {
                        Score++;
                        ob->active=false;
                    }
                }
                
            }
            
            if (ob->position.x<0||ob->position.x>640||ob->position.y<0||ob->position.y>480) //obstacle out of screen
            {
                currentPosition--;
                *ob=Obstacles[currentPosition]; //set the last obstacle of array to the empty position 
                continue;
            }
            ObstacleMove(i);  
       }   
       DrawGame();
    }
    CloseWindow();        
    return 0;
}

void GameInit(void)
{
    Center.x= 320.0;
    Center.y= 240.0;
    currentPosition=0;
    framecount=0;
    Score=0;
    Angle=0, Radius=120, AngleSpeed=PI/180;
    gameOver=false;
}

void BallSpin(void)
{
    if (IsKeyDown(KEY_SPACE)&&!gameOver)
        Angle+=AngleSpeed*8;
    
    Angle-=AngleSpeed*2;
    
    BallPosition1.x=Center.x+Radius*cos(Angle);
    BallPosition1.y=Center.y+Radius*sin(Angle);
   
    BallPosition2.x=Center.x-Radius*cos(Angle);
    BallPosition2.y=Center.y-Radius*sin(Angle);
}

void GenerateObstacle(void)
{
    struct Obstacle* ob=&Obstacles[currentPosition];
    ob->position.x=0;
    ob->position.y=rand()%480;          //obstacle moves vertically random from the left
    ob->size=20+rand()%20;              //obstacles' size ranges randomly in range[20,40]
    ob->initialPosition=ob->position;   
    
    ob->velocity.x=rand()%2+1;          //random velocity.x &y
    ob->velocity.y=rand()%2+1;
    
    ob->active=true;
    currentPosition++;
}

void ObstacleMove(int i)
{
    struct Obstacle* ob=&Obstacles[i];
    if (CheckCollisionCircles(BallPosition1,20,ob->position,ob->size) 
            ||CheckCollisionCircles(BallPosition2,20,ob->position,ob->size))
    {
        gameOver=true;      //collide --> game over
        return;
    }
        
    if (ob->initialPosition.y>240)      //the obstacle move up if from the bottom of screen
    {
        ob->position.x+=3*ob->velocity.x;
        ob->position.y-=ob->velocity.y;
        return;
    }
    
    ob->position.x+=3*ob->velocity.x;   
    ob->position.y+=ob->velocity.y; 
}

void DrawGame(void)
{
    BeginDrawing();
    
    ClearBackground(RAYWHITE);
    if (!gameOver)      
    {
        DrawCircleV(BallPosition1, 20, MAROON);
        DrawCircleV(BallPosition2, 20, DARKBLUE);
        
        for (int i=0; i<currentPosition; i++)
            DrawCircleV(Obstacles[i].position,Obstacles[i].size ,BLACK);
        
        sprintf(ScoreText, "%d", Score); //score-> string and store in ScoreText
        DrawText(ScoreText,560, 10, 40, GRAY);
        
    }
    
    else    
    {
        DrawRectangle(20,40,600,400,SKYBLUE);
        Rectangle rec={20,40,600,400};
        DrawRectangleLinesEx(rec,20,BLUE);
        DrawText("GAME OVER", GetScreenWidth()/2 - MeasureText("GAME OVER", 60)/2, 100, 60, BLACK); //text,x,y,fontsize,color
        char scoreString[50]="YOUR SCORE IS: ";
        strcat(scoreString,ScoreText);
        DrawText(scoreString, GetScreenWidth()/2 - MeasureText(scoreString, 40)/2, GetScreenHeight()/2 -40, 40, DARKGRAY);
        DrawText("Press [ENTER] to replay", GetScreenWidth()/2 - MeasureText("Press [ENTER] to replay", 40)/2, 
            GetScreenHeight()/2 , 40, DARKGRAY);
        
        if (IsKeyDown(KEY_ENTER))
        {
            GameInit();
            return;
        }
    }
       
    EndDrawing();
}





