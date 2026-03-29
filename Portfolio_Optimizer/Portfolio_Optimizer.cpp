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

// MULTI-ASSET DATA LOADER
class MultiAssetLoader {
private:
    std::vector<std::string> tickers;
    std::vector<std::vector<double>> prices;     // prices[asset][day]
    std::vector<std::vector<double>> returns;    // returns[asset][day]
    int numAssets;
    int numDays;

public:
    bool loadCSV(std::string filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not open " << filename << std::endl;
            return false;
        }

        // Read header to get ticker names
        std::string line;
        std::getline(file, line);
        std::stringstream headerSS(line);
        std::string token;
        std::getline(headerSS, token, ',');   // skip "Date"
        while (std::getline(headerSS, token, ',')) {
            tickers.push_back(token);
            prices.push_back(std::vector<double>());
        }
        numAssets = tickers.size();

        // Read price data
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::getline(ss, token, ',');   // skip date
            for (int i = 0; i < numAssets; i++) {
                std::getline(ss, token, ',');
                prices[i].push_back(std::stod(token));
            }
        }
        file.close();
        numDays = prices[0].size();

        // Calculate returns for each asset
        for (int a = 0; a < numAssets; a++) {
            returns.push_back(std::vector<double>());
            for (int d = 1; d < numDays; d++) {
                double ret = (prices[a][d] - prices[a][d-1]) / prices[a][d-1];
                returns[a].push_back(ret);
            }
        }

        std::cout << "loaded " << numAssets << " assets, " << numDays << " days" << std::endl;
        return true;
    }

    int getNumAssets() { return numAssets; }
    std::vector<std::string>& getTickers() { return tickers; }
    std::vector<std::vector<double>>& getReturns() { return returns; }
    std::vector<std::vector<double>>& getPrices() { return prices; }

};

// PORTFOLIO OPTIMIZER
class PortfolioOptimizer {
private:
    int numAssets;
    std::vector<std::string> tickers;
    std::vector<double> meanReturns;
    std::vector<std::vector<double>> covMatrix;
    double riskFreeRate;

    // Calculate mean return for each asset
    void calculateMeanReturns(std::vector<std::vector<double>>& returns) {
        for (int a =0; a < numAssets; a++) {
            double sum = std::accumulate(returns[a].begin(), returns[a].end(), 0.0);
            meanReturns.push_back(sum / returns[a].size());
        }
    }

    // calculate covariance matrix
    void calculateCovMatrix(std::vector<std::vector<double>>& returns) {
        int n = returns[0].size();
        covMatrix.resize(numAssets, std::vector<double>(numAssets, 0.0));

        for (int i = 0; i < numAssets; i++) {
            for (int j = 0; j < numAssets; j++) {
                double sum = 0;
                for (int d = 0; d < n; d++) {
                    sum += (returns[i][d] - meanReturns[i]) * (returns[j][d] - meanReturns[j]);
                }
                covMatrix[i][j] = sum / (n - 1);
            }
        }
    }

    // calculate portfolio return given weights
    double portfolioReturn(std::vector<double>& weights) {
        double ret = 0;
        for (int i =0; i < numAssets; i++) {
            ret += weights[i] * meanReturns[i];
        }
        return ret;
    }

    // Calculate portfolio volatility given weights
    double portfolioVolatility(std::vector<double>& weights) {
        double variance = 0;
        for (int i = 0; i < numAssets; i++) {
            for (int j = 0; j < numAssets; j++) {
                variance += weights[i] * weights[j] * covMatrix[i][j];
            }
        }
        return std::sqrt(variance);
    }

    // Calculate Sharpe ratio
    double sharpeRatio(std::vector<double>& weights) {
        double ret = portfolioReturn(weights);
        double vol = portfolioVolatility(weights);
        if (vol == 0) return 0;
        return (ret - riskFreeRate / 252.0) / vol;
    }

public:
    PortfolioOptimizer(std::vector<std::string>& t, std::vector<std::vector<double>>& returns, double rfRate) {
        tickers = t;
        numAssets = t.size();
        riskFreeRate = rfRate;
        calculateMeanReturns(returns);
        calculateCovMatrix(returns);
    }

    // Generate random weights that sum to 1.0
    std::vector<double> randomWeights(std::mt19937& gen) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        std::vector<double> weights(numAssets);
        double sum = 0;
        for (int i = 0; i < numAssets; i++) {
            weights[i] = dist(gen);
            sum += weights[i];
        }
        for (int i = 0; i < numAssets; i++) {
            weights[i] /= sum;   // normalize so they sum to 1.0
        }
        return weights;

    }

    // Find optimal portfolio by simulating many random portfolios
    void optimize(int numPortfolios = 50000) {
        std::mt19937 gen(42);

        double bestSharpe = -9999;
        std::vector<double> bestWeights;
        double bestReturn = 0;
        double bestVol = 0;

        double minVolSharpe = -9999;
        std::vector<double> minVolWeights;
        double minVol = 9999;
        double minVolReturn = 0;

        // Store all portfolios for display
        std::vector<double> allReturns;
        std::vector<double> allVols;

        for (int p = 0; p < numPortfolios; p++) {
            std::vector<double> w = randomWeights(gen);
            double ret = portfolioReturn(w);
            double vol = portfolioVolatility(w);
            double sr = sharpeRatio(w);

            allReturns.push_back(ret);
            allVols.push_back(vol);

            // Track best Sharpe ratio (optimal portfolio)
            if (sr > bestSharpe) {
                bestSharpe = sr;
                bestWeights = w;
                bestReturn = ret;
                bestVol = vol;
            }

            // Track minimum volatility portfolio
            if (vol < minVol) {
                minVol = vol;
                minVolWeights = w;
                minVolReturn = ret;
                minVolSharpe = sr;
            }
        }

        // Equal weight portfolio for comparison
        std::vector<double> equalWeights(numAssets, 1.0 / numAssets);
        double eqReturn = portfolioReturn(equalWeights);
        double eqVol = portfolioVolatility(equalWeights);
        double eqSharpe = sharpeRatio(equalWeights);

        // Print report
        printReport(bestWeights, bestReturn, bestVol, bestSharpe, minVolWeights, minVolReturn,
             minVol, minVolSharpe, equalWeights, eqReturn, eqVol, eqSharpe, numPortfolios);
    }

    void printReport(std::vector<double>& bestW, double bestRet, double bestVol, double bestSR,
                    std::vector<double>& mvW, double mvRet, double mvVol, double mvSR,
                    std::vector<double>& eqW, double eqRet, double eqVol, double eqSR,
                    int numPortfolios) {
                        std::cout << std::fixed << std::setprecision(4);

        std::cout << "\n##############################################" << std::endl;
        std::cout << "    MULTI-ASSET PORTFOLIO OPTIMIZER" << std::endl;
        std::cout << "##############################################" << std::endl;

        // Asset summary
        std::cout << "\n[1] ASSET SUMMARY" << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
        std::cout << std::setw(10) << "Ticker" << " | "
                  << std::setw(12) << "Mean Return" << " | "
                  << std::setw(12) << "Volatility" << std::endl;
        std::cout << "-----------+--------------+--------------" << std::endl;
        for (int i = 0; i < numAssets; i++) {
            double assetVol = std::sqrt(covMatrix[i][i]);
            std::cout << std::setw(10) << tickers[i] << " | "
                      << std::setw(11) << meanReturns[i] * 100 << "%" << " | "
                      << std::setw(11) << assetVol * 100 << "%" << std::endl;
        }

        // Correlation matrix
        std::cout << "\n[2] CORRELATION MATRIX" << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
        std::cout << std::setw(10) << " ";
        for (int i = 0; i < numAssets; i++) {
            std::cout << std::setw(8) << tickers[i];
        }
        std::cout << std::endl;
        for (int i = 0; i < numAssets; i++) {
            std::cout << std::setw(10) << tickers[i];
            for (int j = 0; j < numAssets; j++) {
                double corr = covMatrix[i][j] /
                    (std::sqrt(covMatrix[i][i]) * std::sqrt(covMatrix[j][j]));
                std::cout << std::setw(8) << std::setprecision(2) << corr;
            }
            std::cout << std::endl;
        }

        std::cout << std::setprecision(4);

        // Optimal portfolio (Max Sharpe)
        std::cout << "\n[3] OPTIMAL PORTFOLIO (Max Sharpe Ratio)" << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
        std::cout << "Portfolios simulated: " << numPortfolios << std::endl;
        for (int i = 0; i < numAssets; i++) {
            std::cout << std::setw(10) << tickers[i] << ": "
                      << std::setw(7) << bestW[i] * 100 << "%" << std::endl;
        }
        std::cout << "Expected Daily Return: " << bestRet * 100 << "%" << std::endl;
        std::cout << "Daily Volatility:      " << bestVol * 100 << "%" << std::endl;
        std::cout << "Annual Return:         " << bestRet * 252 * 100 << "%" << std::endl;
        std::cout << "Annual Volatility:     " << bestVol * std::sqrt(252) * 100 << "%" << std::endl;
        std::cout << "Sharpe Ratio:          " << bestSR << std::endl;

        // Minimum Volatility portfolio
        std::cout << "\n[4] MINIMUM VOLATILITY PORTFOLIO" << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
        for (int i = 0; i < numAssets; i++) {
            std::cout << std::setw(10) << tickers[i] << ": "
                      << std::setw(7) << mvW[i] * 100 << "%" << std::endl;
        }
        std::cout << "Expected Daily Return: " << mvRet * 100 << "%" << std::endl;
        std::cout << "Daily Volatility:      " << mvVol * 100 << "%" << std::endl;
        std::cout << "Annual Return:         " << mvRet * 252 * 100 << "%" << std::endl;
        std::cout << "Annual Volatility:     " << mvVol * std::sqrt(252) * 100 << "%" << std::endl;
        std::cout << "Sharpe Ratio:          " << mvSR << std::endl;

        // Equal weight comparison
        std::cout << "\n[5] EQUAL WEIGHT PORTFOLIO (Benchmark)" << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
        for (int i = 0; i < numAssets; i++) {
            std::cout << std::setw(10) << tickers[i] << ": "
                      << std::setw(7) << eqW[i] * 100 << "%" << std::endl;
        }
        std::cout << "Expected Daily Return: " << eqRet * 100 << "%" << std::endl;
        std::cout << "Daily Volatility:      " << eqVol * 100 << "%" << std::endl;
        std::cout << "Annual Return:         " << eqRet * 252 * 100 << "%" << std::endl;
        std::cout << "Annual Volatility:     " << eqVol * std::sqrt(252) * 100 << "%" << std::endl;
        std::cout << "Sharpe Ratio:          " << eqSR << std::endl;

        std::cout << "\n##############################################\n" << std::endl;
    }

    // Print covariance matrix (for debugging)
    void printCovMatrix() {
        std::cout << "\nCovariance Matrix:" << std::endl;
        for (int i = 0; i < numAssets; i++) {
            for (int j = 0; j < numAssets; j++) {
                std::cout << std::scientific << std::setprecision(4)
                          << covMatrix[i][j] << "  ";
            }
            std::cout << std::endl;
        }
    }
};

// MAIN

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "  MULTI-ASSET PORTFOLIO OPTIMIZER" << std::endl;
    std::cout << "============================================\n" << std::endl;

    // Load data
    MultiAssetLoader loader;
    if (!loader.loadCSV("portfolio_data.csv")) {
        return 1;
    }

    // Optimize with Nigeria T-bill rate (11.5%)
    PortfolioOptimizer optimizer(loader.getTickers(),
                                 loader.getReturns(),
                                 0.115);

    optimizer.optimize(50000);

    return 0;
}