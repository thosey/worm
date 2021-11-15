#include "raylib.h"
#include "math.h"
#include "stdlib.h"

Color colorFromIndex(int idx, int range){   
    return ColorFromHSV((360*idx)/range, 1, 1);
}

float max (float a, float b){
    return (a > b) ? a : b;
}

void drawLine(int x1, int y1, int x2, int y2, int index, int pos, int range){    
    Vector2 start;
    start.x = x1;
    start.y = y1;

    Vector2 end;
    end.x = x2;
    end.y = y2;
        
    float distance = (range + pos-index)%range;  
    float distNoramalized = (range - distance)/range;
    float thickness = 5.0 * (distNoramalized);
    Color color = ColorFromHSV(360*distNoramalized, 1, (0.2 + 0.8*distNoramalized));
   if ((index - pos )%20 == 0){        
      thickness = max(1.0, thickness);
      color = WHITE;
    }
   /* if ((index - pos)%99 == 0){
        thickness = max(2.0, 1.5*thickness);
        color = GREEN;
    } */   
    DrawLineEx (start, end, thickness, color);   
}
int main(void)
{
    const int screenWidth = 3750;
    const int screenHeight = 1800;
    int x = screenWidth/2;
    int y = screenHeight/2;
    
    double vx = 0.0;
    double vy  = 0.0;
    
    double drag = 0.01;
    int discFactor = 200;
    double step = 10.0/discFactor;
    int numPaths = 10000;
    bool fullPaths = false;
    int* pathsX = malloc(sizeof(int)*numPaths);
    int* pathsY= malloc(sizeof(int)*numPaths);
    
    int pathsCounter = 0;
    
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    ClearBackground(BLACK);          
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {              
        vx = vx + (step * (GetRandomValue(0, 2*discFactor) - discFactor)); 
        vy = vy + (step * (GetRandomValue(0, 2*discFactor) - discFactor)); 
        double mag = sqrt((vx*vx) + (vy*vy));

        vx = vx - (drag*mag*vx);       
        vy = vy - (drag*mag*vy);
        
        if(x > screenWidth || x < 0){
            vx = -vx;
        }
        if(y > screenHeight || y < 0){
            vy = -vy;
        }
        x = x + vx;
        y = y + vy;
               
        pathsX[pathsCounter] = x;
        pathsY[pathsCounter] = y;
        pathsCounter = (pathsCounter + 1) % numPaths;
        
        if(pathsCounter == (numPaths-1)){
            fullPaths = true;
        }
        
        BeginDrawing();

        ClearBackground(BLACK);                 
        DrawCircle(x, y, 5, RED);
        DrawLine (x, y, x + 10*vx, y + 10*vy, GREEN);

        if(fullPaths){            
            for(int idx = pathsCounter + 2; idx < numPaths; idx++ ){               
                drawLine (pathsX[idx-1], pathsY[idx-1], pathsX[idx], pathsY[idx], idx, pathsCounter, numPaths);                   
            }
            drawLine (pathsX[numPaths-1], pathsY[numPaths-1], pathsX[0], pathsY[0], 0, pathsCounter, numPaths);                   
        }
               
        if(pathsCounter>1){
        for(int idx = 1; idx < pathsCounter - 1; idx++){
            drawLine (pathsX[idx-1], pathsY[idx-1], pathsX[idx], pathsY[idx], idx, pathsCounter, numPaths);               
        }
        }
             
        DrawFPS(0,0);
        DrawText(TextFormat("%i, %i, %f", x, y, mag), screenWidth/2, 0, 20, LIGHTGRAY);
        EndDrawing();    
    }


    CloseWindow();  
    
    free(pathsX);
    free(pathsY);
    return 0;
}