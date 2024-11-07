#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

#define TOTAL_ENEMIES 15

Vector2 mouse;

bool isGameResetting = false;
// _______________________________________________________________
//
//
// Main Menu System
//
// _______________________________________________________________
typedef enum
{
    MainMenu,
    GameMenu,
    OptionsMenu,
    ExitMenu,
    GameOverMenu,
    ShopMenu,
} MenuState;

MenuState currentMenuState = MainMenu;

void DrawMainMenu()
{
    ClearBackground(BLUE);

    int mainMenuTextWidth = MeasureText("Main Menu", 40);
    DrawText("Main Menu", (GetScreenWidth() / 2) - (mainMenuTextWidth / 2), 20, 40, BLACK);

    Rectangle startButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 60, 200, 40};
    Rectangle optionsButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 200, 40};
    Rectangle exitButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 60, 200, 40};

    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, startButton))
    {
        DrawRectangleRec(startButton, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentMenuState = GameMenu; // Switch to game state
        }
    }
    else
    {
        DrawRectangleRec(startButton, GRAY);
    }

    if (CheckCollisionPointRec(mousePoint, optionsButton))
    {
        DrawRectangleRec(optionsButton, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentMenuState = OptionsMenu; // Switch to options state
        }
    }
    else
    {
        DrawRectangleRec(optionsButton, GRAY);
    }

    if (CheckCollisionPointRec(mousePoint, exitButton))
    {
        DrawRectangleRec(exitButton, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentMenuState = ExitMenu; // Exit the game
        }
    }
    else
    {
        DrawRectangleRec(exitButton, GRAY);
    }

    // Draw button text
    DrawText("Start Game", startButton.x + 40, startButton.y + 8, 20, BLACK);
    DrawText("Options", optionsButton.x + 60, optionsButton.y + 8, 20, BLACK);
    DrawText("Exit", exitButton.x + 80, exitButton.y + 8, 20, BLACK);
}

void MainMenuLogic()
{
    switch (currentMenuState)
    {
    case MainMenu:
        DrawMainMenu();
        break;
    case GameMenu:
        break;
    case GameOverMenu:
        break;
    case ShopMenu:
        break;
    case OptionsMenu:
        break;
    case ExitMenu:
        break;
    }
}

// Function to draw a button and handle clicks
bool DrawButton(Rectangle button, const char *text, Color baseColor, Color hoverColor, Color textColor)
{
    Vector2 mousePoint = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePoint, button);

    // Change color based on hover state
    DrawRectangleRec(button, isHovered ? hoverColor : baseColor);
    DrawText(text, button.x + 50, button.y + 8, 20, textColor); // Draw the button text

    // Return true if button is clicked
    return isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}
// _______________________________________________________________
//
//
// Shop Enum
//
// _______________________________________________________________
typedef enum
{
    BaseHealthPotion,
    SpeedPotion,
    IronSword,
    UltraSword,
    BaseUpgrade_One,
    BaseUpgrade_Two,
    BaseUpgrade_Three,
} ShopStructEnum;
// _______________________________________________________________
//
//
// Base System
//
// _______________________________________________________________
typedef struct
{
    int HP;
    float Size;
    Vector2 Pos;
} BaseStruct;

BaseStruct Base = {
    .HP = 200,
    .Size = 64};

int CurrentBaseUpgrade = BaseUpgrade_One;

void DrawBase()
{
    if (CurrentBaseUpgrade == BaseUpgrade_One)
    {
        DrawCircleV(Base.Pos, Base.Size, LIGHTGRAY);
    }
    else if (CurrentBaseUpgrade == BaseUpgrade_Two)
    {
        /* Code */
    }
    else if (CurrentBaseUpgrade == BaseUpgrade_Three)
    {
        /* code */
    }
}
// _______________________________________________________________
//
//
// Enemy System
//
// _______________________________________________________________
typedef struct
{
    Vector2 Pos;
    Vector2 Vel;
    Vector2 Size;
    int HP;
    int DMG;
    int Type;
    float Speed;
    float AttackCooldown;
    float SpawnCooldown;
    Rectangle Enemy_Rect;
    bool IsStopped;
} EnemyStruct;

typedef enum
{
    Enemy_No_Enemy,
    Enemy_Zombie,
    Enemy_Skeleton,
} EnemyTypeEnum;

typedef struct
{
    int HP;
    int DMG;
    float Speed;
    float AttackCooldown;
    float SpawnCooldown;
    Vector2 Size;
    Rectangle Enemy_Rect;
} EnemyTypeStruct;

EnemyStruct Enemies[TOTAL_ENEMIES];

EnemyTypeStruct Enemy_DB[] = {
    {// NoEnemy
     .HP = 1,
     .DMG = 1,
     .Speed = 1,
     .AttackCooldown = 1.0f,
     .SpawnCooldown = 1.0f,
     .Size = (Vector2){10, 10}},
    {// Zombie
     .HP = 8,
     .DMG = 5,
     .Speed = 20,
     .AttackCooldown = 3.0f,
     .SpawnCooldown = 2.0f,
     .Size = (Vector2){24, 24}},
    {// Skeleton
     .HP = 4,
     .DMG = 10,
     .Speed = 30,
     .AttackCooldown = 5.0f,
     .SpawnCooldown = 4.0f,
     .Size = (Vector2){20, 20}}

};

// Enemies Spawn Amount
int Enemies_MinAmount = 2;
int Enemies_MaxAmount = TOTAL_ENEMIES;

// _______________________________________________________________
int Highscore = 0;
int CurrentWave = 0;
int EnemiesLeft = 0;
bool WaveInProgess = false;

void Enemy_Spawn(int Type, Vector2 Pos)
{
    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {
        if (Enemies[i].Type == Enemy_No_Enemy)
        {
            Enemies[i] = (EnemyStruct){
                .Type = Type,
                .Pos = Pos,
                .Vel = (Vector2){0, 0},
                .Speed = Enemy_DB[Type].Speed,
                .Size = Enemy_DB[Type].Size,
                .DMG = Enemy_DB[Type].DMG,
                .HP = Enemy_DB[Type].HP,
                .AttackCooldown = Enemy_DB[Type].AttackCooldown};

            // Debug code
            // if (Type == Enemy_Skeleton)
            // {
            //     printf("Skeleton Spawned at position: (%f, %f)\n", Pos.x, Pos.y);
            // }
            EnemiesLeft++;
            break;
        }
    }
}

void Enemy_Update(float delta_time, Vector2 targetPos)
{
    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {
        // Creates a distance so the enemies stop when colliding with the base and not when they reach the position of the base
        float distanceTobase = Vector2Distance(Enemies[i].Pos, targetPos);

        // Stops the enemy if it comes in the distance radius
        if (distanceTobase <= Base.Size + 1)
        {
            Enemies[i].IsStopped = true;
            Enemies[i].Vel = (Vector2){0, 0}; // Stops the enemy
        }
        else
        {
            Enemies[i].Vel = Vector2Subtract(targetPos, Enemies[i].Pos);
            Enemies[i].Vel = Vector2Normalize(Enemies[i].Vel);
            Enemies[i].Vel = Vector2Scale(Enemies[i].Vel, Enemy_DB[Enemies[i].Type].Speed);
            Enemies[i].Pos = Vector2Add(Enemies[i].Pos, Vector2Scale(Enemies[i].Vel, delta_time));
        }
    }
}

void Enemy_Draw(float delta_time)
{
    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {
        switch (Enemies[i].Type)
        {
        case Enemy_No_Enemy:
        {
            continue;
        }
        break;

        case Enemy_Zombie:
        {
            DrawRectangleV(Enemies[i].Pos, Enemies[i].Size, DARKGREEN);
        }
        break;

        case Enemy_Skeleton:
        {
            DrawRectangleV(Enemies[i].Pos, Enemies[i].Size, GRAY);
            // Debug code
            // printf("Drawing skeleton at position: (%f, %f)\n", enemies[i].Pos.x, enemies[i].Pos.y);
        }
        break;

        default:
            break;
        }
    }
}
// _______________________________________________________________
void Enemy_SpawnWave()
{
    typedef enum
    {
        Direction_North,
        Direction_South,
        Direction_East,
        Direction_West
    } EnemyDirectionEnum;

    typedef enum
    {
        Enemy_No_Enemy = 0,
        Enemy_Zombie,
        Enemy_Skeleton,
        EnemyType_Count
    } EnemyTypeEnum;

    if (!WaveInProgess)
    {
        int Enemy_SpawnCount = GetRandomValue(Enemies_MinAmount, Enemies_MaxAmount);

        for (int i = 0; i < Enemy_SpawnCount; i++)
        {
            Vector2 EnemySpawn_Pos = {0, 0};

            EnemyDirectionEnum EnemyDirection = (EnemyDirectionEnum)GetRandomValue(0, 3);

            switch (EnemyDirection)
            {
            case Direction_North:
                EnemySpawn_Pos.x = GetRandomValue(0, GetScreenWidth());
                EnemySpawn_Pos.y = -50;
                break;
            case Direction_South:
                EnemySpawn_Pos.x = GetRandomValue(0, GetScreenWidth());
                EnemySpawn_Pos.y = GetScreenHeight() + 50;
                ;
                break;
            case Direction_East:
                EnemySpawn_Pos.x = GetScreenWidth() + 50;
                EnemySpawn_Pos.y = GetRandomValue(0, GetScreenHeight());
                break;
            case Direction_West:
                EnemySpawn_Pos.x = -50;
                EnemySpawn_Pos.y = GetRandomValue(0, GetScreenHeight());
                break;
            }

            int Enemy_Random = GetRandomValue(1, EnemyType_Count - 1);

            Enemy_Spawn(Enemy_Random, EnemySpawn_Pos);
        }

        WaveInProgess = true;
    }
}

void Enemy_CheckDefeated()
{
    int EnemiesAlive = 0;
    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {
        if (Enemies[i].Type != Enemy_No_Enemy && Enemies[i].HP > 0)
        {
            EnemiesAlive++;
        }
    }

    if (EnemiesAlive == 0)
    {
        WaveInProgess = false;
        CurrentWave++; // Increases the wave number
    }
}

void Enemy_Collision(Vector2 targetPos, int *targetHP, float targetSize, float delta_time)
{
    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {

        if (Enemies[i].Type == Enemy_No_Enemy)
        {
            continue;
        }

        Rectangle Rect_Enemy = {Enemies[i].Pos.x, Enemies[i].Pos.y, Enemies[i].Size.x, Enemies[i].Size.y}; // Enemy Rectangle

        if (Enemies[i].AttackCooldown > 0.0f) // Reduce attackcooldown over time
        {
            Enemies[i].AttackCooldown -= delta_time;
        }

        if (CheckCollisionCircleRec(targetPos, targetSize, Rect_Enemy))
        {
            Enemies[i].Vel = (Vector2){0.0f, 0.0f}; // Stops the Enemy
            Enemies[i].IsStopped = true;            // Sets as stopped

            if (Enemies[i].AttackCooldown <= 0.0f) // Only attack when the enemy is allowed to
            {
                (*targetHP) -= Enemies[i].DMG;    // Reduce the base HP
                Enemies[i].AttackCooldown = 3.0f; // Waits 3 seconds until attacking again
                printf("Enemy damaged base at posistion: (%f, %f) Base HP: %d\n", Enemies[i].Pos.x, Enemies[i].Pos.y, *targetHP);
            }
        }
        else
        {
            Enemies[i].IsStopped = false;
            Enemies[i].Vel = Vector2Normalize(Vector2Subtract(targetPos, Enemies[i].Pos));
            Enemies[i].Vel = Vector2Scale(Enemies[i].Vel, Enemies[i].Speed);
        }
    }
}
// _______________________________________________________________
//
//
// Player System
//
// _______________________________________________________________
typedef struct PlayerStruct
{
    int DMG;
    float Speed;
    Vector2 Pos;
    Vector2 Vel;
    Vector2 Size;
    int Coins;
    float AttackCooldown;
} PlayerStruct;

PlayerStruct Player = {
    .DMG = 1,
    .Speed = 200,
    .Vel = (Vector2){0, 0},
    .Size = (Vector2){24, 24},
    .AttackCooldown = 0.0f,
    .Coins = 0};

typedef enum
{
    Direction_UP,
    Direction_Down,
    Direction_Right,
    Direction_Left
} PlayerDirectionEnum;

PlayerDirectionEnum LastDirection = Direction_Down; // Defaults the direction to down

Rectangle Rect_Player;

void Player_Collision(float delta_time)
{
    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {
        if (Enemies[i].Type == Enemy_No_Enemy)
        {
            continue;
        }

        Rectangle Rect_Enemy = {Enemies[i].Pos.x, Enemies[i].Pos.y, Enemies[i].Size.x, Enemies[i].Size.y};

        if (CheckCollisionRecs(Rect_Enemy, Rect_Player))
        {
            // Enemy Stops
            Enemies[i].Vel = (Vector2){0.0f, 0.0f};
            Enemies[i].IsStopped = true;

            // Player movement reverses
            Player.Pos.x -= Player.Vel.x * delta_time;
            Player.Pos.y -= Player.Vel.y * delta_time;
        }
        else
        {
            Enemies[i].IsStopped = false;
        }
    }
}
// _______________________________________________________________
//
//
// Weapon System
//
// _______________________________________________________________
typedef struct
{
    int Type;
    int DMG;
    int HP;
    float SwordWidth;
    float SwordHeight;
    float Speed;
} WeaponStruct;

typedef enum
{
    Weapon_No_Weapon,
    Weapon_Sword_Wooden,
    Weapon_Sword_Iron,
    Weapon_Sword_Ultra,
    Potion_HP,
    Potion_Speed,
} WeaponTypeEnum;

typedef struct
{
    int DMG;
    int HP;
    float SwordWidth;
    float SwordHeight;
    float Speed;
} WeaponTypeStruct;

WeaponStruct Weapons[10];

WeaponTypeStruct Weapon_DB[] = {
    {// No Weapon
     .DMG = 1},
    {// Wooden Sword
     .DMG = 2,
     .SwordWidth = 18,
     .SwordHeight = 18},
    {// Iron Sword
     .DMG = 5,
     .SwordWidth = 24,
     .SwordHeight = 24},
    {// Ultra Sword
     .DMG = 10,
     .SwordWidth = 36,
     .SwordHeight = 36},
    {// Health Pot
     .HP = 200},
    {// Speed Pot
     .Speed = 100},
};

Rectangle Rect_SwordSwing;

int CurrentWeapon = Weapon_Sword_Wooden;

float SwordWidth;
float SwordHeight;

void Weapon_Type(int Type)
{
    for (int i = 0; i < 10; i++)
    {
        if (Weapons[i].Type == Weapon_No_Weapon)
        {
            Weapons[i] = (WeaponStruct){
                .Type = Type,
                .DMG = Weapon_DB[Type].DMG,
                .HP = Weapon_DB[Type].HP,
                .Speed = Weapon_DB[Type].Speed,
                .SwordWidth = Weapon_DB[Type].SwordWidth,
                .SwordHeight = Weapon_DB[Type].SwordHeight};

            break;
        }
    }
    SwordWidth = Weapon_DB[Type].SwordWidth;
    SwordHeight = Weapon_DB[Type].SwordHeight;
}

void Sword_Collision(float delta_time)
{
    // Update the attack cooldown
    if (Player.AttackCooldown > 0.0f)
    {
        Player.AttackCooldown -= delta_time;
    }

    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {
        if (Enemies[i].Type == Enemy_No_Enemy || Enemies[i].HP <= 0)
        {
            continue;
        }

        Rectangle Rect_Enemy = {Enemies[i].Pos.x, Enemies[i].Pos.y, Enemies[i].Size.x, Enemies[i].Size.y};

        // Checks if the sword range collides with an enemy and if it does it damages it based on the currentweapon damage
        if (CheckCollisionRecs(Rect_SwordSwing, Rect_Enemy))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && Player.AttackCooldown <= 0.0f)
            {
                // Debug code for sword rectangle
                DrawRectangleRec(Rect_SwordSwing, RED);
                if (Player.AttackCooldown <= 0.0f)
                {
                    Enemies[i].HP -= Weapon_DB[CurrentWeapon].DMG;
                    Player.AttackCooldown = 0.5f;

                    // Prints out the enemys position and HP before and after attacked
                    if (Enemies[i].HP >= 0)
                    {
                        printf("You hit enemy at position: (%f, %f), Enemy HP was: %d, Enemy current HP: %d\n", Enemies[i].Pos.x, Enemies[i].Pos.y, Enemies[i].HP + Weapons[i].DMG, Enemies[i].HP);
                    }

                    // Checks if enemy dies and prints out its HP before and after
                }
                if (Enemies[i].HP <= 0)
                {
                    printf("Enemy deafeated at position: (%f, %f), Enemy HP was: %d, Enemy is now DEAD\n", Enemies[i].Pos.x, Enemies[i].Pos.y, Enemies[i].HP + Weapons[i].DMG);
                    // Gives the player coins if enemies defeated
                    if (Enemies[i].Type == Enemy_Skeleton)
                    {
                        Player.Coins += 2;
                    }
                    if (Enemies[i].Type == Enemy_Zombie)
                    {
                        Player.Coins += 4;
                    }

                    Enemies[i].Type = Enemy_No_Enemy; // Sets the enemy's Type to no enemy which removes it
                }
            }
        }
    }
}
// _______________________________________________________________
//
//
// Shop Menu System
//
// _______________________________________________________________
void ShopMenuSystem()
{
    if (IsKeyPressed(KEY_F))
    {
        currentMenuState = GameMenu;
    }

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(PINK, 0.4f));

    int shopMenuTextWidth = MeasureText("Shop Menu", 50);
    DrawText("Shop Menu", (GetScreenWidth() / 2) - (shopMenuTextWidth / 2), 100, 50, BLACK);

    Rectangle IronSwordButton = {GetScreenWidth() / 2 - 300, 420, 200, 50};

    if (DrawButton(IronSwordButton, "Iron Sword", GRAY, LIGHTGRAY, BLACK) && Player.Coins <= 15)
    {
        CurrentWeapon = Weapon_Sword_Iron;
        Player.Coins -= 15;
    }
}
// _______________________________________________________________
//
// Clear Enemies Function
// Clears all current enemies
//
// _______________________________________________________________
void ClearEnemies()
{
    // Clear all enemies by resetting their count or emptying their array
    for (int i = 0; i < TOTAL_ENEMIES; i++)
    {
        Enemies[i].Type = Enemy_No_Enemy;
    }
}
// _______________________________________________________________
//
// Reset Game State Function
//
// _______________________________________________________________
void ResetGameState()
{
    // Reset base and player stats
    Base.HP = 200;
    Player.Coins = 0;
    CurrentWave = 0;
    WaveInProgess = false;
    EnemiesLeft = Enemy_No_Enemy; // Ensure enemies are removed

    // Reset player and base positions
    Base.Pos = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};
    Player.Pos = (Vector2){(GetScreenWidth() / 2) - 12, (GetScreenHeight() / 2) - 12};

    // Clear any enemies from the current wave
    ClearEnemies();

    // Reset other gameplay variables if needed
    LastDirection = Direction_Down;      // Reset player direction
    CurrentWeapon = Weapon_Sword_Wooden; // Reset weapon to default
}
// _______________________________________________________________
//
// Game Over System
//
// _______________________________________________________________
void DrawGameOverScreen()
{
    // Dim the background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GRAY, 0.5f));

    // Display Game Over info
    char waveText[50];
    sprintf(waveText, "You reached Wave %d", CurrentWave);
    DrawText(waveText, GetScreenWidth() / 2 - MeasureText(waveText, 40) / 2, 200, 40, WHITE);

    // WIP
    char HighscoreText[50];
    sprintf(HighscoreText, "Highscore: Wave %d", Highscore);
    DrawText(HighscoreText, GetScreenWidth() / 2 - MeasureText(HighscoreText, 20) / 2, 250, 20, WHITE);

    Rectangle tryAgainButton = {GetScreenWidth() / 2 - 100, 350, 200, 50};
    Rectangle mainMenuButton = {GetScreenWidth() / 2 - 100, 420, 200, 50};

    if (DrawButton(tryAgainButton, "Try Again", DARKGRAY, LIGHTGRAY, WHITE))
    {
        // Reset game state
        isGameResetting = true;
        ResetGameState();
        isGameResetting = false;
        currentMenuState = GameMenu;
    }

    if (DrawButton(mainMenuButton, "Main Menu", DARKGRAY, LIGHTGRAY, WHITE))
    {
        isGameResetting = true;
        ResetGameState();
        isGameResetting = false;
        currentMenuState = MainMenu;
    }
}
// _______________________________________________________________
int main()
{
    srand(time(0));

    int screenWidth = 800;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Game");

    SetTargetFPS(60);
    // _______________________________________________________________
    // Game Background color
    Color currentBackgroundColor = BLACK;
    // _______________________________________________________________
    // Places Base in the middle of the screen
    Base.Pos = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};

    // Places Player in the middle of the Base
    Player.Pos = (Vector2){(GetScreenWidth() / 2) - 12, (GetScreenHeight() / 2) - 12};

    while (!WindowShouldClose())
    {
        float Delta_Time = GetFrameTime();
        mouse = GetMousePosition();

        BeginDrawing();

        switch (currentMenuState)
        {
        case MainMenu:
            MainMenuLogic();
            break;

        case GameMenu:
            if (IsKeyPressed(KEY_F))
            {
                isGameResetting = false;
                currentMenuState = ShopMenu;
            }
            if (Base.HP <= 0)
            {
                if (CurrentWave > Highscore)
                {
                    Highscore = CurrentWave;
                    currentMenuState = GameOverMenu;
                }
            }
            else
            {
                if (!isGameResetting)
                {
                    Enemy_Update(Delta_Time, Base.Pos);
                    Sword_Collision(Delta_Time);
                    Enemy_CheckDefeated();
                }

                if (!WaveInProgess)
                {
                    Enemy_SpawnWave();
                }

                Rect_Player = (Rectangle){Player.Pos.x, Player.Pos.y, Player.Size.x, Player.Size.y};

                DrawBase();
                Enemy_Collision(Base.Pos, &Base.HP, Base.Size, Delta_Time);
                Player_Collision(Delta_Time);

                ClearBackground(currentBackgroundColor);

                Enemy_Draw(Delta_Time);

                Player.Vel = (Vector2){0, 0};

                // Player Movement
                //
                // _______________________________________________________________
                if (IsKeyDown(KEY_W))
                {
                    Player.Vel.y = -1;
                    LastDirection = Direction_UP;
                }
                if (IsKeyDown(KEY_S))
                {
                    Player.Vel.y = 1;
                    LastDirection = Direction_Down;
                }
                if (IsKeyDown(KEY_A))
                {
                    Player.Vel.x = -1;
                    LastDirection = Direction_Left;
                }
                if (IsKeyDown(KEY_D))
                {
                    Player.Vel.x = 1;
                    LastDirection = Direction_Right;
                }

                Player.Vel = Vector2Normalize(Player.Vel);
                Player.Vel = Vector2Scale(Player.Vel, Player.Speed);
                Player.Vel = Vector2Scale(Player.Vel, GetFrameTime());
                Player.Pos = Vector2Add(Player.Pos, Player.Vel);
                // _______________________________________________________________
                //
                //
                // Sword Swing Direction
                //
                // _______________________________________________________________
                Weapon_Type(CurrentWeapon);

                switch (LastDirection)
                {
                case Direction_UP: // When Player Looks Up
                {
                    Rect_SwordSwing = (Rectangle){Player.Pos.x, Player.Pos.y - SwordHeight, Player.Size.x, SwordHeight};
                }
                break;

                case Direction_Down: // When Player Looks Down
                {
                    Rect_SwordSwing = (Rectangle){Player.Pos.x, Player.Pos.y + Player.Size.y, Player.Size.x, SwordHeight};
                }
                break;

                case Direction_Left: // When Player Looks Left
                {
                    Rect_SwordSwing = (Rectangle){Player.Pos.x - SwordWidth, Player.Pos.y, SwordWidth, Player.Size.y};
                }
                break;

                case Direction_Right: // When Player Looks Right
                {
                    Rect_SwordSwing = (Rectangle){Player.Pos.x + Player.Size.x, Player.Pos.y, SwordWidth, Player.Size.y};
                }
                break;
                }
                // _______________________________________________________________
                //
                // Draws Base HP Text
                int baseHPTextWidth = MeasureText(TextFormat("Base HP: %i", Base.HP), 25);
                DrawText(TextFormat("Base HP: %i", Base.HP), ((GetScreenWidth() / 2) - (baseHPTextWidth / 2)) + 300, GetScreenHeight() / 1.05, 25, RED);
                // _______________________________________________________________
                //
                // Draws Coins Text
                int coinsTextWidth = MeasureText(TextFormat("Coins: %i", Player.Coins), 25);
                DrawText(TextFormat("Coins: %i", Player.Coins), ((GetScreenWidth() / 2) - (coinsTextWidth / 2)) + 300, GetScreenHeight() / 1.1, 25, GOLD);
                // _______________________________________________________________
                //
                // Draws Player
                DrawRectangleV(Player.Pos, Player.Size, WHITE);
                // _______________________________________________________________
                //
                // Draws Wave Text
                char WaveText[32];
                sprintf(WaveText, "Wave: %d", CurrentWave);
                DrawText(WaveText, (GetScreenWidth() / 2) - 35, 10, 20, WHITE);
                // _______________________________________________________________
            }
            break;

        case GameOverMenu:
            DrawGameOverScreen();
            break;

        case ShopMenu:
            ShopMenuSystem();
            break;

        case OptionsMenu:
            ClearBackground(DARKGRAY);

            Vector2 mousePoint = GetMousePosition();

            // Creates the buttons
            Rectangle mainButton = {GetScreenWidth() / 2 - 100, 100, 200, 40};
            Rectangle maroonButton = {GetScreenWidth() / 2 - 295, 300, 200, 40};
            Rectangle darkPurpleButton = {GetScreenWidth() / 2 - 295, 350, 200, 40};
            Rectangle navyBlueButton = {GetScreenWidth() / 2 - 295, 400, 200, 40};
            Rectangle charcoalButton = {GetScreenWidth() / 2 - 295, 450, 200, 40};
            Rectangle deepBrownButton = {GetScreenWidth() / 2 - 295, 500, 200, 40};
            Rectangle darkTealButton = {GetScreenWidth() / 2 - 295, 550, 200, 40};
            Rectangle oliveGreenButton = {GetScreenWidth() / 2 - 295, 600, 200, 40};

            // Check button clicks and handle actions
            if (DrawButton(mainButton, "Main Menu", GRAY, LIGHTGRAY, BLACK))
            {
                currentMenuState = MainMenu; // Switch to Main Menu state
            }
            if (DrawButton(maroonButton, "Maroon", (Color){128, 0, 0, 255}, LIGHTGRAY, WHITE))
            {
                currentBackgroundColor = (Color){128, 0, 0, 255}; // Change background color to Maroon
            }
            if (DrawButton(darkPurpleButton, "Dark Purple", (Color){48, 25, 52, 255}, LIGHTGRAY, WHITE))
            {
                currentBackgroundColor = (Color){48, 25, 52, 255}; // Change background color to Dark Purple
            }
            if (DrawButton(navyBlueButton, "Navy Blue", (Color){0, 0, 128, 255}, LIGHTGRAY, WHITE))
            {
                currentBackgroundColor = (Color){0, 0, 128, 255}; // Change background color to Navy Blue
            }
            if (DrawButton(charcoalButton, "Charcoal", (Color){30, 30, 30, 255}, LIGHTGRAY, WHITE))
            {
                currentBackgroundColor = (Color){30, 30, 30, 255}; // Change background color to Charcoal
            }
            if (DrawButton(deepBrownButton, "Deep Brown", (Color){61, 43, 31, 255}, LIGHTGRAY, WHITE))
            {
                currentBackgroundColor = (Color){61, 43, 31, 255}; // Change background color to Deep Brown
            }
            if (DrawButton(darkTealButton, "Dark Teal", (Color){0, 64, 64, 255}, LIGHTGRAY, WHITE))
            {
                currentBackgroundColor = (Color){0, 64, 64, 255}; // Change background color to Dark Teal
            }
            if (DrawButton(oliveGreenButton, "Olive Green", (Color){85, 107, 47, 255}, LIGHTGRAY, WHITE))
            {
                currentBackgroundColor = (Color){85, 107, 47, 255}; // Change background color to Olive Green
            }

            // Draws the title text
            int optionsTextWidth = MeasureText("Options Menu", 45);
            DrawText("Options Menu", (GetScreenWidth() / 2) - (optionsTextWidth / 2), 20, 40, BLACK);

            // Draws the commands title text
            int commandsTextWidth = MeasureText("Commands", 35);
            DrawText("Commands", (GetScreenWidth() / 2) - (commandsTextWidth / 2) + 200, (GetScreenHeight() / 2) - 175, 35, BLACK);

            // Draws the Appearance title text
            int appearanceTextWidth = MeasureText("Appearance", 35);
            DrawText("Appearance", (GetScreenWidth() / 2) - (appearanceTextWidth / 2) - 200, (GetScreenHeight() / 2) - 175, 35, BLACK);
            int backgroundColorTextWidth = MeasureText("Background Color", 20);
            DrawText("Background Color", (GetScreenWidth() / 2) - (backgroundColorTextWidth / 2) - 200, (GetScreenHeight() / 2) - 125, 20, BLACK);

            // Draws all the commands
            int controlsTextWidth = MeasureText("Controls", 30);
            DrawText("Controls", (GetScreenWidth() / 2) - (controlsTextWidth / 2) + 200, (GetScreenHeight() / 2) - 100, 30, BLACK);
            int movementTextWidth = MeasureText("Movement: -- W A S D", 20);
            DrawText("Movement: -- W A S D", (GetScreenWidth() / 2) - (movementTextWidth / 2) + 200, (GetScreenHeight() / 2) - 50, 20, BLACK);
            int attackTextWidth = MeasureText("Attack: -- LMB", 20);
            DrawText("Attack: -- LMB", (GetScreenWidth() / 2) - (attackTextWidth / 2) + 175, (GetScreenHeight() / 2), 20, BLACK);
            int shopTextWidth = MeasureText("Open Shop Menu: -- F", 20);
            DrawText("Open Shop Menu: -- F", (GetScreenWidth() / 2) - (attackTextWidth / 2) + 175, (GetScreenHeight() / 2) + 50, 20, BLACK);

            break;
        case ExitMenu:
            CloseWindow();
            exit(0);
            break;
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}