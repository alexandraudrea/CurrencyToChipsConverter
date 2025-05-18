#pragma once

#include <map>
#include <vector>

enum class Currency {
	EUR, GBP, DKK, CZK, RON
};

class CurrencyConverter {

public:
	explicit CurrencyConverter(const std::map<Currency, double>& exchangeRates);
	int convertToUSD(double amount, const Currency& currency);
	std::vector<std::pair<int, int>> chipBreakdown(int amount);

private:
	struct CurrencyCompare {
		bool operator()(const Currency& a, const Currency& b) const {
			return static_cast<int>(a) < static_cast<int>(b);
		}
	};

	std::map<Currency, double> rates; // exchange rates to USD

};