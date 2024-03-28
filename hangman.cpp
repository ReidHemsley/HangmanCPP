#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <cctype>

int main() {
    // Read the wordbank file
    std::ifstream wordbankFile("wordbank.txt");
    if (!wordbankFile) {
        std::cerr << "Failed to open wordbank file." << std::endl;
        return 1;
    }

    // Store the words from the file in a vector
    std::vector<std::string> wordbank;
    std::string word;
    while (std::getline(wordbankFile, word)) {
        wordbank.push_back(word);
    }

    // Close the wordbank file
    wordbankFile.close();

    // Seed the random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine randomEngine(seed);

    // Main game loop
    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        // Select a random word from the wordbank
        std::uniform_int_distribution<int> distribution(0, wordbank.size() - 1);
        std::string secretWord = wordbank[distribution(randomEngine)];

        // Convert the secret word to lowercase
        std::transform(secretWord.begin(), secretWord.end(), secretWord.begin(), ::tolower);

        // Create a vector to store the guessed letters
        std::vector<char> guessedLetters;

        // Initialize the number of attempts
        int attempts = 6;

        // Game loop
        while (attempts > 0) {
            // Display the current state of the word
            std::string displayWord;
            for (char letter : secretWord) {
                if (std::find(guessedLetters.begin(), guessedLetters.end(), std::tolower(letter)) != guessedLetters.end()) {
                    displayWord += letter;
                } else {
                    displayWord += "_";
                }
            }
            std::cout << "Current word: " << displayWord << std::endl;

            // Check if the player has won
            if (displayWord.find('_') == std::string::npos) {
                std::cout << "Congratulations! You won!" << std::endl;
                break; // Exit the game loop if the word has been guessed correctly
            }

            // Prompt the user for a guess
            char guess;
            std::cout << "Guess a letter: ";
            std::cin >> guess;

            // Convert the guess to lowercase
            guess = std::tolower(guess);

            // Check if the user entered more than one character
            if (std::cin.fail() || std::cin.peek() != '\n') {
                std::cout << "Invalid input. Please enter a single letter." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            // Check if the guess is correct
            if (std::find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
                std::cout << "You already guessed that letter!" << std::endl;
            } else {
                guessedLetters.push_back(guess);
                if (secretWord.find(guess) != std::string::npos) {
                    std::cout << "Correct guess!" << std::endl;
                } else {
                    std::cout << "Wrong guess!" << std::endl;
                    attempts--;
                    std::cout << "Attempts left: " << attempts << std::endl;
                }
            }

            // Check if the player has lost
            if (attempts == 0) {
                std::cout << "Game over! You lost. The word was: " << secretWord << std::endl;
                break;
            }
        }

        // Ask the user if they want to play again
        std::cout << "Do you want to play again? (y/n): ";
        std::cin >> playAgain;
    }

    return 0;
}
