#pragma once
#include <fstream>

int getHighScore(int& percentage)
{
    int m_highScore;
    std::fstream highScore("high_score.txt",
                           std::ios::in | std::ios::out | std::ios::app);
    highScore >> m_highScore;
    highScore >> percentage;
    highScore.close();
    return m_highScore;
}

void setHighScore(int score, int& m_highScore, int& percentage,
                  int& hPercentage)
{
    if (score > m_highScore) {
        m_highScore = score;
        if (hPercentage != 100) {
            hPercentage = percentage;
            std::fstream highScore("high_score.txt",
                                   std::ofstream::out |
                                       std::ofstream::trunc);
            highScore << score << '\n' << percentage;
            highScore.close();
        }
        else {
            std::fstream highScore("high_score.txt",
                                   std::ofstream::out |
                                       std::ofstream::trunc);
            highScore << score << "\n0";
            highScore.close();
        }
    }
    else if (score == m_highScore && percentage > hPercentage) {
        m_highScore = score;
        hPercentage = percentage;
        std::fstream highScore("high_score.txt",
                               std::ofstream::out | std::ofstream::trunc);
        highScore << score << '\n' << percentage;
        highScore.close();
    }
}