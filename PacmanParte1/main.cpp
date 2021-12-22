#include "Map.h"
#include "Enemy.h"
#include<time.h>
#include "TimeManager.h"
#include<vector>

/// <summary>
/// Sets the needed variables
/// </summary>
void Setup();
/// <summary>
/// Handles the inputs
/// </summary>
void Input();
/// <summary>
/// Handles the logic of the game
/// </summary>
void Logic();
/// <summary>
/// Draws the screen
/// </summary>
void Draw();

enum USER_INPUTS { NONE, UP, DOWN, RIGHT, LEFT, QUIT };
Map pacman_map = Map();
std::vector<Enemy>enemigos;

char player_char = 'O';
int player_x = 1;
int player_y = 1;
int player_points = 0;
int playerLife = 3; // contador de impactos
bool gameOver = false;  // fin del juego 
USER_INPUTS input = USER_INPUTS::NONE;
bool run = true;
bool win = false;

int main()
{
   
    Setup();
    while (run)
    {
        Input();
        Logic();
        Draw();
    }
}

void Setup()
{
    std::cout.sync_with_stdio(false);
    srand(time(NULL));
    player_x = pacman_map.spawn_player.X;
    player_y = pacman_map.spawn_player.Y;
    unsigned short enemyNumber = 0;
    std::cout << "Cuantos enemigos desea";
    std::cin >> enemyNumber;
    for (size_t i = 0; i < enemyNumber; i++)
    {
        enemigos.push_back(Enemy(pacman_map.spawn_enemy));
    }
}

void Input()
{
    input = USER_INPUTS::NONE;
    if (ConsoleUtils::KeyPressed(VK_UP) || ConsoleUtils::KeyPressed('W'))
    {
        input = USER_INPUTS::UP;
    }
    if (ConsoleUtils::KeyPressed(VK_DOWN) || ConsoleUtils::KeyPressed('S'))
    {
        input = USER_INPUTS::DOWN;
    }
    if (ConsoleUtils::KeyPressed(VK_RIGHT) || ConsoleUtils::KeyPressed('D'))
    {
        input = USER_INPUTS::RIGHT;
    }
    if (ConsoleUtils::KeyPressed(VK_LEFT) || ConsoleUtils::KeyPressed('A'))
    {
        input = USER_INPUTS::LEFT;
    }
    if (ConsoleUtils::KeyPressed(VK_ESCAPE) || ConsoleUtils::KeyPressed('Q'))
    {
        input = USER_INPUTS::QUIT;
    }
}

void Logic()
{
    if (win)
    {
        switch (input)
        {
        case QUIT:
            run = false;
            break;
        }
    }
    else if (gameOver)
    {
        switch (input)
        {
        case QUIT:
            run = false;
        }
    }
    else
    {
        int player_y_new = player_y;
        int player_x_new = player_x;
        switch (input)
        {
        case UP:
            player_y_new--;
            break;
        case DOWN:
            player_y_new++;
            break;
        case RIGHT:
            player_x_new++;
            break;
        case LEFT:
            player_x_new--;
            break;
        case QUIT:
            run = false;
            break;
        }
        if (player_x_new < 0)
        {
            player_x_new = pacman_map.Width - 1;
        }
        player_x_new %= pacman_map.Width;
        if (player_y_new < 0)
        {
            player_y_new = pacman_map.Height - 1;
        }
        player_y_new %= pacman_map.Height;

        switch (pacman_map.GetTile(player_x_new, player_y_new))
        {
        case Map::MAP_TILES::MAP_WALL:
            player_y_new = player_y;
            player_x_new = player_x;
            break;
        case Map::MAP_TILES::MAP_POINT:
            pacman_map.points--;
            player_points++;
            pacman_map.SetTile(player_x_new, player_y_new, Map::MAP_TILES::MAP_EMPTY);
            break;
        case Map::MAP_TILES::MAP_POWERUP:

            player_points += 25;
            //enemy1.PowerUpPicked();
            for (size_t i = 0; i < enemigos.size(); i++)
            {
                enemigos[i].PowerUpPicked();
            }
            pacman_map.SetTile(player_x_new, player_y_new, Map::MAP_TILES::MAP_EMPTY);
            break;
        }

        player_y = player_y_new;
        player_x = player_x_new;

        for (size_t i = 0; i < enemigos.size(); i++)
        {
            Enemy::ENEMY_STATE enemy1state = enemigos[i].Update(&pacman_map, { (short)player_x,(short)player_y });
            switch (enemy1state)
            {

            case Enemy::ENEMY_KILLED:  // player mata al enemigo
                player_points += 50;
                break;

            case Enemy::ENEMY_DEAD: // enemigo mata al player
                playerLife--;
                player_x = pacman_map.spawn_player.X;
                player_y = pacman_map.spawn_player.Y;
                break;


            }
            // Si el jugador recibe tres impactos se acaba la partida
            if (playerLife < 1)
            {
                gameOver = true;
            }

            if (pacman_map.points <= 0)
            {
                win = true;
            }
            /* Enemy::ENEMY_STATE enemy1state = enemy1.Update(&pacman_map, {(short)player_x,(short)player_y});
             switch (enemy1state)
             {

             case Enemy::ENEMY_KILLED:
                 player_points += 50;
                 break;
             case Enemy::ENEMY_DEAD:
                 player_x = pacman_map.spawn_player.X;
                 player_y = pacman_map.spawn_player.Y;
                 break;

             }*/
        }
    }
        
}

void Draw()
{
    ConsoleUtils::Console_SetPos(0,0);
    pacman_map.Draw();
    ConsoleUtils::Console_SetPos(player_x, player_y);
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::DARK_YELLOW);
    std::cout << player_char;

    //enemy1.Draw();
    for (size_t i = 0; i < enemigos.size(); i++)
    {
        enemigos[i].Draw();
    }
    ConsoleUtils::Console_ClearCharacter({ 0,(short)pacman_map.Height });
    ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::CYAN);
    std::cout << "Puntuacion actual: " << player_points << " Puntuacion pendiente: " << pacman_map.points << std::endl;
    if (win)
    {
        ConsoleUtils::Console_SetColor(ConsoleUtils::CONSOLE_COLOR::GREEN);
        std::cout << "Has ganado!" << std::endl;
    }
    std::cout << "Fotogramas:" << TimeManager::getInstance().frameCount << std::endl;
    std::cout << "Time:" << TimeManager::getInstance().time << std::endl;
    std::cout << "DeltaTime:" << TimeManager::getInstance().deltaTime << std::endl;
    TimeManager::getInstance().NextFrame();
    std::cout << "Numero de vidas->" << playerLife;
    if (playerLife < 1)
    {
        std::cout << " GAME OVER PULSE Q PARA SALIR " << std::endl;
    }
}