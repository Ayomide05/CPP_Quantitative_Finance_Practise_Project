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
An integrated risk analysis system that combines all three previous projects into a single unified pipeline. Takes user input, loads market data, and generates a comprehensive risk report covering statistical analysis, option pricing, and Monte Carlo VaR.

**Concepts:** System integration, modular class design, user interaction, data flow between components, software architecture

**Features:** Interactive user input for all parameters, CSV market data loading and parsing, statistical risk metrics (volatility, drawdown, return analysis), Black-Scholes option pricing with Greeks, Monte Carlo VaR and Expected Shortfall, unified four-section risk report.

**Sample output:**
```
##############################################
       INTEGRATED RISK REPORT
       GTCO
##############################################

[1] MARKET DATA SUMMARY
Period:              2025-01-02 to 2025-01-29
Trading Days:        20
Total Return:        21.9780%

[2] RISK STATISTICS
Avg Daily Return:    1.0807%
Daily Volatility:    2.5113%
Annual Volatility:   39.8659%
Max Drawdown:        1.8000%

[3] OPTION PRICING (Black-Scholes)
Call Price:          5.4310
Put Price:           3.3723
Call Delta:          0.6138
Gamma:               0.0346

[4] MONTE CARLO VALUE AT RISK
Portfolio Value:     10000000.00
VaR (95%):           308841.65
VaR (99%):           493063.41
ES  (95%):           412255.15
ES  (99%):           570123.22

##############################################
          END OF REPORT
##############################################
```

---

### Project 5: Multi-Asset Portfolio Optimizer
Implements Markowitz mean-variance portfolio optimization using Monte Carlo simulation to find optimal asset allocation across multiple stocks.

**Concepts:** Covariance matrix computation, matrix arithmetic, Sharpe ratio optimization, portfolio theory, efficient frontier, random weight generation

**Features:**
- Multi-asset CSV data loading and return calculation
- Covariance and correlation matrix computation
- Monte Carlo portfolio simulation (50,000 random portfolios)
- Optimal portfolio identification (maximum Sharpe ratio)
- Minimum volatility portfolio identification
- Equal weight benchmark comparison
- Annualised return and volatility scaling

**Sample output:**
```
##############################################
    MULTI-ASSET PORTFOLIO OPTIMIZER
##############################################
[1] ASSET SUMMARY
    Ticker |  Mean Return |   Volatility
      GTCO |      1.0807% |      2.5113%
       UBA |      0.6416% |      1.9077%
    ZENITH |      0.6793% |      1.8415%
   DANGCEM |      0.5690% |      1.8753%
      MTNN |      0.5767% |      1.6263%

[2] CORRELATION MATRIX
              GTCO     UBA  ZENITH DANGCEM    MTNN
      GTCO    1.00    0.90    0.97    0.99    0.98
       UBA    0.90    1.00    0.82    0.87    0.87

[3] OPTIMAL PORTFOLIO (Max Sharpe Ratio)
      GTCO: 88.03%   UBA: 6.21%   ZENITH: 5.00%
      Sharpe Ratio: 0.4066

[4] MINIMUM VOLATILITY PORTFOLIO
      MTNN: 88.50%   UBA: 8.23%
      Annual Volatility: 26.13%

[5] EQUAL WEIGHT PORTFOLIO (Benchmark)
      Sharpe Ratio: 0.3490
##############################################
```

---

## How the Projects Connect

The projects form an integrated quantitative finance pipeline:
1. **Project 1** reads market data and calculates volatility and mean return
2. **Project 2** uses that volatility to price options via Black-Scholes
3. **Project 3** uses return statistics to simulate portfolio risk via Monte Carlo
4. **Project 4** connects Projects 1–3 into a single interactive risk system
5. **Project 5** extends to multi-asset analysis with portfolio optimization

## How to Run

### Prerequisites
- C++ compiler (g++ with C++11 support or later)

### Compile and run any project
```bash
# Project 1: Market Data Analyzer
cd Project1-MarketDataAnalyzer
g++ MarketDataAnalyzer.cpp -o MarketDataAnalyzer
./MarketDataAnalyzer

# Project 2: Black-Scholes Option Pricer
cd Project2-BlackScholes
g++ BlackScholes.cpp -o BlackScholes
./BlackScholes

# Project 3: Monte Carlo VaR Engine
cd Project3-MonteCarloVaR
g++ MonteCarloVaR.cpp -o MonteCarloVaR
./MonteCarloVaR

# Project 4: Mini Risk System (requires market_data.csv)
cd Project4-MiniRiskSystem
g++ MiniRiskSystem.cpp -o MiniRiskSystem
./MiniRiskSystem

# Project 5: Portfolio Optimizer (requires portfolio_data.csv)
cd Project5-PortfolioOptimizer
g++ PortfolioOptimizer.cpp -o PortfolioOptimizer
./PortfolioOptimizer
```

## Technical Skills Demonstrated
- Modern C++ (C++11/14): smart pointers, move semantics, lambdas, auto
- Object-Oriented Design: classes, inheritance, polymorphism, encapsulation
- STL: vectors, maps, algorithms, iterators, numeric
- File I/O and multi-column CSV data parsing
- Mathematical computing: volatility, Black-Scholes, normal distribution, Greeks
- Random number generation and Monte Carlo simulation
- Portfolio theory: covariance matrices, Sharpe ratio, Markowitz optimization
- Memory management: dynamic allocation, RAII
- Software design: modular architecture, multi-component integration

## Author
Ayomide — Aspiring Quantitative Risk Analyst

Built as part of a structured C++ learning journey targeting quantitative finance and market risk roles.
