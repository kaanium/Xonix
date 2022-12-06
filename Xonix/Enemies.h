#pragma once

#include <vector>
#include <gsl/gsl>

struct Position {
    int x, y;
};

enum class Direction { LEFTU, RIGHTU, LEFTD, RIGHTD };

extern Position plaPos;
extern bool bLostHealth;
extern int MAP_Y, MAP_X;

/// ((SPEED_DIVIDER-1)/SPEED_DIVIDER) -> new speed
/// now - > 1/SPEED_DIVIDER
constexpr int SPEED_DIVIDER = 2;

class Enemies {
public:
    Position pos{};
    Direction direction;

    virtual ~Enemies() = default;

    Enemies(int y, int x) noexcept
    {
        pos.y = y;
        pos.x = x;
        direction = static_cast<Direction>(rand() % 4);
    }

    Enemies(Enemies&&) = default;
    Enemies& operator=(Enemies&&) = default;

    Enemies(Enemies const&) = default;
    Enemies& operator=(Enemies const&) = default;

    bool checkIfNearDead(std::vector<std::vector<int>> map,
                         Direction mDirection) noexcept
    {
        switch (mDirection) {
        case Direction::LEFTU:
            if (gsl::at(gsl::at(map, pos.y - 1), pos.x - 1) == 1)
                return true;
            break;

        case Direction::RIGHTU:
            if (gsl::at(gsl::at(map, pos.y - 1), pos.x + 1) == 1)
                return true;
            break;

        case Direction::LEFTD:
            if (gsl::at(gsl::at(map, pos.y + 1), pos.x - 1) == 1)
                return true;
            break;

        case Direction::RIGHTD:
            if (gsl::at(gsl::at(map, pos.y + 1), pos.x + 1) == 1)
                return true;
        }
        return false;
    }

    void InThisSpace(std::vector<std::vector<int>>& map)
    {
        if (gsl::at(gsl::at(map, pos.y + 1), pos.x + 1) == 5 ||
            gsl::at(gsl::at(map, pos.y - 1), pos.x + 1) == 5 ||
            gsl::at(gsl::at(map, pos.y + 1), pos.x - 1) == 5 ||
            gsl::at(gsl::at(map, pos.y - 1), pos.x - 1) == 5 ||
            gsl::at(gsl::at(map, pos.y + 1), pos.x) == 5 ||
            gsl::at(gsl::at(map, pos.y - 1), pos.x) == 5 ||
            gsl::at(gsl::at(map, pos.y), pos.x + 1) == 5 ||
            gsl::at(gsl::at(map, pos.y), pos.x - 1) == 5)
            return;
        std::vector<Position> filler{ { pos.x, pos.y } };

        for (size_t i = 0; i < filler.size(); ++i) {
            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y + 1),
                         gsl::at(filler, i).x + 1)) {
                filler.push_back({ gsl::at(filler, i).x + 1,
                                   gsl::at(filler, i).y + 1 });
                gsl::at(gsl::at(map, gsl::at(filler, i).y + 1),
                        gsl::at(filler, i).x + 1) = 5;
            }

            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y - 1),
                         gsl::at(filler, i).x + 1)) {
                filler.push_back({ gsl::at(filler, i).x + 1,
                                   gsl::at(filler, i).y - 1 });
                gsl::at(gsl::at(map, gsl::at(filler, i).y - 1),
                        gsl::at(filler, i).x + 1) = 5;
            }

            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y + 1),
                         gsl::at(filler, i).x - 1)) {
                filler.push_back({ gsl::at(filler, i).x - 1,
                                   gsl::at(filler, i).y + 1 });
                gsl::at(gsl::at(map, gsl::at(filler, i).y + 1),
                        gsl::at(filler, i).x - 1) = 5;
            }

            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y - 1),
                         gsl::at(filler, i).x - 1)) {
                filler.push_back({ gsl::at(filler, i).x - 1,
                                   gsl::at(filler, i).y - 1 });
                gsl::at(gsl::at(map, gsl::at(filler, i).y - 1),
                        gsl::at(filler, i).x - 1) = 5;
            }

            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y + 1),
                         gsl::at(filler, i).x)) {
                filler.push_back(
                    { gsl::at(filler, i).x, gsl::at(filler, i).y + 1 });
                gsl::at(gsl::at(map, gsl::at(filler, i).y + 1),
                        gsl::at(filler, i).x) = 5;
            }

            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y - 1),
                         gsl::at(filler, i).x)) {
                filler.push_back(
                    { gsl::at(filler, i).x, gsl::at(filler, i).y - 1 });
                gsl::at(gsl::at(map, gsl::at(filler, i).y - 1),
                        gsl::at(filler, i).x) = 5;
            }

            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y),
                         gsl::at(filler, i).x + 1)) {
                filler.push_back(
                    { gsl::at(filler, i).x + 1, gsl::at(filler, i).y });
                gsl::at(gsl::at(map, gsl::at(filler, i).y),
                        gsl::at(filler, i).x + 1) = 5;
            }

            if (!gsl::at(gsl::at(map, gsl::at(filler, i).y),
                         gsl::at(filler, i).x - 1)) {
                filler.push_back(
                    { gsl::at(filler, i).x - 1, gsl::at(filler, i).y });
                gsl::at(gsl::at(map, gsl::at(filler, i).y),
                        gsl::at(filler, i).x - 1) = 5;
            }
        }
    }

    virtual Direction movement(std::vector<std::vector<int>>& map,
                               Direction mDirection,
                               std::vector<Enemies> enemy, int z) noexcept
    {
        if ((gsl::at(gsl::at(map, pos.y - 1), pos.x) &&
             gsl::at(gsl::at(map, pos.y + 1), pos.x)) ||
            (gsl::at(gsl::at(map, pos.y), pos.x - 1) &&
             gsl::at(gsl::at(map, pos.y), pos.x + 1)) ||
            ((mDirection == Direction::RIGHTD ||
              mDirection == Direction::LEFTU) &&
             (gsl::at(gsl::at(map, pos.y - 1), pos.x - 1) > 1 &&
              gsl::at(gsl::at(map, pos.y + 1), pos.x + 1) > 1)) ||
            ((mDirection == Direction::RIGHTU ||
              mDirection == Direction::LEFTD) &&
             (gsl::at(gsl::at(map, pos.y - 1), pos.x + 1) > 1 &&
              gsl::at(gsl::at(map, pos.y + 1), pos.x - 1) > 1))) {
            gsl::at(gsl::at(map, pos.y), pos.x) = 2;
            return mDirection;
        }
        int i = 0;
        for (; i < 4; ++i) {
            if (enemy[i].pos.y == pos.y && enemy[i].pos.x == pos.x &&
                i != z)
                break;
        }
        if (i == 4)
            gsl::at(gsl::at(map, pos.y), pos.x) = 0;
        switch (mDirection) {
        case Direction::LEFTU:

            if (gsl::at(gsl::at(map, pos.y - 1), pos.x) > 1 &&
                gsl::at(gsl::at(map, pos.y), pos.x - 1) != 2) {
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x - 1) > 1 &&
                     gsl::at(gsl::at(map, pos.y - 1), pos.x) != 2) {
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y - 1), pos.x - 1) > 1) {
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else {
                --pos.x;
                --pos.y;
                break;
            }

        case Direction::LEFTD:

            if (gsl::at(gsl::at(map, pos.y + 1), pos.x) > 1 &&
                gsl::at(gsl::at(map, pos.y), pos.x - 1) != 2) {
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x - 1) > 1 &&
                     gsl::at(gsl::at(map, pos.y + 1), pos.x) != 2) {
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y + 1), pos.x - 1) > 1) {
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else {
                --pos.x;
                ++pos.y;
                break;
            }

        case Direction::RIGHTU:
            if (gsl::at(gsl::at(map, pos.y - 1), pos.x) > 1 &&
                gsl::at(gsl::at(map, pos.y), pos.x + 1) != 2) {
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x + 1) > 1 &&
                     gsl::at(gsl::at(map, pos.y - 1), pos.x) != 2) {
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y - 1), pos.x + 1) > 1) {
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else {
                ++pos.x;
                --pos.y;
                break;
            }

        case Direction::RIGHTD:
            if (gsl::at(gsl::at(map, pos.y + 1), pos.x) > 1 &&
                gsl::at(gsl::at(map, pos.y), pos.x + 1) != 2) {
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x + 1) > 1 &&
                     gsl::at(gsl::at(map, pos.y + 1), pos.x) != 2) {
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y + 1), pos.x + 1) > 1) {
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else {
                ++pos.x;
                ++pos.y;
                break;
            }
        }
        if (gsl::at(gsl::at(map, pos.y), pos.x) == 1 ||
            (plaPos.x == pos.x && plaPos.y == pos.y))
            bLostHealth = true;
        gsl::at(gsl::at(map, pos.y), pos.x) = 4;
        return mDirection;
    }
};

class TypeTwo : public virtual Enemies {
public:
    virtual ~TypeTwo() = default;
    TypeTwo(int y, int x) noexcept : Enemies(y, x) {}

    TypeTwo(TypeTwo&&) = default;
    TypeTwo& operator=(TypeTwo&&) = default;

    TypeTwo(TypeTwo const&) = default;
    TypeTwo& operator=(TypeTwo const&) = default;

    Direction movement(std::vector<std::vector<int>>& map,
                       Direction mDirection) noexcept
    {
        gsl::at(gsl::at(map, pos.y), pos.x) = 2;

        if (pos.y == 0 && pos.x == 0) {
            ++pos.y, ++pos.x;
            gsl::at(gsl::at(map, pos.y), pos.x) = 6;
            return Direction::RIGHTD;
        }

        else if (pos.y == 0 && pos.x == MAP_Y - 1) {
            ++pos.y, --pos.x;
            gsl::at(gsl::at(map, pos.y), pos.x) = 6;
            return Direction::LEFTD;
        }

        else if (pos.y == MAP_X - 1 && pos.x == 0) {
            --pos.y, ++pos.x;
            gsl::at(gsl::at(map, pos.y), pos.x) = 6;
            return Direction::RIGHTU;
        }

        else if (pos.y == MAP_X - 1 && pos.x == MAP_Y - 1) {
            --pos.y, --pos.x;
            gsl::at(gsl::at(map, pos.y), pos.x) = 6;
            return Direction::LEFTU;
        }

        switch (mDirection) {
        case Direction::LEFTU:

            if ((!pos.y && pos.x) ||
                (gsl::at(gsl::at(map, pos.y - 1), pos.x) != 2 &&
                 gsl::at(gsl::at(map, pos.y), pos.x - 1) == 2)) {
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else if ((!pos.x && pos.y) ||
                     (gsl::at(gsl::at(map, pos.y), pos.x - 1) != 2 &&
                      gsl::at(gsl::at(map, pos.y - 1), pos.x) == 2)) {
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else if ((!pos.y && !pos.x) ||
                     (gsl::at(gsl::at(map, pos.y - 1), pos.x - 1) != 2)) {
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else {
                --pos.x;
                --pos.y;
                break;
            }

        case Direction::LEFTD:

            if ((pos.y == MAP_X - 1 && pos.x) ||
                (gsl::at(gsl::at(map, pos.y + 1), pos.x) != 2 &&
                 gsl::at(gsl::at(map, pos.y), pos.x - 1) == 2)) {
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else if ((!pos.x && pos.y != MAP_X - 1) ||
                     (gsl::at(gsl::at(map, pos.y), pos.x - 1) != 2 &&
                      gsl::at(gsl::at(map, pos.y + 1), pos.x) == 2)) {
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else if ((!pos.x && pos.y == MAP_X - 1) ||
                     (gsl::at(gsl::at(map, pos.y + 1), pos.x - 1) != 2)) {
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else {
                --pos.x;
                ++pos.y;
                break;
            }

        case Direction::RIGHTU:
            if ((!pos.y && pos.x != MAP_Y - 1) ||
                (gsl::at(gsl::at(map, pos.y - 1), pos.x) != 2 &&
                 gsl::at(gsl::at(map, pos.y), pos.x + 1) == 2)) {
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else if ((pos.x == MAP_Y - 1 && pos.y) ||
                     (gsl::at(gsl::at(map, pos.y), pos.x + 1) != 2 &&
                      gsl::at(gsl::at(map, pos.y - 1), pos.x) == 2)) {
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else if ((!pos.y && pos.x == MAP_Y - 1) ||
                     (gsl::at(gsl::at(map, pos.y - 1), pos.x + 1) != 2)) {
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else {
                ++pos.x;
                --pos.y;
                break;
            }

        case Direction::RIGHTD:
            if ((pos.y == MAP_X - 1 && pos.x != MAP_Y - 1) ||
                (gsl::at(gsl::at(map, pos.y + 1), pos.x) != 2 &&
                 gsl::at(gsl::at(map, pos.y), pos.x + 1) == 2)) {
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else if ((pos.x == MAP_Y - 1 && pos.y != MAP_X - 1) ||
                     (gsl::at(gsl::at(map, pos.y), pos.x + 1) != 2 &&
                      gsl::at(gsl::at(map, pos.y + 1), pos.x) == 2)) {
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else if ((pos.x == MAP_Y - 1 && pos.y == MAP_X - 1) ||
                     (gsl::at(gsl::at(map, pos.y + 1), pos.x + 1) != 2)) {
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else {
                ++pos.x;
                ++pos.y;
                break;
            }
        }
        if (plaPos.x == pos.x && plaPos.y == pos.y)
            bLostHealth = true;
        gsl::at(gsl::at(map, pos.y), pos.x) = 6;
        return mDirection;
    }
};

class TypeThree : public virtual Enemies {
public:
    virtual ~TypeThree() = default;
    TypeThree(int y, int x) noexcept : Enemies(y, x) {}

    TypeThree(TypeThree&&) = default;
    TypeThree& operator=(TypeThree&&) = default;

    TypeThree(TypeThree const&) = default;
    TypeThree& operator=(TypeThree const&) = default;

    Direction movement(std::vector<std::vector<int>>& map,
                       Direction mDirection) noexcept
    {
        static int speed = 0;
        speed = (speed + 1) % SPEED_DIVIDER;
        if (speed != 1)

            return mDirection;

        gsl::at(gsl::at(map, pos.y), pos.x) = 0;
        switch (mDirection) {
        case Direction::LEFTU:

            if (gsl::at(gsl::at(map, pos.y - 1), pos.x) &&
                gsl::at(gsl::at(map, pos.y), pos.x - 1) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y - 1 == plaPos.y && pos.x == plaPos.x))

                    gsl::at(gsl::at(map, pos.y - 1), pos.x) = 0;
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x - 1) &&
                     gsl::at(gsl::at(map, pos.y - 1), pos.x) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y == plaPos.y && pos.x - 1 == plaPos.x))

                    gsl::at(gsl::at(map, pos.y), pos.x - 1) = 0;
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y - 1), pos.x - 1)) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y - 1 == plaPos.y && pos.x - 1 == plaPos.x))

                    gsl::at(gsl::at(map, pos.y - 1), pos.x - 1) = 0;
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else {
                --pos.x;
                --pos.y;
                break;
            }

        case Direction::LEFTD:

            if (gsl::at(gsl::at(map, pos.y + 1), pos.x) &&
                gsl::at(gsl::at(map, pos.y), pos.x - 1) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y + 1 == plaPos.y && pos.x == plaPos.x))

                    gsl::at(gsl::at(map, pos.y + 1), pos.x) = 0;
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x - 1) &&
                     gsl::at(gsl::at(map, pos.y + 1), pos.x) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y == plaPos.y && pos.x - 1 == plaPos.x))

                    gsl::at(gsl::at(map, pos.y), pos.x - 1) = 0;
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y + 1), pos.x - 1)) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y + 1 == plaPos.y && pos.x - 1 == plaPos.x))

                    gsl::at(gsl::at(map, pos.y + 1), pos.x - 1) = 0;
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else {
                --pos.x;
                ++pos.y;
                break;
            }

        case Direction::RIGHTU:
            if (gsl::at(gsl::at(map, pos.y - 1), pos.x) &&
                gsl::at(gsl::at(map, pos.y), pos.x + 1) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y - 1 == plaPos.y && pos.x == plaPos.x))

                    gsl::at(gsl::at(map, pos.y - 1), pos.x) = 0;
                ++pos.x;
                ++pos.y;
                mDirection = Direction::RIGHTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x + 1) &&
                     gsl::at(gsl::at(map, pos.y - 1), pos.x) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y == plaPos.y && pos.x + 1 == plaPos.x))

                    gsl::at(gsl::at(map, pos.y), pos.x + 1) = 0;
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y - 1), pos.x + 1)) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y - 1 == plaPos.y && pos.x + 1 == plaPos.x))

                    gsl::at(gsl::at(map, pos.y - 1), pos.x + 1) = 0;
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else {
                ++pos.x;
                --pos.y;
                break;
            }

        case Direction::RIGHTD:
            if (gsl::at(gsl::at(map, pos.y + 1), pos.x) &&
                gsl::at(gsl::at(map, pos.y), pos.x + 1) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y + 1 == plaPos.y && pos.x == plaPos.x))

                    gsl::at(gsl::at(map, pos.y + 1), pos.x) = 0;
                ++pos.x;
                --pos.y;
                mDirection = Direction::RIGHTU;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y), pos.x + 1) &&
                     gsl::at(gsl::at(map, pos.y + 1), pos.x) != 2) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y == plaPos.y && pos.x + 1 == plaPos.x))

                    gsl::at(gsl::at(map, pos.y), pos.x + 1) = 0;
                --pos.x;
                ++pos.y;
                mDirection = Direction::LEFTD;
                break;
            }
            else if (gsl::at(gsl::at(map, pos.y + 1), pos.x + 1)) {
                if (pos.y > 1 && pos.x > 1 && pos.y < MAP_X - 2 &&
                    pos.x < MAP_Y - 2 &&
                    !(pos.y + 1 == plaPos.y && pos.x + 1 == plaPos.x))
                    gsl::at(gsl::at(map, pos.y + 1), pos.x + 1) = 0;
                --pos.x;
                --pos.y;
                mDirection = Direction::LEFTU;
                break;
            }
            else {
                ++pos.x;
                ++pos.y;
                break;
            }
        }
        gsl::at(gsl::at(map, pos.y), pos.x) = 10;
        return mDirection;
    }
};