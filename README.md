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
*Coming soon*

---

### Project 3: Monte Carlo VaR Engine
*Coming soon*

---

### Project 4: Mini Risk System
*Coming soon*

---

## How to Run

### Prerequisites
- C++ compiler (g++ with C++11 support or later)

### Compile any project
```bash
cd MarketDataAnalyzer
g++ MarketDataAnalyzer.cpp -o MarketDataAnalyzer
./MarketDataAnalyzer
```

## Technical Skills Demonstrated
- Modern C++ (C++11/14): smart pointers, move semantics, lambdas
- Object-Oriented Design: classes, inheritance, polymorphism
- STL: vectors, maps, algorithms, iterators
- File I/O and data parsing
- Mathematical computing: volatility, Black-Scholes, Monte Carlo simulation
- Memory management: dynamic allocation, RAII

## Author
Ayomide — Aspiring Quantitative Risk Analyst

Built as part of a structured C++ learning journey targeting quantitative finance and market risk roles.
