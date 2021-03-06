#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->picPushButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->dateEdit->setDate(getCurrentDate());
    setWindowTitle("NASA Picture of the day");

    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
         this, [=](QNetworkReply *reply) {
            if (reply->error()) {

                qDebug() << reply->errorString();
                ui->errorLabel->setText("Sorry! No picture found.");
                ui->picPushButton->setEnabled(false);
                ui->waitLabel->setText("");
                return;
            }

            if (data == APOD) {
                // parse JSON object to get data


                QString answer = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(answer.toUtf8());

                QJsonObject obj = doc.object();
                explanation = QString(obj.find("explanation").value().toString());
                imageurl = QUrl(obj.find("url").value().toString());
                ui->picPushButton->setEnabled(true);
                ui->saveButton->setEnabled(true);
                ui->waitLabel->setText("");
            }

            else if (data == PICTURE) {
                // display image


                QByteArray jpegData = reply->readAll();
                QPixmap pixmap;
                pixmap.loadFromData(jpegData);

                img = pixmap;

                int width = ui->label->width();
                int height = ui->label->height();

                ui->label->setPixmap(pixmap.scaled(width,height,Qt::KeepAspectRatio));
                ui->textBrowser->setText(explanation);
                ui->picPushButton->setEnabled(false);
            }

            //qDebug() << answer;
            }
     );

        // https://apod.nasa.gov/apod/image/2201/eclipse_EV1024.jpg

}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
}

QDate MainWindow::getCurrentDate()
{
    QDateTime date = QDateTime::currentDateTime();
    QDate today = date.date();
    return today;

}

std::string MainWindow::returnDate()
{
    QDate qdate = ui->dateEdit->date();
    std::string year = std::to_string(qdate.year());
    std::string month = std::to_string(qdate.month());
    std::string day = std::to_string(qdate.day());

    if (qdate.month() < 10) {
        month = "0"+month;
    }

    if (qdate.day() < 10) {
        day = "0"+day;
    }

    std::string newdate = year+"-"+month+"-"+day;
    date = newdate;

    return newdate;

}

void MainWindow::on_pushButton_clicked()
{
    data = APOD;
    ui->label->clear();
    ui->errorLabel->clear();
    ui->saveButton->setEnabled(false);

    std::string apidate = returnDate();
    std::string url = std::string("https://api.nasa.gov/planetary/apod?") + "date=" + apidate + "&" + "api_key=LvfXhGqeHglcdvXboak6dvdysKR3jpIR24z67P6t";

    request.setUrl(QUrl(QString::fromStdString(url)));
    manager->get(request);
    ui->waitLabel->setText("Please wait...");

}

void MainWindow::on_picPushButton_clicked()
{
    data = PICTURE;
    request.setUrl(imageurl);
    manager->get(request);
}


void MainWindow::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image File"),
                                                    QString(),
                                                  tr("Images (*.png)"));
    img.save(filename);
}

