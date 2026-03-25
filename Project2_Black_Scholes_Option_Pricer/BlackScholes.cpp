#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>

// Standard normal cumulative distribution function
double normalCDF(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}
// Standard normal probability density function
double normalPDF(double x) {
    return std::exp(-0.5 * x * x) / std::sqrt(2 * M_PI);
}

class BlackScholes {
private:
    double S;    // Stock Price
    double K;    // Strike Price
    double T;    // Time to expiry (years)
    double r;    // Risk-free rate (decimal)
    double sigma;  // Volatility (decimal)
    double d1;
    double d2;

    void calculateD1D2() {
        d1 = (std::log(S / K) + (r + (sigma * sigma) / 2.0) * T) / (sigma * std::sqrt(T));
        d2 = d1 - sigma * std::sqrt(T);
    }

public:
    //  ---- Constructor ----
    BlackScholes(double stock, double strike, double time, double rate, double vol) {
        S = stock;
        K = strike;
        T = time;
        r = rate;
        sigma = vol;
        calculateD1D2();
    }

    // --- Option Prices ---
    double callPrice() {
        return S * normalCDF(d1) - K * std::exp(-r * T) * normalCDF(d2);
    }
    double putPrice() {
        return K * std::exp(-r * T) * normalCDF(-d2) - S * normalCDF(-d1);
    }
    
    // --- Greeks ---
    // Delta -  how much option price changes per ₦1 move in stock
    double callDelta() {
        return normalCDF(d1);
    }
    double putDelta() {
        return normalCDF(d1) -1;
    }

    // Gamma - how much delta changes per ₦1 move in stock
    double gamma() {
        double nd1 = std::exp(-0.5 * d1 * d1) / std::sqrt(2 * M_PI);
        return nd1 / (S * sigma * std::sqrt(T));
    }

    // Theta - time decay per day
    double callTheta() {
        double nd1 = std::exp(-0.5 * d1 * d1) / std::sqrt(2 * M_PI);
        double term1 = -(S * nd1 * sigma) / (2 * std::sqrt(T));
        double term2 = r * K * std::exp(-r * T) * normalCDF(d2);
        return (term1 - term2) / 365.0;
    }

    double putTheta() {
        double nd1 = std::exp(-0.5 * d1 *d1) / std::sqrt(2 * M_PI);
        double term1 = -(S * nd1 * sigma) / (2 * std::sqrt(T));
        double term2 = r * K * std::exp(-r * T) * normalCDF(-d2);
        return (term1 + term2) / 365.0;
    }

    // Vega - sensitivity to 1% change in volatility
    double vega() {
        double nd1 = std::exp(-0.5 * d1 * d1) / std::sqrt(2 * M_PI);
        return (S * nd1 * std::sqrt(T)) / 100.0;
    }

    // Rho — sensitivity to 1% change in interest rate
    double callRho() {
        return (K * T * std::exp(-r * T) * normalCDF(d2)) / 100.0;
    }

    double putRho() {
        return -(K * T * std::exp(-r * T) * normalCDF(-d2)) / 100.0;
    }

    // --- Put-Call Parity Check ---
    double putCallParityLHS() {
        return callPrice() - putPrice();
    }

    double putCallParityRHS() {
        return S - K * std::exp(-r * T);
    }

    // -- Report ----
    void printReport() {
        std::cout << std::fixed << std::setprecision(4);
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "   BLACK-SCHOLES OPTION PRICER" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Stock Price (S):    " << S << std::endl;
        std::cout << "Strike Price (K):   " << K << std::endl;
        std::cout << "Time to Expiry (T): " << T << " years" << std::endl;
        std::cout << "Risk-Free Rate (r): " << r * 100 << "%" << std::endl;
        std::cout << "Volatility (sigma): " << sigma * 100 << "%" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "d1:                 " << d1 << std::endl;
        std::cout << "d2:                 " << d2 << std::endl;
        std::cout << "N(d1):              " << normalCDF(d1) << std::endl;
        std::cout << "N(d2):              " << normalCDF(d2) << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Call Price:          " << callPrice() << std::endl;
        std::cout << "Put Price:           " << putPrice() << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "         GREEKS" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Call Delta:          " << callDelta() << std::endl;
        std::cout << "Put Delta:           " << putDelta() << std::endl;
        std::cout << "Gamma:               " << gamma() << std::endl;
        std::cout << "Call Theta (daily):  " << callTheta() << std::endl;
        std::cout << "Put Theta (daily):   " << putTheta() << std::endl;
        std::cout << "Vega (per 1% vol):   " << vega() << std::endl;
        std::cout << "Call Rho (per 1%):   " << callRho() << std::endl;
        std::cout << "Put Rho (per 1%):    " << putRho() << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "   PUT-CALL PARITY CHECK" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "C - P =               " << putCallParityLHS() << std::endl;
        std::cout << "S - K*e^(-rT) =       " << putCallParityRHS() << std::endl;
        std::cout << "Difference:           " << std::abs(putCallParityLHS() - putCallParityRHS()) << std::endl;
        std::cout << "==========================================" << std::endl;
    }
};

int main() {
    // Example 1: GTCO option (Nigerian market)
    // Stock at 55.50, Strike at 55.00, 3 months to expiry,
    // Risk-free rate 11.5% (Nigeria T-bill rate),
    // Volatility 39.87% (calculated from Project 1)
    BlackScholes gtcoOption(55.50, 55.00, 0.25, 0.115, 0.3987);

    gtcoOption.printReport();

    // Example 2: MSFT option (US market)
    // Stock at 420.00, Strike at 425.00, 6 months to expiry,
    // Risk-free rate 4.5% (US T-bill rate), Volatility 25%
    BlackScholes msftOption(420.00, 425.00, 0.5, 0.045, 0.25);
    msftOption.printReport();

    return 0;
}