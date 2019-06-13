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
char ScoreText[1024];
bool gameOver=false;

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
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
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
       BallPosition1.x=Center.x+Radius*cos(Angle);
       BallPosition1.y=Center.y+Radius*sin(Angle);
       
       BallPosition2.x=Center.x-Radius*cos(Angle);
       BallPosition2.y=Center.y-Radius*sin(Angle);
       for (int i=0; i<currentPosition; i++)
       {
            struct Obstacle* ob=&Obstacles[i];
            
            if (ob->active)
            {
                if (ob->position.x>480)
                {
                    Score++; 
                    ob->active=false;
                }
                else if (ob->position.x>160)
                {
                    if ((ob->position.y<80&&ob->initialPosition.y>240)||(ob->position.y>400&&ob->initialPosition.y<=240))
                    {
                        Score++;
                        ob->active=false;
                    }
                }
                
            }
            
            if (ob->position.x<0||ob->position.x>640||ob->position.y<0||ob->position.y>480)
            {
                currentPosition--;
                Obstacles[i]=Obstacles[currentPosition];
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
    
    sprintf(ScoreText, "%d", Score);
  
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
        {
            DrawCircleV(Obstacles[i].position,Obstacles[i].size ,BLACK);
        }
        sprintf(ScoreText, "%d", Score);
        DrawText(ScoreText,560, 10, 40, GRAY);
        
    }
    
    else    
    {
        DrawText("GAME OVER", GetScreenWidth()/2 - MeasureText("GAME OVER", 60)/2, 100, 60, BLACK);
        char scoreString[30]="YOUR SCORE IS: ";
        strcat(scoreString,ScoreText);
        DrawText(scoreString, GetScreenWidth()/2 - MeasureText(scoreString, 40)/2, GetScreenHeight()/2 -40, 40, GRAY);
        DrawText("Press [ENTER] to replay", GetScreenWidth()/2 - MeasureText("Press [ENTER] to replay", 40)/2, GetScreenHeight()/2 , 40, GRAY);

        
        if (IsKeyDown(KEY_ENTER))
        {
            GameInit();
            return;
        }
    }
       
    EndDrawing();
}

void GenerateObstacle(void)
{
    Obstacles[currentPosition].position.x=0;
    Obstacles[currentPosition].position.y=rand()%480;
    Obstacles[currentPosition].size=20;
    Obstacles[currentPosition].initialPosition=Obstacles[currentPosition].position;
    
    Obstacles[currentPosition].velocity.x=rand()%2+1;
    Obstacles[currentPosition].velocity.y=rand()%2+1;
    
    Obstacles[currentPosition].active=true;
    currentPosition++;
    
}

void ObstacleMove(int i)
{
    if (CheckCollisionCircles(BallPosition1,20,Obstacles[i].position,20)||CheckCollisionCircles(BallPosition2,20,Obstacles[i].position,20))
    {
        gameOver=true;
        return;
    }
        
    if (Obstacles[i].initialPosition.y>240)
    {
        Obstacles[i].position.x+=3*Obstacles[i].velocity.x;
        Obstacles[i].position.y-=Obstacles[i].velocity.y;
        return;
    }
    
        Obstacles[i].position.x+=3*Obstacles[i].velocity.x;
        Obstacles[i].position.y+=Obstacles[i].velocity.y; 
     
    
}

void BallSpin(void)
{
    if (IsKeyDown(KEY_SPACE)&&!gameOver)
    {
        Angle+=AngleSpeed*8;
    }
    Angle-=AngleSpeed*2;
}


