#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm> // For std::max

// Function to calculate the cumulative normal distribution
double norm_cdf(double value) {
    return 0.5 * std::erfc(-value * std::sqrt(0.5));
}

// Black-Scholes Formula for European Option (Call and Put)
double black_scholes(bool isCall, double S, double K, double r, double T, double sigma) {
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);

    if (isCall) {
        return S * norm_cdf(d1) - K * exp(-r * T) * norm_cdf(d2);
    } else {
        return K * exp(-r * T) * norm_cdf(-d2) - S * norm_cdf(-d1);
    }
}

// Binomial Option Pricing Model
double binomial_option_pricing(bool isCall, int steps, double S, double K, double r, double T, double sigma) {
    double dt = T / steps;                      
    double u = exp(sigma * sqrt(dt));           
    double d = 1 / u;                           
    double p = (exp(r * dt) - d) / (u - d);     
    std::vector<double> prices(steps + 1);      

    for (int i = 0; i <= steps; i++) {
        prices[i] = S * pow(u, steps - i) * pow(d, i);
        double payoff = isCall ? std::max(0.0, prices[i] - K) : std::max(0.0, K - prices[i]);
        prices[i] = payoff;
    }

    for (int j = steps - 1; j >= 0; j--) {
        for (int i = 0; i <= j; i++) {
            prices[i] = (p * prices[i] + (1 - p) * prices[i + 1]) * exp(-r * dt);
        }
    }

    return prices[0];
}

// Monte Carlo Simulation for European Option Pricing (Call and Put)
double monte_carlo_option(bool isCall, int num_simulations, double S, double K, double r, double T, double sigma) {
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::normal_distribution<> d(0, 1); 

    std::vector<double> payoffs;

    for (int i = 0; i < num_simulations; ++i) {
        double Z = d(gen); 
        double stock_price_at_maturity = S * exp((r - 0.5 * sigma * sigma) * T + sigma * sqrt(T) * Z);
        double payoff = isCall ? std::max(stock_price_at_maturity - K, 0.0) : std::max(K - stock_price_at_maturity, 0.0);
        payoffs.push_back(payoff);
    }

    double average_payoff = std::accumulate(payoffs.begin(), payoffs.end(), 0.0) / num_simulations;
    return exp(-r * T) * average_payoff;
}

int main() {
    std::cout << "Hello! I'm your friendly option pricing bot 🤖\n";
    std::cout << "Which model would you like to use today? (Type 'Black-Scholes', 'Binomial', or 'Monte Carlo')\n";
    std::string model;
    std::getline(std::cin, model);

    std::cout << "Do you want to price a Call or a Put option? (Type 'Call' or 'Put')\n";
    std::string optionType;
    std::getline(std::cin, optionType);
    bool isCall = (optionType == "Call");

    double S, K, r, T, sigma;
    std::cout << "Enter the current stock price (S): ";
    std::cin >> S;
    std::cout << "Enter the strike price (K): ";
    std::cin >> K;
    std::cout << "Enter the risk-free rate (r) as a decimal (e.g., 0.05 for 5%): ";
    std::cin >> r;
    std::cout << "Enter the time to expiration in years (T): ";
    std::cin >> T;
    std::cout << "Enter the volatility of the stock as a decimal (sigma): ";
    std::cin >> sigma;

    double price = 0.0;
    if (model == "Black-Scholes") {
        price = black_scholes(isCall, S, K, r, T, sigma);
        std::cout << "According to the Black-Scholes model, the " << optionType << " option price is: $" << price << "\n";
    } else if (model == "Binomial") {
        int steps;
        std::cout << "Enter the number of steps for the binomial model: ";
        std::cin >> steps;
        price = binomial_option_pricing(isCall, steps, S, K, r, T, sigma);
        std::cout << "According to the Binomial model, the " << optionType << " option price is: $" << price << "\n";
    } else if (model == "Monte Carlo") {
        int simulations;
        std::cout << "Enter the number of simulations for the Monte Carlo model: ";
        std::cin >> simulations;
        price = monte_carlo_option(isCall, simulations, S, K, r, T, sigma);
        std::cout << "According to the Monte Carlo model, the " << optionType << " option price is: $" << price << "\n";
    } else {
        std::cout << "Oops! It seems like you entered an unsupported model. Please try again.\n";
    }

    std::cout << "Thank you for using the Option Pricing Bot! Have a great day! 😊\n";
    return 0;
}
