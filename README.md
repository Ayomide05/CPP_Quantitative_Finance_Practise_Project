# C++ Quantitative Finance Portfolio

A collection of quantitative finance tools built in C++, covering market data analysis, option pricing, risk modelling, and portfolio simulation.

## Projects

### Project 1: Market Data Analyzer
Reads stock price data from CSV files and generates a risk report with key financial metrics.

**Concepts:** File I/O, CSV parsing, STL containers, algorithms, OOP

**Metrics computed:**
- Daily returns and total period return
- Average daily return
- Daily and annualised volatility
- Maximum drawdown
- Best and worst trading days

**Sample output:**
```
========================================
       RISK REPORT: GTCO
========================================
Period:                2025-01-02 to 2025-01-29
Trading Days:          20
Start Price:           45.5
End Price:             55.5
Total Return:          21.978%
----------------------------------------
Avg Daily Return:      1.08066%
Best Day:              4.48065%
Worst Day:             -1.8%
Daily Volatility:      2.51132%
Annualised Volatility: 39.8659%
Max Drawdown:          1.8%
========================================
```

---

### Project 2: Black-Scholes Option Pricer
Prices European call and put options using the Black-Scholes model with full Greeks computation and put-call parity verification.

**Concepts:** `<cmath>` (exp, log, sqrt), normal distribution, OOP, financial mathematics

**Features:**
- European call and put pricing
- Full Greeks surface: Delta, Gamma, Theta, Vega, Rho
- Put-call parity verification
- Multiple market support (tested with NGX and US equities)

**Sample output:**
```
==========================================
     BLACK-SCHOLES OPTION PRICER
==========================================
Stock Price (S):     55.5000
Strike Price (K):    55.0000
Time to Expiry (T):  0.2500 years
Risk-Free Rate (r):  11.5000%
Volatility (sigma):  39.8700%
------------------------------------------
Call Price:           5.4315
Put Price:            3.3727
------------------------------------------
           GREEKS
------------------------------------------
Call Delta:           0.6138
Put Delta:            -0.3862
Gamma:                0.0346
Call Theta (daily):   -0.0322
Put Theta (daily):    -0.0154
Vega (per 1% vol):    0.1062
Call Rho (per 1%):    0.0716
Put Rho (per 1%):     -0.0620
------------------------------------------
   PUT-CALL PARITY CHECK
------------------------------------------
C - P =               2.0587
S - K*e^(-rT) =       2.0587
Difference:           0.0000
==========================================
```

---

### Project 3: Monte Carlo VaR Engine
Simulates thousands of portfolio scenarios using random number generation to calculate Value at Risk and Expected Shortfall at multiple confidence levels.

**Concepts:** `<random>` (Mersenne Twister, normal distribution), Monte Carlo simulation, VaR, Expected Shortfall, scenario analysis

**Features:**
- Monte Carlo simulation with configurable scenarios (default: 10,000)
- VaR at 90%, 95%, and 99% confidence levels
- Expected Shortfall (Conditional VaR) at all levels
- Scenario distribution analysis (loss/gain breakdown)
- Extreme scenario identification
- Reproducible results via seeded random generation

**Sample output:**
```
==========================================
       MONTE CARLO VaR ENGINE
==========================================
Portfolio Value:      10000000.00
Daily Mean Return:    1.08%
Daily Volatility:     2.51%
Simulations:          10000
------------------------------------------
     VALUE AT RISK (VaR)
------------------------------------------
VaR (90%):            212811.42
VaR (95%):            308689.47
VaR (99%):            492814.73
------------------------------------------
     EXPECTED SHORTFALL (ES)
------------------------------------------
ES  (90%):            334089.17
ES  (95%):            412048.80
ES  (99%):            569834.19
==========================================
```

---

### Project 4: Mini Risk System
*Coming soon*

---

## How to Run

### Prerequisites
- C++ compiler (g++ with C++11 support or later)

### Compile any project
```bash
cd Project1-MarketDataAnalyzer
g++ MarketDataAnalyzer.cpp -o MarketDataAnalyzer
./MarketDataAnalyzer

cd Project2-BlackScholes
g++ BlackScholes.cpp -o BlackScholes
./BlackScholes

cd Project3-MonteCarloVaR
g++ MonteCarloVaR.cpp -o MonteCarloVaR
./MonteCarloVaR
```

## How the Projects Connect

The projects form an integrated risk analysis pipeline:
1. **Project 1** reads market data and calculates volatility (39.87%) and mean return (1.08%)
2. **Project 2** uses that volatility as an input to price options on the same stock
3. **Project 3** uses the same return statistics to simulate portfolio risk via Monte Carlo

## Technical Skills Demonstrated
- Modern C++ (C++11/14): smart pointers, move semantics, lambdas
- Object-Oriented Design: classes, inheritance, polymorphism
- STL: vectors, maps, algorithms, iterators
- File I/O and data parsing
- Mathematical computing: volatility, Black-Scholes, Monte Carlo simulation
- Random number generation and statistical simulation
- Memory management: dynamic allocation, RAII

## Author
Ayomide — Aspiring Quantitative Risk Analyst

Built as part of a structured C++ learning journey targeting quantitative finance and market risk roles.
