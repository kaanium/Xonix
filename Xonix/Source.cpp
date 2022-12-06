// TODO:
// -------
//
// CODE: fix x, y; comments
//
// -------
//
/// FIXME: green together movement; stuck red; left green spot
/// immediate Turn pink point
//
// -------
//
// GAME: new record sound;
// extra life
//
// -------
//
// VISUAL: fix tearing; gui?
//
// -------

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include "Enemies.h"
#include "high_score.h"
#include "sounds.h"

#pragma comment(lib, "winmm.lib")

constexpr auto RESET = "\033[0m";
constexpr auto B_RED = "\033[41m";
constexpr auto B_CYAN = "\033[46m";
constexpr auto B_BLUE = "\033[44m";
constexpr auto B_PURPLE = "\033[45m";
constexpr auto B_WHITE = "\033[47m";
constexpr auto B_GREEN = "\033[42m";

int MAP_Y = 90, MAP_X = 49;

int MAX_PERCENTAGE = 100;

constexpr auto TIME_LIMIT = 90 * CLOCKS_PER_SEC;
constexpr auto WIN_PERCENTAGE = 75;

static int ENEMY_COUNT = 2, PERCENTAGE;
static double TURN_TIME_LIMIT = .05;
static int LIVES = 3, ROUND_COUNT = 1, HIGH_SCORE, H_PERCENTAGE;

const std::string WORD_LIST[] = { "Dodge!", "Sick!", "Cool!", "gg ez",
                                  "Badass!" };

const char* RANDOM_COLOR[] = { "\033[1;91m", "\033[1;92m", "\033[1;93m",
                               "\033[1;94m", "\033[1;95m", "\033[1;96m",
                               "\033[1;97m" };

const std::wstring WOAH_LIST[] = { L"woa1.mp3", L"woa2.mp3", L"woa3.mp3" };

enum class Directions {
    UP = 'w',
    DOWN = 's',
    LEFT = 'a',
    RIGHT = 'd',
    RESET = 'r'
};

Position plaPos;
Position prePlaPos;

bool bReset = false;
bool bLostHealth = false;
bool bTimeTrigger = false;
bool bCoolness = false;
bool bClockTrigger = false;

void ResetPosition(int x, int y) noexcept
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { gsl::narrow_cast<SHORT>(x),
                    gsl::narrow_cast<SHORT>(y) };
    SetConsoleCursorPosition(hOut, coord);
}

void ResetGame()
{
    ENEMY_COUNT = 2;
    ROUND_COUNT = 1;
    LIVES = 3;
    TURN_TIME_LIMIT = .05;
    bReset = true;
    bLostHealth = false;
    bCoolness = false;
    ResetPosition(95, 0);
    std::cout << "            ";
}

bool checkWin(std::vector<std::vector<int>> map)
{
    double count = 0;
    for (int x = 0; x < MAP_X; ++x)
        for (int y = 0; y < MAP_Y; ++y)
            if (gsl::at(gsl::at(map, x), y) == 2)
                ++count;
    ResetPosition(0, 0);
    PERCENTAGE =
        static_cast<int>(100 * ((count - 4 * (MAP_X + MAP_Y - 4)) /
                                ((MAP_X - 4) * (MAP_Y - 4))));
    std::cout << "FULL: %" << PERCENTAGE;
    return (PERCENTAGE >= WIN_PERCENTAGE);
}

bool checkTime()
{
    static clock_t start = clock();
    if (bReset && !bTimeTrigger) {
        start = clock();
        bReset = false;
    }
    else if (bTimeTrigger) {
        bReset = false;
        bTimeTrigger = false;
    }

    ResetPosition(15, 0);
    std::cout << "TIME LEFT: "
              << (TIME_LIMIT - (clock() - start)) / CLOCKS_PER_SEC
              << "   ";
    return (clock() - start >= TIME_LIMIT);
}

void HideCursor(bool showFlag) noexcept
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void UpdateMap(std::vector<std::vector<int>>& map,
               std::vector<Enemies> enemy)
{
    static std::vector<std::vector<int>> prevMap;

    if (bReset)
        prevMap = std::vector<std::vector<int>>{};

    for (int x = 0; x < MAP_X; ++x) {
        for (int y = 0; y < MAP_Y; ++y) {
            if (prevMap.size() &&
                gsl::at(gsl::at(map, x), y) ==
                    gsl::at(gsl::at(prevMap, x), y) &&
                !(x == plaPos.y && y == plaPos.x)) {
                continue;
            }

            ResetPosition(2 * y, x + 1);

            if (gsl::at(gsl::at(map, x), y) == 2) {
                if (x == plaPos.y && y == plaPos.x)
                    std::cout << B_PURPLE << "  " << RESET;
                else
                    std::cout << B_CYAN << "  " << RESET;
            }
            else {
                bool isWritten = false;
                for (int index = 0; index < ENEMY_COUNT; ++index) {
                    if (x == gsl::at(enemy, index).pos.y &&
                        y == gsl::at(enemy, index).pos.x) {
                        isWritten = true;
                        std::cout << B_GREEN << "  " << RESET;
                        break;
                    }
                }

                if (x == plaPos.y && y == plaPos.x && !isWritten) {
                    if (gsl::at(gsl::at(map, x), y) == 1)
                        bLostHealth = true;
                    std::cout << B_WHITE << "  " << RESET;
                    gsl::at(gsl::at(map, x), y) = 1;
                }
                else if (!isWritten) {
                    if (!(gsl::at(gsl::at(map, x), y)))
                        std::cout << "  ";
                    else if (gsl::at(gsl::at(map, x), y) == 6)
                        std::cout << B_RED << "  " << RESET;
                    else if (gsl::at(gsl::at(map, x), y) == 10)
                        std::cout << B_BLUE << "  " << RESET;
                    else
                        std::cout << B_PURPLE << "  " << RESET;
                }
            }
        }
    }
    prevMap = map;
}

int fillSpacesWOEnemies(std::vector<std::vector<int>>& map,
                        Directions yon) noexcept
{
    int count = 0;
    for (int x = 1; x < MAP_X - 1; ++x) {
        for (int y = 1; y < MAP_Y - 1; ++y) {
            if (gsl::at(gsl::at(map, x), y) == 0 ||
                gsl::at(gsl::at(map, x), y) == 1) {
                gsl::at(gsl::at(map, x), y) = 2;
                ++count;
            }
            else if (gsl::at(gsl::at(map, x), y) == 5)
                gsl::at(gsl::at(map, x), y) = 0;
        }
    }
    if (count < 150)
        count = 150;
    return (count < 650) ? count / 10 : 65;
}

void PlayerMovement(std::vector<std::vector<int>>& map,
                    std::vector<Enemies> enemy, TypeThree eater)
{
    static bool bOutside = false, bMoving = false;
    bool bTrigger = false;
    static Directions prevDir{};

    if (bReset) {
        bOutside = false;
        bMoving = false;
        prevDir = Directions::RESET;
        prePlaPos.x = MAP_Y / 2, prePlaPos.y = 0;
    }

    Directions dir{};

    const clock_t start = clock();
    while (!_kbhit()) {
        if (((static_cast<double>(clock()) - start) / CLOCKS_PER_SEC) >=
            TURN_TIME_LIMIT) {
            if (bMoving) {
                bTrigger = true;
                goto keepMoving;
            }
            else
                return;
        }
    }

    dir = static_cast<Directions>(_getch());
    if (prevDir == dir)
        while (((static_cast<double>(clock()) - start) / CLOCKS_PER_SEC) <
               TURN_TIME_LIMIT)
            ;
    bMoving = true;
keepMoving:;
    if (bTrigger)
        dir = prevDir;

    if (prevDir == Directions::UP && dir == Directions::DOWN && bOutside)
        dir = Directions::UP;
    if (prevDir == Directions::DOWN && dir == Directions::UP && bOutside)
        dir = Directions::DOWN;
    if (prevDir == Directions::LEFT && dir == Directions::RIGHT &&
        bOutside)
        dir = Directions::LEFT;
    if (prevDir == Directions::RIGHT && dir == Directions::LEFT &&
        bOutside)
        dir = Directions::RIGHT;
    switch (dir) {
    case Directions::UP:
        if (plaPos.y)
            --plaPos.y;
        break;

    case Directions::DOWN:
        if (plaPos.y != MAP_X - 1)
            ++plaPos.y;
        break;

    case Directions::RIGHT:
        if (plaPos.x != MAP_Y - 1)
            ++plaPos.x;
        break;

    case Directions::LEFT:
        if (plaPos.x)
            --plaPos.x;
        break;

    case Directions::RESET:
        LIVES = 1;
        return;
    }

    if (gsl::at(gsl::at(map, plaPos.y), plaPos.x) == 2 && bOutside) {
        for (int i = 0; i < ENEMY_COUNT; ++i)
            gsl::at(enemy, i).InThisSpace(map);
        eater.InThisSpace(map);
        std::wstring volume =
            std::to_wstring(fillSpacesWOEnemies(map, dir));
        bOutside = false;
        bMoving = false;
        MyPlaySound(L"score.mp3", volume);
    }
    else if (gsl::at(gsl::at(map, prePlaPos.y), prePlaPos.x) != 2 &&
             !bOutside)
        bOutside = true;
    prevDir = dir;
    ResetPosition(2 * prePlaPos.x, 1 + prePlaPos.y);
    if (gsl::at(gsl::at(map, prePlaPos.y), prePlaPos.x) == 2)
        std::cout << B_CYAN << "  " << RESET;
    else
        std::cout << B_PURPLE << "  " << RESET;

    prePlaPos = plaPos;
}

void SetupMap(std::vector<std::vector<int>>& map)
{
    system("cls");
    map.push_back(std::vector<int>());
    for (int x = 0; x < MAP_X; ++x) {
        for (int y = 0; y < MAP_Y; ++y) {
            if (x < 2 || x > MAP_X - 3 || y < 2 || y > MAP_Y - 3)
                gsl::at(map, x).push_back(2);
            else
                gsl::at(map, x).push_back(0);
        }

        map.push_back(std::vector<int>());
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string pause = "";

    static bool bStart = true;

    if (!bReset) {
        ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
        SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN,
                    0x20000000);
        SetSoundPath();

        PlayMusic(L"song.mp3");
    }

    plaPos.x = MAP_Y / 2, plaPos.y = 0;
    prePlaPos = plaPos;
    HIGH_SCORE = getHighScore(H_PERCENTAGE);

    if (bStart)
        for (int i = HIGH_SCORE - 3; i > 0; --i) {
            ++ENEMY_COUNT;
            ++ROUND_COUNT;
            TURN_TIME_LIMIT *= 0.85;
        }
    bStart = false;

    srand(gsl::narrow_cast<unsigned int>(time(NULL)));
    HideCursor(false);

    std::vector<Enemies> enemy;
    for (int i = 0; i < ENEMY_COUNT; ++i)
        enemy.emplace_back(rand() % (MAP_X - 4) + 2,
                           rand() % (MAP_Y - 4) + 2);

    TypeTwo insideEnemy{ MAP_X - 1, MAP_Y / 2 };
    TypeThree eaterEnemy{ rand() % (MAP_X - 4) + 2,
                          rand() % (MAP_Y - 4) + 2 };

    std::vector<std::vector<int>> map;

    SetupMap(map);

    while (true) {
        for (auto& i : enemy) {
            if (i.checkIfNearDead(map, i.direction))
                bCoolness = true;
        }

        PlayerMovement(map, enemy, eaterEnemy);

        eaterEnemy.direction =
            eaterEnemy.movement(map, eaterEnemy.direction);

        for (int i = 0; i < ENEMY_COUNT; ++i) {
            gsl::at(enemy, i).direction = gsl::at(enemy, i).movement(
                map, gsl::at(enemy, i).direction, enemy, i);
        }

        insideEnemy.direction =
            insideEnemy.movement(map, insideEnemy.direction);

        UpdateMap(map, enemy);

        ResetPosition(35, 0);

        std::cout << "LIVES: " << LIVES << "        ROUND: " << ROUND_COUNT
                  << "         HIGH SCORE:" << HIGH_SCORE << " / %"
                  << H_PERCENTAGE;
        if (bLostHealth) {
            --LIVES;
            bLostHealth = false;
            bReset = true;
            if (LIVES) {
                MyPlaySound(L"hit.mp3", L"30");
                ResetPosition(95, 0);
                std::cout << "           ";
                bTimeTrigger = true;
                bCoolness = false;
                plaPos.x = MAP_Y / 2, plaPos.y = 0;
                for (int x = 2; x < MAP_X - 2; ++x)
                    for (int y = 2; y < MAP_Y - 2; ++y)
                        if (gsl::at(gsl::at(map, x), y) == 1)
                            gsl::at(gsl::at(map, x), y) = 0;
            }
            else {
                MyPlaySound(L"lost.mp3", L"30");
                setHighScore(ROUND_COUNT, HIGH_SCORE, PERCENTAGE,
                             H_PERCENTAGE);
                ResetGame();
                bStart = true;
                return main();
            }
        }
        else if (checkTime()) {
            MyPlaySound(L"lost.mp3", L"30");
            setHighScore(ROUND_COUNT, HIGH_SCORE, PERCENTAGE,
                         H_PERCENTAGE);
            ResetGame();
            bStart = true;
            return main();
        }
        else if (checkWin(map)) {
            system("cls");
            MyPlaySound(L"passed.mp3", L"30");
            std::cout << "Win! Please press enter to continue to the next "
                         "round.";
            std::getline(std::cin, pause);
            bReset = true;
            ++ENEMY_COUNT;
            ++ROUND_COUNT;
            TURN_TIME_LIMIT *= 0.85;
            setHighScore(ROUND_COUNT, HIGH_SCORE, PERCENTAGE,
                         MAX_PERCENTAGE);
            return main();
        }

        if (bCoolness) {
            static clock_t start;
            static std::string chosenWord;
            if (!bClockTrigger) {
                bClockTrigger = true;
                start = clock();
                chosenWord = gsl::at(WORD_LIST, rand() % 5);
                MyPlaySound(gsl::at(WOAH_LIST, rand() % 3), L"30");
            }
            if (((clock() - start) / CLOCKS_PER_SEC) <= 3) {
                ResetPosition(95, 0);
                std::cout << gsl::at(RANDOM_COLOR, rand() % 7)
                          << chosenWord << RESET;
            }
            else {
                bCoolness = false;
                bClockTrigger = false;
                ResetPosition(95, 0);
                std::cout << "           ";
            }
        }
    }
}