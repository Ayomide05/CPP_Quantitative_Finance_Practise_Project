/*
A C++ tool that read stock price data from CSV files,
calculates daily returns, and generates a risk report
with key metrics: average return, volatility, and max drawdown.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>


class MarketDataAnalyzer {
private:
    std::string ticker;
    std::vector<std::string> dates;
    std::vector<double> prices;
    std::vector<double> returns;

    // --- Calculation Methods (private because only the class uses them) ---

    void calculateReturns() {
        for (int i = 1; i < prices.size(); i++) {
            double dailyReturn = ((prices[i] - prices[i - 1])/ prices[i - 1]) * 100;
            returns.push_back(dailyReturn); 
        }
    }

    double calculateAvgReturn() {
        double sum = std::accumulate(returns.begin(), returns.end(), 0.0);
        return sum / returns.size();
    }

    double calculateVolatility() {
        double avg = calculateAvgReturn();
        double sumSquared = 0.0;
        for (int i =0; i < returns.size(); i++) {
            sumSquared += std::pow(returns[i] - avg, 2);
        }
        return std::sqrt(sumSquared / (returns.size() - 1));
    }

    double calculateAnnualisedVolatility() {
        return calculateVolatility() * std::sqrt(252);
    }

    double calculateMaxDrawdown() {
        double peak = prices[0];
        double maxDrawdown = 0.0;
        for (int i = 1; i < prices.size(); i++) {
            if (prices[i] > peak) {
                peak = prices[i];
            }
            double drawdown = ((peak - prices[i]) / peak) * 100;
            if (drawdown > maxDrawdown) {
                maxDrawdown = drawdown;
            }
        }
        return maxDrawdown;
    }

    double calculateTotalReturn() {
        return ((prices.back() - prices.front()) / prices.front()) * 100;
    }

public:
    // -- Constructor --
    MarketDataAnalyzer(std::string t) {
        ticker = t;
    }

    // --- Load CSV Data ----
    bool loadCSV(std::string filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cout << "Error: could not open " << filename << std::endl;
            return false;
        }

        std::string line;

        // Skip header
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string date, tickerCol, priceStr;

            std::getline(ss, date, ',');
            std::getline(ss, tickerCol, ',');
            std::getline(ss, priceStr, ',');

            dates.push_back(date);
            prices.push_back(std::stod(priceStr));
        }

        file.close();

        if (prices.empty()) {
            std::cout << "Error: No data found in file" << std::endl;
            return false;
        }

        // Calculate returns automatically after loading
        calculateReturns();

        std::cout << "Loaded " << prices.size() << " prices, " << returns.size() << "returns for " << ticker << std::endl;

        return true;
    }

    // -- Primt Daily Data ---
    void printDailyData() {
        std::cout << "\n --- DAILY DATA: " << ticker << " ---" << std::endl;
        std::cout << dates[0] << " | " << prices[0] << " | ---" << std::endl;
        for (int i = 1; i < prices.size(); i++) {
            std::cout << dates[i] << " | " << prices[i] << " | " << returns[i - 1] << "%" << std::endl;
        }
    }

    // --- Print Risk report ---
    void printRiskReport() {
        double avgReturn = calculateAvgReturn();
        double dailyVol = calculateVolatility();
        double annualVol = calculateAnnualisedVolatility();
        double maxDD = calculateMaxDrawdown();
        double totalReturn = calculateTotalReturn();

        auto minIt = std::min_element(returns.begin(), returns.end());
        auto maxIt = std::max_element(returns.begin(), returns.end());

        std::cout << "\n===========================================" << std::endl;
        std::cout << "            RISK REPORT: " << ticker << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Period:                " << dates.front() << " to " << dates.back() << std::endl;
        std::cout << "Trading Days:          " << prices.size() << std::endl;
        std::cout << "Start Price:           " << prices.front() << std::endl;
        std::cout << "End Price:             " << prices.back() << std::endl;
        std::cout << "Total Return:          " << totalReturn << "%" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Avg Daily Return:      " << avgReturn << "%" << std::endl;
        std::cout << "Best Day:              " << *maxIt << "%" << std::endl;
        std::cout << "Worst Day:             " << *minIt << "%" << std::endl;
        std::cout << "Daily Volatility:      " << dailyVol << "%" << std::endl;
        std::cout << "Annualised Volatility: " << annualVol << "%" << std::endl;
        std::cout << "Max Drawdown:          " << maxDD << "%" << std::endl;
        std::cout << "========================================" << std::endl;
    }
};

int main() {
    MarketDataAnalyzer gtco("GTCO");

    if (gtco.loadCSV("market_data.csv")) {
        gtco.printDailyData();
        gtco.printRiskReport();
    }

    return 0;
}

