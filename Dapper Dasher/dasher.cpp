#include "raylib.h"


struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}


AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
        {
            data.runningTime = 0.0;

        // Update animation frame
            data.rec.x = data.frame * data.rec.width;
            data.frame++;     
            if (data.frame > maxFrame)
            {
                data.frame = 0;
            }
        }
    return data;
}

int main()
{
    double windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");
    bool collision{};
    SetTargetFPS(60);

    // VARIABLES

    // Player
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.frame = 1;
    scarfyData.pos.x = (windowDimensions[0]/2) - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.updateTime = 1.0/12.0;
    
    int velocity = 0;
    int Jump = -600;
    int gravity = 1'000;
    bool isInAir = false;

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    AnimData finishline{
        finishline.rec.width = 10,
        finishline.rec.height = 400,
        finishline.rec.x = 0,
        finishline.rec.y = 0,
        finishline.pos.x = 6500,
    };


    // Nebula Hazards
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae = 15;
    AnimData nebulae[sizeOfNebulae]{};

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].rec.x = 0.0;
            nebulae[i].rec.y = 0.0;
            nebulae[i].rec.width = nebula.width/8;
            nebulae[i].rec.height = nebula.height/8;
            nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
            nebulae[i].frame = 0;
            nebulae[i].runningTime = 0.0;
            nebulae[i].updateTime = 0.0;
            nebulae[i].pos.x = windowDimensions[0] + i * 400;
        }


    int nebulaVel = -600;

    
    // GAME
    
    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();
        

        // Begin Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dt;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }

        mgX -= 40 * dt;
        if (mgX <= -midground.width *2)
        {
            mgX = 0.0;
        }

        fgX -= 80 * dt;
        if (fgX <= -midground.width *2)
        {
            fgX = 0.0;
        }


        // Draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        Vector2 mg1Pos{bgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        Vector2 fg1Pos{bgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // Game Logic


        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dt, 7 );
        }
      
        


        if (!isInAir)
        {
           scarfyData = updateAnimData(scarfyData, dt, 5);
        }

        
        // Ground Check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            velocity = 0;
            isInAir = false;
        }
        else{
            velocity += gravity * dt;
            isInAir = true;
        }
        
        // Player Controls
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += Jump;
        }

        // Update nebula positions
        for (int i = 0; i< sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebulaVel * dt;
        }

        finishline.pos.x += nebulaVel * dt;

        // Update scarfy position
        scarfyData.pos.y += velocity * dt;

        
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }

        }

        if (collision)
        {
            DrawText("GAME OVER", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, DARKBLUE);
        }

        else if (scarfyData.pos.x >= finishline.pos.x)
        {
            DrawText("YOU WIN", windowDimensions[0] / 3, windowDimensions[1] / 2, 40, DARKBLUE);
        }

        else{

        // Draw Nebulas
        for (int i = 0; i< sizeOfNebulae; i++)
        {
            DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
        }

        // Draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }



        // End Drawing
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}