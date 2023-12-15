#include "TexasHoldEm.hpp"
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>

std::map<int, std::map<int, int>> globalResults; // To store win condition counts for each hand
std::mutex globalMutex;

void ProcessResults(const GameResult& result) {
    // Lock the mutex while updating the global results
    std::lock_guard<std::mutex> lock(globalMutex);

    int handKey = result.hand.cards[0].suit * 1000000 + result.hand.cards[0].rank * 10000 + result.hand.cards[1].suit * 100 + result.hand.cards[1].rank;

    globalResults[handKey][result.win_condition]++;
}

const int totalIterations = 5000000;
const int numThreads = 4;
// Thread function
void thread_function() {
    for (int i = 0; i < totalIterations / numThreads; ++i) {
        TexasHoldEm game;
        GameResult result = game.play();
        ProcessResults(result);
    }
}

int main() {
	std::vector<std::thread> threads;

	auto startTime = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numThreads; ++i) {
		threads.emplace_back(thread_function);
	}

	for (auto& th : threads) {
		th.join();
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	double durationInSeconds = static_cast<double>(duration.count()) / 1000.0;
	std::cout << "Execution Time for " << numThreads << " threads: " << durationInSeconds << " seconds" << std::endl;	
    

    std::lock_guard<std::mutex> lock(globalMutex);

    std::ofstream outputFile("output.txt");
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

    std::cout << "Execution Time: " << durationInSeconds << " seconds" << std::endl;
    double iterationsPerSecond = static_cast<double>(totalIterations) / durationInSeconds;
    std::cout << "Iterations per second: " << std::fixed << std::setprecision(2) << iterationsPerSecond << std::endl;
    std::cout << "Number of hands seen: " << globalResults.size() << std::endl;

    return 0;
}