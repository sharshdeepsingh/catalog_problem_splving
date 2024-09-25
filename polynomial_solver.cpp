#include <iostream>
#include <vector>
#include <fstream>
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;

const long long MOD = 1000000007; // Large prime modulus

// Fast modular exponentiation
long long mod_pow(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

// Convert string in given base to decimal (modulo MOD)
long long to_decimal(const std::string& value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit = isdigit(c) ? c - '0' : c - 'A' + 10;
        result = (result * base + digit) % MOD;
    }
    return result;
}

// Lagrange interpolation to find constant term
long long lagrange_interpolation(const std::vector<std::pair<long long, long long>>& points) {
    long long result = 0;
    int n = points.size();

    for (int i = 0; i < n; i++) {
        long long numerator = 1, denominator = 1;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                numerator = (numerator * points[j].first) % MOD;
                denominator = (denominator * ((points[i].first - points[j].first + MOD) % MOD)) % MOD;
            }
        }
        long long term = (points[i].second * numerator) % MOD;
        term = (term * mod_pow(denominator, MOD - 2)) % MOD; // Fermat's little theorem for modular inverse
        result = (result + term) % MOD;
    }

    return (result + MOD) % MOD;
}

int main() {
    std::ifstream file("input.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    json input;
    try {
        file >> input;
    } catch (json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }

    int k = input["keys"]["k"];
    std::vector<std::pair<long long, long long>> points;

    for (int i = 1; points.size() < k && i <= input["keys"]["n"]; i++) {
        if (input.contains(std::to_string(i))) {
            auto point = input[std::to_string(i)];
            int base = std::stoi(point["base"].get<std::string>());
            points.emplace_back(i, to_decimal(point["value"], base));
        }
    }

    if (points.size() < k) {
        std::cerr << "Not enough valid points provided." << std::endl;
        return 1;
    }

    std::cout << lagrange_interpolation(points) << std::endl;
    return 0;
}