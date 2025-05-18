#pragma once

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>

#include "CurrencyConverter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void onConvertButtonClicked(); //Slot that is connected to the convert button clicked signal
	void onRatesDownloaded(QNetworkReply* reply); //Slot that is connected to the rates downloaded signal

private:
	Ui::MainWindow* ui; //UI object for the main window
	std::unique_ptr<CurrencyConverter> converter;

	std::optional<Currency> fromString(const QString& code); //Convert a string to a Currency object

	QNetworkAccessManager* m_networkManager;

};