

#include "raylib.h"

#define PHYSAC_IMPLEMENTATION
#include "extras/physac.h"

#define VELOCITY    0.5f

int mainjuliauquemmandouconsertar(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "alo alo, teste.mp4");

    Texture2D runner = LoadTexture("resources/personagens/runner.png"); // Runner texture
    Texture2D texture = LoadTexture("resources/cenario.png"); // city texture

    // Physac logo drawing position
    int logoX = screenWidth - MeasureText("Physac", 30) - 10;
    int logoY = 15;

    Rectangle sourceRecRunner = {0, 0, (float)runner.width/6.05, (float)runner.height};

    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2.0f, (float)screenHeight }, (float)screenWidth, 100, 10);
    PhysicsBody platformLeft = CreatePhysicsBodyRectangle((Vector2){ screenWidth*0.25f, screenHeight*0.6f }, screenWidth*0.25f, 10, 10);
    PhysicsBody platformRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth*0.75f, screenHeight*0.6f }, screenWidth*0.25f, 10, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2.0f }, 10, (float)screenHeight, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ (float)screenWidth + 5, screenHeight/2.0f }, 10, (float)screenHeight, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    platformLeft->enabled = false;
    platformRight->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2.0f, screenHeight/2.0f }, 50, 50, 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    //Animations
    float runnerWidth = (float)runner.width/6.0;
    int runnerFrames = 6;
    float runnertimer= 0.0f;
    int runnerFrame = 0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdatePhysics();              // Update physics system

        if (IsKeyPressed(KEY_R))      // Reset physics input
        {
            // Reset movement physics body position, velocity and rotation
            body->position = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
            body->velocity = (Vector2){ 0, 0 };
            SetPhysicsBodyRotation(body, 0);
        }

        // Horizontal movement input
        if (IsKeyDown(KEY_RIGHT)) body->velocity.x = VELOCITY;
        else if (IsKeyDown(KEY_LEFT)) body->velocity.x = -VELOCITY;

        // Vertical movement input checking if player physics body is grounded
        if (IsKeyDown(KEY_UP) && body->isGrounded) body->velocity.y = -VELOCITY*4;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);
            DrawTexture(texture, 0, 0, WHITE);
            // Draw created physics bodies
            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                PhysicsBody body = GetPhysicsBody(i);

                int vertexCount = GetPhysicsShapeVerticesCount(i);
                for (int j = 0; j < vertexCount; j++)
                {
                    // Get physics bodies shape vertices to draw lines
                    // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                    Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                    int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                    Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                    DrawLineV(vertexA, vertexB, BLANK);
                }
            }

            DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
            DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);

            DrawText("Physac", logoX, logoY, 30, WHITE);
            DrawText("Powered by", logoX + 50, logoY - 7, 10, WHITE);

            runnertimer += GetFrameTime();

            if(runnertimer >= 0.2f){
                runnertimer = 0.0f;
                runnerFrame +=1;
            }

            runnerFrame = runnerFrame % runnerFrames;
            sourceRecRunner.x = runnerWidth * runnerFrame;

            DrawTextureRec(runner, sourceRecRunner, (Vector2){body->position.x - 40, body->position.y - 100}, WHITE);
            DrawRectangle(screenWidth*0.25f - screenWidth*0.25f/2, screenHeight*0.6f - 5, screenWidth*0.25f, 10, LIGHTGRAY);
            DrawRectangle(screenWidth*0.75f - screenWidth*0.25f/2, screenHeight*0.6f - 5, screenWidth*0.25f, 10, LIGHTGRAY);
            DrawRectangle(screenWidth/2.0f - (float)screenWidth/2, (float)screenHeight - 50, (float)screenWidth, 100, LIGHTGRAY);
            //pela biblioteca os dois primeiros argumentos são coordenadas do centro da figura, para desenhar em cima 
            //dela eu estou subtraindo metade do comprimento do bloco e da altura do bloco

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(runner);
    UnloadTexture(texture);
    ClosePhysics();       // Unitialize physics

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
