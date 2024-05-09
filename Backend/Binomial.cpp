#include <iostream>
#include <cmath>
#include <vector>

// Function to calculate the factorial of a number (used in binomial coefficient calculation)
int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Function to calculate binomial coefficient C(n, k) = n! / (k! * (n-k)!)
int binomial_coefficient(int n, int k) {
    return factorial(n) / (factorial(k) * factorial(n - k));
}

// Binomial Option Pricing Model
double binomial_option_pricing(bool isCall, int steps, double S, double K, double r, double T, double sigma) {
    double dt = T / steps;                      // Time per step
    double u = exp(sigma * sqrt(dt));           // Up factor
    double d = 1 / u;                           // Down factor
    double p = (exp(r * dt) - d) / (u - d);     // Risk-neutral probability
    std::vector<double> prices(steps + 1);      // Vector to store the prices at expiration

    // Calculate stock prices at expiration
    for (int i = 0; i <= steps; i++) {
        prices[i] = S * pow(u, steps - i) * pow(d, i);
    }

    // Calculate option values at expiration
    for (int i = 0; i <= steps; i++) {
        if (isCall) {
            prices[i] = std::max(0.0, prices[i] - K);
        } else {
            prices[i] = std::max(0.0, K - prices[i]);
        }
    }

    // Calculate option values back to the present
    for (int j = steps - 1; j >= 0; j--) {
        for (int i = 0; i <= j; i++) {
            prices[i] = (p * prices[i] + (1 - p) * prices[i + 1]) * exp(-r * dt);
        }
    }

    return prices[0];
}

int main() {
    int steps = 100;         // Number of steps in the binomial tree
    double S = 100.0;        // Current stock price
    double K = 100.0;        // Strike price
    double r = 0.05;         // Annual risk-free rate
    double T = 1.0;          // Time to expiration in years
    double sigma = 0.2;      // Volatility

    double callPrice = binomial_option_pricing(true, steps, S, K, r, T, sigma);
    double putPrice = binomial_option_pricing(false, steps, S, K, r, T, sigma);

    std::cout << "Binomial Model Call Option Price: " << callPrice << std::endl;
    std::cout << "Binomial Model Put Option Price: " << putPrice << std::endl;

    return 0;
}
