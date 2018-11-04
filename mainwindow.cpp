#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "decoder.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QByteArray get_byte_string(QString string)
{
    QByteArray byte_string;
    QStringList splitList = string.split(" ");
    for ( const auto& i : splitList  )
    {
        if (i.size() == 2)
        {
            byte_string.append(QByteArray::fromHex(i.toLatin1()));
        }
        else
        {
            byte_string.clear();
            break;
        }
    }
    return byte_string;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    QByteArray byte_string = get_byte_string(ui->lineEdit->text());
    if (byte_string.size() == 0)
    {
        ui->textBrowser->insertPlainText("Ошибка: неправильный формат данных");
    }
    else
    {
      Decoder *decod = Decoder::get_decoder(byte_string);
      QObject::connect(decod, SIGNAL(send_string(QString)),ui->textBrowser,SLOT(insertPlainText(QString)));
      decod->decode();
      delete decod;
    }
}
