#include <iostream>
#include <cmath>
#include <random>
#include <vector>

// Function to calculate the average of a vector
double average(const std::vector<double>& values) {
    double sum = 0.0;
    for (double value : values) {
        sum += value;
    }
    return sum / values.size();
}

// Monte Carlo Simulation for European Option Pricing (Call and Put)
double monte_carlo_option(bool isCall, int num_simulations, double S, double K, double r, double T, double sigma) {
    std::random_device rd;  // Non-deterministic random number generator
    std::mt19937 gen(rd()); // Mersenne-Twister generator, seeded with rd()
    std::normal_distribution<> d(0, 1); // Standard normal distribution

    double dt = T; // Time step size (one large step if T is the time to maturity)
    std::vector<double> payoffs;

    // Simulate random paths for the underlying asset price
    for (int i = 0; i < num_simulations; ++i) {
        double Z = d(gen); // Random value from standard normal distribution
        double stock_price_at_maturity = S * exp((r - 0.5 * sigma * sigma) * dt + sigma * sqrt(dt) * Z);

        double payoff;
        if (isCall) {
            payoff = std::max(stock_price_at_maturity - K, 0.0);
        } else {
            payoff = std::max(K - stock_price_at_maturity, 0.0);
        }
        payoffs.push_back(payoff);
    }

    // Calculate the average payoff and discount it back to the present value
    double average_payoff = average(payoffs);
    return exp(-r * T) * average_payoff;
}

int main() {
    int num_simulations = 1000000; // Number of simulations
    double S = 100.0;              // Current stock price
    double K = 100.0;              // Strike price
    double r = 0.05;               // Annual risk-free rate
    double T = 1.0;                // Time to expiration in years
    double sigma = 0.2;            // Volatility

    double callPrice = monte_carlo_option(true, num_simulations, S, K, r, T, sigma);
    double putPrice = monte_carlo_option(false, num_simulations, S, K, r, T, sigma);

    std::cout << "Monte Carlo Call Option Price: " << callPrice << std::endl;
    std::cout << "Monte Carlo Put Option Price: " << putPrice << std::endl;

    return 0;
}
