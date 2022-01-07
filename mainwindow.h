#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetworkAuth>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include <QPixmap>
#include <QByteArray>

// API key for NASA LvfXhGqeHglcdvXboak6dvdysKR3jpIR24z67P6t


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum dataType {APOD, PICTURE};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QDate getCurrentDate();
    std::string returnDate();

private slots:
    void on_pushButton_clicked();
    void on_picPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    dataType data = APOD;

    QUrl imageurl;
    QString explanation;

    QUrl apiurl;
    std::string date;



};
#endif // MAINWINDOW_H
