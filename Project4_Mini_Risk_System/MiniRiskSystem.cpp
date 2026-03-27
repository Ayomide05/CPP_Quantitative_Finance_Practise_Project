#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iomanip>

// NORMAL DISTRIBUTION HELPERS
double normalCDF(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}
double normalPDF(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2 * M_PI);
}

// MARKET DATA LOADER
class MarketDataLoader{
private:
    std::string ticker;
    std::vector<std::string> dates;
    std::vector<double> prices;
    std::vector<double> returns;

public:
    MarketDataLoader(std::string t) { ticker = t; }

    bool loadCSV(std::string filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not open " << filename << std::endl;
            return false;
        }

        std::string line;
        std::getline(file, line);   // skip header

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

        for (int i = 1; i < prices.size(); i++) {
            returns.push_back(((prices[i] - prices[i-1]) / prices[i-1]));
        }

        return !prices.empty();
    }
    std::string getTicker() { return ticker; }
    std::vector<std::string>& getDates() { return dates; }
    std::vector<double>& getPrices() { return prices; }
    std::vector<double>& getReturns() { return returns; }
};

// Statistical Analyzer
class StatisticalAnalyzer {
private:
    std::vector<double> prices;
    std::vector<double> returns;
public:
    StatisticalAnalyzer(std::vector<double>& p, std::vector<double>& r) : prices (p), returns(r) {}
    double meanReturn() {
        return std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    }
    double dailyVolatility() {
        double avg = meanReturn();
        double sumSq = 0;
        for (double r : returns) {
            sumSq += std::pow(r - avg, 2);
        }
        return std::sqrt(sumSq / (returns.size() - 1));
    }
    
    double annualisedVolatility () {
        return dailyVolatility() * std::sqrt(252);
    }

    double maxDrawdown() {
        double peak = prices[0];
        double maxDD = 0;
        for (int i = 1; i < prices.size(); i++) {
            if (prices[i] > peak) peak = prices[i];
            double dd = ((peak - prices[i]) / peak) * 100;
            if (dd > maxDD) maxDD = dd;
        }
        return maxDD;
    }

    double totalReturn() {
        return ((prices.back() - prices.front()) / prices.front()) * 100;
    }

    double bestDay() {
        return *std::max_element(returns.begin(), returns.end()) * 100;
    }

    double worstDay() {
        return *std::min_element(returns.begin(), returns.end()) * 100;
    }

    int priceCount() { return prices.size(); }

};

// Black-Scholes Option Pricer
class OptionPricer {
private:
    double S, K, T, r, sigma, d1, d2;

    void calculateD1D2() {
        d1 = (std::log(S / K) + (r + (sigma * sigma) / 2.0) * T)
             / (sigma * std::sqrt(T));
        d2 = d1 - sigma * std::sqrt(T);
    }

public:
    OptionPricer(double stock, double strike, double time,
                 double rate, double vol) {
        S = stock; K = strike; T = time; r = rate; sigma = vol;
        calculateD1D2();
    }

    double callPrice() {
        return S * normalCDF(d1) - K * std::exp(-r * T) * normalCDF(d2);
    }

    double putPrice() {
        return K * std::exp(-r * T) * normalCDF(-d2) - S * normalCDF(-d1);
    }

    double callDelta() { return normalCDF(d1); }
    double putDelta() { return normalCDF(d1) - 1; }

    double gamma() {
        return normalPDF(d1) / (S * sigma * std::sqrt(T));
    }

    double vega() {
        return (S * normalPDF(d1) * std::sqrt(T)) / 100.0;
    }

    double getD1() { return d1; }
    double getD2() { return d2; }
};

// Monte Carlo VaR Engine
class VaREngine {
private:
    double portfolioValue;
    double meanReturn;
    double volatility;
    int numSimulations;
    std::vector<double> simulatedValues;

public:
    VaREngine(double value, double mean, double vol, int sims) {
        portfolioValue = value;
        meanReturn = mean;
        volatility = vol;
        numSimulations = sims;

        std::mt19937 generator(42);
        std::normal_distribution<double> normal(0.0, 1.0);

        for (int i = 0; i < numSimulations; i++) {
            double randReturn = meanReturn + volatility * normal(generator);
            simulatedValues.push_back(portfolioValue * (1 + randReturn));
        }
        std::sort(simulatedValues.begin(), simulatedValues.end());
    }

    double calculateVaR(double confidence) {
        int index = (int)((1 - confidence) * numSimulations);
        return portfolioValue - simulatedValues[index];
    }

    double calculateES(double confidence) {
        int index = (int)((1 - confidence) * numSimulations);
        double sum = 0;
        for (int i = 0; i < index; i++) sum += simulatedValues[i];
        return portfolioValue - (sum / index);
    }

    double worstScenario() { return simulatedValues.front(); }
    double bestScenario() { return simulatedValues.back(); }
};


//  INTEGRATED RISK REPORT
void printFullReport(MarketDataLoader &data, StatisticalAnalyzer &stats,
                     OptionPricer &pricer, VaREngine &var,
                     double portfolioValue, double strikePrice,
                     double riskFreeRate, double timeToExpiry) {

    std::cout << std::fixed << std::setprecision(4);
    
    std::cout << "\n##############################################" << std::endl;
    std::cout << "       INTEGRATED RISK REPORT" << std::endl;
    std::cout << "       " << data.getTicker() << std::endl;
    std::cout << "##############################################" << std::endl;

    // Section 1: Market Data Summary
    std::cout << "\n[1] MARKET DATA SUMMARY" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Period:              " << data.getDates().front()
              << " to " << data.getDates().back() << std::endl;
    std::cout << "Trading Days:        " << data.getPrices().size() << std::endl;
    std::cout << "Start Price:         " << data.getPrices().front() << std::endl;
    std::cout << "End Price:           " << data.getPrices().back() << std::endl;
    std::cout << "Total Return:        " << stats.totalReturn() << "%" << std::endl;

    // Section 2: Risk Statistics
    std::cout << "\n[2] RISK STATISTICS" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Avg Daily Return:    " << stats.meanReturn() * 100 << "%" << std::endl;
    std::cout << "Best Day:            " << stats.bestDay() << "%" << std::endl;
    std::cout << "Worst Day:           " << stats.worstDay() << "%" << std::endl;
    std::cout << "Daily Volatility:    " << stats.dailyVolatility() * 100 << "%" << std::endl;
    std::cout << "Annual Volatility:   " << stats.annualisedVolatility() * 100 << "%" << std::endl;
    std::cout << "Max Drawdown:        " << stats.maxDrawdown() << "%" << std::endl;

    // Section 3: Option Pricing
    std::cout << "\n[3] OPTION PRICING (Black-Scholes)" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Strike:              " << strikePrice << std::endl;
    std::cout << "Time to Expiry:      " << timeToExpiry << " years" << std::endl;
    std::cout << "Risk-Free Rate:      " << riskFreeRate * 100 << "%" << std::endl;
    std::cout << "Implied Volatility:  " << stats.annualisedVolatility() * 100 << "%" << std::endl;
    std::cout << "d1:                  " << pricer.getD1() << std::endl;
    std::cout << "d2:                  " << pricer.getD2() << std::endl;
    std::cout << "Call Price:          " << pricer.callPrice() << std::endl;
    std::cout << "Put Price:           " << pricer.putPrice() << std::endl;
    std::cout << "Call Delta:          " << pricer.callDelta() << std::endl;
    std::cout << "Put Delta:           " << pricer.putDelta() << std::endl;
    std::cout << "Gamma:               " << pricer.gamma() << std::endl;
    std::cout << "Vega:                " << pricer.vega() << std::endl;

    // Section 4: Monte Carlo VaR
    std::cout << std::setprecision(2);
    std::cout << "\n[4] MONTE CARLO VALUE AT RISK" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Portfolio Value:     " << portfolioValue << std::endl;
    std::cout << "Simulations:         10,000" << std::endl;
    std::cout << "VaR (95%):           " << var.calculateVaR(0.95) << std::endl;
    std::cout << "VaR (99%):           " << var.calculateVaR(0.99) << std::endl;
    std::cout << "ES  (95%):           " << var.calculateES(0.95) << std::endl;
    std::cout << "ES  (99%):           " << var.calculateES(0.99) << std::endl;
    std::cout << "Worst Scenario:      " << var.worstScenario() << std::endl;
    std::cout << "Best Scenario:       " << var.bestScenario() << std::endl;

    std::cout << "\n##############################################" << std::endl;
    std::cout << "          END OF REPORT" << std::endl;
    std::cout << "##############################################\n" << std::endl;
}

// MAIN

int main() {
    std::cout << "      MINI RISK SYSTEM v1.0" << std::endl;

    // ---User Inputs ---
    std::string filename, ticker;
    double portfolioValue, strikePrice, riskFreeRate, timeToExpiry;

    std::cout << "Enter CSV filename: ";
    std::cin >> filename;

    std::cout << "Enter ticker symbol: ";
    std::cin >> ticker;

    std::cout << "Enter portfolio value: ";
    std::cin >> portfolioValue;

    std::cout << "Enter option strike price: ";
    std::cin >> strikePrice;

    std::cout << "Enter risk-free rate (e.g. 0.115 for 11.5%): ";
    std::cin >> riskFreeRate;

    std::cout << "Enter time to expiry in years (e.g. 0.25): ";
    std::cin >> timeToExpiry;

    // --- Load Data ---
    MarketDataLoader data(ticker);
    if (!data.loadCSV(filename)) {
        std::cout << "Failed to load data. Exiting." << std::endl;
        return 1;
    }

    // --- Run Analysis ---
    StatisticalAnalyzer stats(data.getPrices(), data.getReturns());

    double currentPrice = data.getPrices().back();
    double annualVol = stats.annualisedVolatility();

    OptionPricer pricer(currentPrice, strikePrice, timeToExpiry,
                        riskFreeRate, annualVol);

    VaREngine var(portfolioValue, stats.meanReturn(),
                  stats.dailyVolatility(), 10000);

    // --- Print Report ---
    printFullReport(data, stats, pricer, var,
                    portfolioValue, strikePrice,
                    riskFreeRate, timeToExpiry);

    return 0;
}




