#include "TexasHoldEm.hpp"
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

std::map<int, std::map<int, int>> globalResults;
std::mutex globalMutex;

int currentIterations = 0;
int totalIterations;
int numThreads;

void ProcessResults(const GameResult& result) {
    std::lock_guard<std::mutex> lock(globalMutex);
	
    int handKey = result.hand.cards[0].suit * 1000000 + result.hand.cards[0].rank * 10000 + result.hand.cards[1].suit * 100 + result.hand.cards[1].rank;
	
    globalResults[handKey][result.win_condition]++;
}

void progressThread() {
    while (currentIterations < totalIterations) {
        // Calculate progress and display the progress bar here
        double progress = static_cast<double>(currentIterations) / totalIterations;
        int barWidth = 50;
        int progressBar = static_cast<int>(progress * barWidth);
        std::cout << "Progress: [";
        for (int j = 0; j < progressBar; ++j) {
            std::cout << "=";
        }
        for (int j = progressBar; j < barWidth; ++j) {
            std::cout << " ";
        }
        std::cout << "] " << std::fixed << std::setprecision(1) << (progress * 100.0) << "%" << std::flush; // Add std::flush
        std::cout << "\r"; // Move cursor to the beginning of the line
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


void thread_function() {
    for (int i = 0; i < totalIterations / numThreads; ++i) {
        TexasHoldEm game;
        GameResult result = game.play();
        ProcessResults(result);
		currentIterations++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <totalIterations> <numThreads>" << std::endl;
        return 1;
    }

    totalIterations = std::atoi(argv[1]);
    numThreads = std::atoi(argv[2]);

    if (totalIterations <= 0 || numThreads <= 0) {
        std::cerr << "Error: Invalid arguments. Both arguments must be positive integers." << std::endl;
        return 1;
    }
	
	std::vector<std::thread> threads;

	std::thread progressbarthread(progressThread);

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numThreads; ++i) {
		threads.emplace_back(thread_function);
	}

	for (auto& th : threads) {
		th.join();
	}

	TexasHoldEm game;
	while (currentIterations < totalIterations) {
		GameResult result = game.play();
		ProcessResults(result);
		currentIterations++;
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	double durationInSeconds = static_cast<double>(duration.count()) / 1000.0;
		
	progressbarthread.join();

    std::lock_guard<std::mutex> lock(globalMutex);

    std::ofstream outputFile("hand_data.txt");
    if (!outputFile) {
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }

    for (const auto& handEntry : globalResults) {
		int handKey = handEntry.first;
		int suit1 = (handKey / 1000000) % 100;
		int rank1 = (handKey / 10000) % 100;
		int suit2 = (handKey / 100) % 100;
		int rank2 = handKey % 100;

		const std::map<int, int>& winConditions = handEntry.second;
		outputFile << "Hand: " << std::setfill('0') << std::setw(2) << suit1 << std::setw(2) << rank1 << std::setw(2) << suit2 << std::setw(2) << rank2 << std::endl;
		for (const auto& winEntry : winConditions) {
			outputFile << "  Win Condition " << winEntry.first << ": " << winEntry.second << " times" << std::endl;
		}
	}

    outputFile.close(); // Close the output file

	double progress = static_cast<double>(currentIterations) / totalIterations;
    int barWidth = 50;
    int progressBar = static_cast<int>(progress * barWidth);

    std::cout << "Progress: [";
    for (int j = 0; j < progressBar; ++j) {
        std::cout << "=";
    }
    for (int j = progressBar; j < barWidth; ++j) {
        std::cout << " ";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << (progress * 100.0) << "%" << std::endl << std::flush; // Add std::flush

    std::cout << "Execution Time: " << durationInSeconds << " seconds" << std::endl;
    double iterationsPerSecond = static_cast<double>(totalIterations) / durationInSeconds;
    std::cout << "Iterations per second: " << std::fixed << std::setprecision(2) << iterationsPerSecond << std::endl;
    std::cout << "Number of hands seen: " << globalResults.size() << std::endl;

    return 0;
}