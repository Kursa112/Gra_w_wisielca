#pragma once
#include <string>
#include <vector>

class Game
{
    enum class GameState
    {
        START,
        UPDATE,
        FINISH
    };

public:
    void OnInit();
    void OnInput();
    bool OnUpdate(float deltaTime);
    void OnRender();
    void OnShutdown();

    std::string maskWord(const std::string& word);
    void loadWordsFromFile(const std::string& filename);

private:
    void drawHangman() const;

    char worde;
    std::string m_word;
    std::string m_maskedWord;
    std::string m_category; 
    std::vector<std::pair<std::string, std::string>> m_wordsAndCategories;
    std::vector<char> m_guessedLetters;
    GameState m_gameState = GameState::START;
    int m_wrongGuesses = 0;
    const int MAX_WRONG_GUESSES = 6;
};
