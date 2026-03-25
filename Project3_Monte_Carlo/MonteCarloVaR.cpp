#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iomanip>

class MonteCarloVaR {
private:
    double portfolioValue;
    double meanReturn;     // daily mean return (decimal)
    double volatility;     // daily volatility (decimal)
    int numSimulations;
    std::vector<double> simulatedReturns;
    std::vector<double> simulatedValues;

    void runSimulation() {
        std::mt19937 generator(42);
        std::normal_distribution<double> normal(0.0, 1.0);

        for (int i = 0; i < numSimulations; i++) {
            // Generate a random daily return
            double randomReturn = meanReturn + volatility * normal(generator);
            simulatedReturns.push_back(randomReturn);

            // Calculate the portfolio value from this return
            double simValue = portfolioValue * (1 + randomReturn);
            simulatedValues.push_back(simValue);
        }

        // Sort values from worst to best
        std::sort(simulatedValues.begin(), simulatedValues.end());
        std::sort(simulatedReturns.begin(), simulatedReturns.end());
    }

public:
    // --- Constructor ---
    MonteCarloVaR(double value, double mean, double vol, int sims) {
        portfolioValue = value;
        meanReturn = mean;
        volatility = vol;
        numSimulations = sims;
        runSimulation();
    }

    // VaR at a given confidence level
    double calculateVaR(double confidence) {
        int index = (int)((1 - confidence) * numSimulations);
        return portfolioValue - simulatedValues[index];
    }

    // Expected Shortfall (average loss beyond VaR)
    double calculateES(double confidence) {
        int index = (int)((1 - confidence) * numSimulations);
        double sum = 0;
        for (int i = 0; i < index; i++) {
            sum += simulatedValues[i];
        }
        double avgWorstValue = sum / index;
        return portfolioValue - avgWorstValue;
    }

    // --- Scenario Distribution Analysis ---
    void printDistribution() {
        int totalLoss = 0;
        int smallLoss = 0;
        int mediumLoss = 0;
        int largeLoss = 0;
 
        for (int i = 0; i < numSimulations; i++) {
            double pnl = simulatedValues[i] - portfolioValue;
            if (pnl < 0) {
                totalLoss++;
                if (pnl > -portfolioValue * 0.01) smallLoss++;
                else if (pnl > -portfolioValue * 0.03) mediumLoss++;
                else largeLoss++;
            }
        }
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "     SCENARIO DISTRIBUTION" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "Total Loss Scenarios: " << totalLoss
                  << " (" << (totalLoss * 100.0 / numSimulations) << "%)" << std::endl;
        std::cout << "  Small  (< 1%):     " << smallLoss << std::endl;
        std::cout << "  Medium (1-3%):     " << mediumLoss << std::endl;
        std::cout << "  Large  (> 3%):     " << largeLoss << std::endl;
        std::cout << "Total Gain Scenarios: " << (numSimulations - totalLoss)
                  << " (" << ((numSimulations - totalLoss) * 100.0 / numSimulations) << "%)" << std::endl;
    }

    void printReport() {
        std::cout << std::fixed << std::setprecision(2);
 
        std::cout << "\n==========================================" << std::endl;
        std::cout << "       MONTE CARLO VaR ENGINE" << std::endl;
        std::cout << "==========================================" << std::endl;
        std::cout << "Portfolio Value:      " << portfolioValue << std::endl;
        std::cout << "Daily Mean Return:    " << meanReturn * 100 << "%" << std::endl;
        std::cout << "Daily Volatility:     " << volatility * 100 << "%" << std::endl;
        std::cout << "Simulations:          " << numSimulations << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "     VALUE AT RISK (VaR)" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "VaR (90%):            " << calculateVaR(0.90) << std::endl;
        std::cout << "VaR (95%):            " << calculateVaR(0.95) << std::endl;
        std::cout << "VaR (99%):            " << calculateVaR(0.99) << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "     EXPECTED SHORTFALL (ES)" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "ES  (90%):            " << calculateES(0.90) << std::endl;
        std::cout << "ES  (95%):            " << calculateES(0.95) << std::endl;
        std::cout << "ES  (99%):            " << calculateES(0.99) << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "     EXTREME SCENARIOS" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "Worst Scenario:       " << simulatedValues.front() << std::endl;
        std::cout << "Best Scenario:        " << simulatedValues.back() << std::endl;
        std::cout << "Worst Daily Loss:     " << portfolioValue - simulatedValues.front() << std::endl;
        std::cout << "Best Daily Gain:      " << simulatedValues.back() - portfolioValue << std::endl;
 
        printDistribution();
 
        std::cout << "==========================================" << std::endl;
    }
};

int main() {
    // Portfolio: 10 million naira
    // Using GTCO stats from Project 1:
    // Daily mean return: 1.08% = 0.0108
    // Daily volatility: 2.51% = 0.0251
    std::cout << "=== GTCO Portfolio ===" << std::endl;
    MonteCarloVaR gtcoVar(10000000.0, 0.0108, 0.0251, 10000);
    gtcoVar.printReport();
 
    // Example 2: Conservative US equity portfolio
    // Portfolio: $1,000,000
    // Daily mean return: 0.04%, Daily volatility: 1.2%
    std::cout << "\n=== US Equity Portfolio ===" << std::endl;
    MonteCarloVaR usVar(1000000.0, 0.0004, 0.012, 10000);
    usVar.printReport();
 
    return 0;
}