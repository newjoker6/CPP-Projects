#include "raylib.h"



int width = 800;
int height = 450;

int circle_x = 200;
int circle_y = 200;
int circle_radius = 25;
int l_circle_x = circle_x - circle_radius;
int r_circle_x = circle_x + circle_radius;
int u_circle_y = circle_y - circle_radius;
int b_circle_y = circle_y + circle_radius;

int axe_x = 400;
int axe_y = 0;
int axe_length = 50;
int l_axe_x = axe_x;
int r_axe_x = axe_x + axe_length;
int u_axe_y = axe_y;
int b_axe_y = axe_y + axe_length;

int direction = 10;
bool collision_with_axe = 
                (b_axe_y >= u_circle_y) && 
                (u_axe_y <= b_circle_y) && 
                (r_axe_x >= l_circle_x) && 
                (l_axe_x <= r_circle_x);

int score = 0;

int main()
{
    InitWindow(width, height, "Axe Game");

    SetTargetFPS(60);
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(WHITE);
        
       DrawText("Score: 0", 20, 20, 20, BLUE);

        if (collision_with_axe)
        {
            DrawText("Game Over", 400, 200, 20, RED);
        }
        else
        {
            // Update Collision Edges
            l_circle_x = circle_x - circle_radius;
            r_circle_x = circle_x + circle_radius;
            u_circle_y = circle_y - circle_radius;
            b_circle_y = circle_y + circle_radius;

            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_length;
            u_axe_y = axe_y;
            b_axe_y = axe_y + axe_length;



            //Game Logic Begins
            DrawCircle(circle_x, circle_y, circle_radius, BLUE);
            DrawRectangle(axe_x, axe_y, 50, axe_length, RED);

            axe_y += direction;

            if(axe_y > height || axe_y < 0)
            {
                direction = -direction;
            }

            // Player Movement
            if (IsKeyDown(KEY_D) && circle_x + circle_radius< width)
            {
                circle_x += 10;
            }
            else if (IsKeyDown(KEY_A) && circle_x - circle_radius > 0)
            {
                circle_x -= 10;
            }

            if (b_axe_y >= u_circle_y && u_axe_y <= b_circle_y && l_axe_x <= r_circle_x && r_axe_x >= l_circle_x)
            {
                collision_with_axe = true;
            }
        }


        // Game Logic Ends
        EndDrawing();
    }
}
