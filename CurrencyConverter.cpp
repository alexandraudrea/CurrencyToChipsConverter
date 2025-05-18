#include "CurrencyConverter.h"
#include <cmath>
#include <qdebug.h>

CurrencyConverter::CurrencyConverter(const std::map<Currency, double>& exchangeRates) : rates(exchangeRates) {}

int CurrencyConverter::convertToUSD(double amount, const Currency& currency)
{
	if (rates.empty()) {
		qDebug() << "Using default rates.";
		std::map<Currency, double> defRates = { {Currency::EUR, 0.89}, {Currency::GBP, 0.75}, {Currency::DKK, 6.66}, {Currency::CZK, 22.27}, {Currency::RON, 4.55} };
		auto it = defRates.find(currency);
			return static_cast<int>(std::round(amount / it->second));

	}

	auto it = rates.find(currency);
	if (it != rates.end()) {
		return static_cast<int>(std::round(amount / it->second));
	}

}

std::vector<std::pair<int, int>> CurrencyConverter::chipBreakdown(int amount) {
	std::vector<std::pair<int, int>> result;
	std::vector<int> denominations = { 100, 50, 10, 5, 1 };

	for (int denom : denominations) {
		int count = amount / denom;
		result.push_back({ denom, count });
		amount %= denom;
	}

	return result;
}

