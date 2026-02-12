#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define RADIUS 6

#define FPS 60
#define NUM_CHARGES 500

#define SPEED 5

#define F 20000

typedef struct{
    float x, y, r, vx, vy, fx, fy;
    int charge; // can be +1 or -1
}Charge;

Charge charges[NUM_CHARGES];

void InitCharges()
{
    float r=RADIUS;
    for (int i=0; i<NUM_CHARGES; i++)
    {
        charges[i].r = r;
        charges[i].x = GetRandomValue(r, WIDTH-r);
        charges[i].y = GetRandomValue(r, HEIGHT-r);
        charges[i].vx = GetRandomValue(-SPEED, SPEED);
        charges[i].vy = GetRandomValue(-SPEED, SPEED);
        charges[i].fx = 0;
        charges[i].fy = 0;
        charges[i].charge = rand() % 2  ? 1 : -1;
    }
}

void DrawCharges()
{
    Color color;
    for (int i=0; i<NUM_CHARGES; i++)
    {
        switch(charges[i].charge)
        {
            case 1:
                color = RED;
                break;
            case -1:
                color = BLUE;
                break;
        }
        DrawCircle(charges[i].x, charges[i].y, charges[i].r,color);

    }
}
void ComputeForces()
{
    Charge *c1;
    Charge *c2;

    for (int i=0; i<NUM_CHARGES; i++)
    {
        c1 = &charges[i];
        for (int j=i+1; j<NUM_CHARGES; j++)
        {
            c2 = &charges[j];
            // What's the distance
            float dx = c2->x - c1->x;
            float dy = c2->y - c1->y;
            float r2 = dx*dx + dy*dy;

            if (r2<1)   //Clamping to d = 1
                r2=1;
            float d = sqrt(r2);
            float nx = dx/d;
            float ny = dy/d;

            float fcoulomb = F * c1->charge * c2->charge /r2; //The constant 1000 is just a "knob" to make the simulation look right on screeen.
            float fx = fcoulomb * nx;
            float fy = fcoulomb * ny;

            c1->fx -= fx;
            c1->fy -= fy;
            c2->fx = fx;
            c2->fy = fy;


        }

    }
}

void UpdateCharges(float dt)
{
    Charge *c;
    for (int i=0; i<NUM_CHARGES; i++)
    {
        c = &charges[i];
        // Numerical integration using the semi-implicit Euler method
        c->vx += c->fx * dt;
        c->vy += c->fy * dt;
        c->x += c->vx * dt;
        c->y += c->vy * dt;

        c->vx *= 0.99;
        c->vy *= 0.99;

    }
}

void ResetForces()
{
    Charge *c;
    for (int i=0; i<NUM_CHARGES; i++)
    {
        c = &charges[i];

        c->fx = 0;
        c->fy = 0;
    }
}
void CollideWalls()
{
    Charge *c;
    for (int i=0; i<NUM_CHARGES; i++)
    {
        c = &charges[i];
        if(c->x < c->r)
        {
            c->x = c->r;
            c->vx = -c->vx;
        }
        if(c->y < c->r)
        {
            c->y = c->r;
            c->vy = -c->vy;
        }
        if(c->x > WIDTH-c->r)
        {
            c->x = WIDTH-c->r;
            c->vx = -c->vx;
        }
        if(c->y > HEIGHT-c->r)
        {
            c->y = HEIGHT-c->r;
            c->vy = -c->vy;
        }

    }

}

int main() {
    srand(time(NULL));
    InitWindow(WIDTH, HEIGHT, "Coulomb Force Simulation");
    SetTargetFPS(FPS);
    InitCharges();
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            ResetForces();
            ComputeForces();
            UpdateCharges(GetFrameTime());
            CollideWalls();
            DrawCharges();// After DrawCharges():
            if (IsKeyPressed(KEY_S)) {
                TakeScreenshot("screenshot.png");
                printf("Screenshot saved!\n");
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
