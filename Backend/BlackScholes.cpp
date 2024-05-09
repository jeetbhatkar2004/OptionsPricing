#include <iostream>
#include <cmath>
#include <algorithm> // For std::max function

// Function to calculate the cumulative normal distribution
double norm_cdf(double value) {
    return 0.5 * std::erfc(-value * std::sqrt(0.5));
}

// Black-Scholes Formula for European Call Option
double black_scholes(bool isCall, double S, double K, double r, double T, double sigma) {

    if (isCall == true){
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return S * norm_cdf(d1) - K * exp(-r * T) * norm_cdf(d2);
    }


    if (isCall == false){
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return K * exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
    }
}



int main() {
    bool isCall = true;
    double S = 100.0;   // Current stock price (e.g., $100)
    double K = 100.0;   // Strike price (e.g., $100)
    double r = 0.05;    // Annual risk-free rate (5%)
    double T = 1.0;     // Time to expiration in years (1 year)
    double sigma = 0.2; // Volatility of the stock (20%)

    std::cout << "Black-Scholes Call Option Price: " 
              << black_scholes_call(S, K, r, T, sigma) << std::endl;
    std::cout << "Black-Scholes Put Option Price: " 
              << black_scholes_put(S, K, r, T, sigma) << std::endl;

    return 0;
}
