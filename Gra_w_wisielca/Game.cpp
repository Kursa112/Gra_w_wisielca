#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <algorithm>

void Game::OnInit()
{
    m_guessedLetters.clear();
    srand(static_cast<unsigned int>(std::time(NULL)));
    if (!m_wordsAndCategories.empty()) {
        int randomIndex = rand() % m_wordsAndCategories.size();
        m_word = m_wordsAndCategories[randomIndex].first;
        m_category = m_wordsAndCategories[randomIndex].second;
        m_maskedWord = maskWord(m_word);
        m_gameState = GameState::START;
        m_wrongGuesses = 0;

        std::cout << "Kategoria: " << m_category << std::endl;
        std::cout << "Slowo Do Odganiecia: " << m_maskedWord << std::endl;

    }
    else {
        std::cerr << "Error: Lista Slow oraz Kategorii jest pusta." << std::endl;
        m_gameState = GameState::FINISH;
    }
}

void Game::OnInput()
{
    std::cout << "Podaj Litere: ";
    std::cin >> worde;
    worde = std::tolower(worde);

    if (std::find(m_guessedLetters.begin(), m_guessedLetters.end(), worde) == m_guessedLetters.end()) {
        m_guessedLetters.push_back(worde);

        if (m_word.find(worde) == std::string::npos) {
            ++m_wrongGuesses;
        }
    }
}

bool Game::OnUpdate(float deltaTime)
{
    // Aktualizacja zamaskowanego s³owa na podstawie odgadniêtych liter
    m_maskedWord = m_word;
    for (char& ch : m_maskedWord)
    {
        if (std::isalpha(ch) && std::find(m_guessedLetters.begin(), m_guessedLetters.end(), ch) == m_guessedLetters.end())
        {
            ch = '_';
        }
    }

    // Sprawdzenie, czy gracz odgad³ ca³e s³owo
    if (m_maskedWord == m_word)
    {
        std::cout << "Gratulacje! Odgadles slowo: " << m_word << " (Kategoria: " << m_category << ")" << std::endl;
        return false;
    }

    // Sprawdzenie, czy gracz przekroczy³ maksymaln¹ liczbê b³êdnych prób
    if (m_wrongGuesses >= MAX_WRONG_GUESSES)
    {
        std::cout << "Przegrales! Slowo to: " << m_word << " (Kategoria: " << m_category << ")" << std::endl;
        return false;
    }

    return true;
}

void Game::OnRender()
{
    std::cout << "Kategoria: " << m_category << std::endl;
    std::cout << "Slowo Do Odganiecia: " << m_maskedWord << std::endl;
    drawHangman();
}

void Game::OnShutdown()
{
    m_guessedLetters.clear();
}

// Zamiana Litery Na _
std::string Game::maskWord(const std::string& word)
{
    std::string maskedWord;
    for (char ch : word)
    {
        if (std::isalpha(ch))
        {
            maskedWord += '_';
        }
        else
        {
            maskedWord += ch;
        }
    }
    return maskedWord;
}

// Rysowanie wisielca na podstawie liczby b³êdnych prób
void Game::drawHangman() const
{
    static const std::string hangmanPics[] = {
        "  +---+\n      |\n      |\n      |\n     ===\n",
        "  +---+\n  O   |\n      |\n      |\n     ===\n",
        "  +---+\n  O   |\n  |   |\n      |\n     ===\n",
        "  +---+\n  O   |\n /|   |\n      |\n     ===\n",
        "  +---+\n  O   |\n /|\\  |\n      |\n     ===\n",
        "  +---+\n  O   |\n /|\\  |\n /    |\n     ===\n",
        "  +---+\n  O   |\n /|\\  |\n / \\  |\n     ===\n"
    };

    std::cout << hangmanPics[m_wrongGuesses] << std::endl;
}

// Funkcja do ³adowania s³ów z pliku wraz z kategoriami
void Game::loadWordsFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Blad Podczas Opalania Pliku: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(',');
        if (pos != std::string::npos && pos < line.size() - 1) {
            std::string word = line.substr(0, pos);
            std::string category = line.substr(pos + 1);
            m_wordsAndCategories.push_back(std::make_pair(word, category));
        }
    }

    file.close();
}
