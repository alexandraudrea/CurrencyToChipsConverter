#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qdebug.h>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_networkManager(new QNetworkAccessManager(this))
{
	ui->setupUi(this);

	ui->currencyComboBox->addItem("EUR");
	ui->currencyComboBox->addItem("GBP");
	ui->currencyComboBox->addItem("DKK");
	ui->currencyComboBox->addItem("CZK");
	ui->currencyComboBox->addItem("RON");


	connect(ui->convertButton, &QPushButton::clicked, this, &MainWindow::onConvertButtonClicked);
	connect(m_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onRatesDownloaded);

	m_networkManager->get(QNetworkRequest(QUrl("https://www.floatrates.com/daily/usd.json")));

}

MainWindow::~MainWindow()
{
	delete ui;
	ui = nullptr;
}

std::optional<Currency> MainWindow::fromString(const QString& code)
{
	if (code == "EUR") return Currency::EUR;
	if (code == "GBP") return Currency::GBP;
	if (code == "DKK") return Currency::DKK;
	if (code == "CZK") return Currency::CZK;
	if (code == "RON") return Currency::RON;
	return std::nullopt;
}

void MainWindow::onRatesDownloaded(QNetworkReply* reply)
{
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Network error:" << reply->errorString();
		return;
	}

	QByteArray data = reply->readAll();
	reply->deleteLater();

	QJsonDocument doc = QJsonDocument::fromJson(data);
	QJsonObject obj = doc.object();

	std::map<Currency, double> parsedRates;

	for (const QString& key : { "eur", "gbp", "dkk", "czk", "ron" }) 
	{
		if (obj.contains(key)) 
		{
			QJsonObject rateObj = obj[key].toObject();
			double rate = rateObj["rate"].toDouble();
			auto currencyOpt = fromString(key.toUpper());
			if (currencyOpt.has_value()) 
			{
				parsedRates[currencyOpt.value()] = rate;
			}
		}
	}

	converter = std::make_unique<CurrencyConverter>(parsedRates);

	qDebug() << "Rates loaded.";
}

void MainWindow::onConvertButtonClicked()
{
	Q_ASSERT(converter);


	if (!converter) {
		qDebug() << "Rates not loaded.";
		ui->usdResultLabel->setText("Rates not loaded.");
		return;
	}

	double amount = ui->amountLineEdit->text().toDouble();
	QString code = ui->currencyComboBox->currentText();
	auto currencyOpt = fromString(code);

	if (!currencyOpt.has_value()) {
		qDebug() << "Invalid currency code:" << code;
		ui->usdResultLabel->setText("Invalid currency.");
		return;
	}

	int usd = converter->convertToUSD(amount, currencyOpt.value());
	qDebug() << "Converted amount:" << usd;
	ui->usdResultLabel->setText(QString("Converted Amount: $%1 USD").arg(usd));

	auto breakdown = converter->chipBreakdown(usd); 

	QStringList breakdownList;
	for (const std::pair<int, int>& entry : breakdown) {
		int denom = entry.first;
		int count = entry.second;
		if (count > 0)
			breakdownList << QString("%1 x $%2").arg(count).arg(denom);
	}


	qDebug() << "Final breakdown list:" << breakdownList;

	if (breakdownList.isEmpty()) {
		breakdownList << "No chips required.";
	}

	ui->chipOutputText->setPlainText(breakdownList.join("\n"));

}