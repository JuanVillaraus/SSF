#include "wconfig.h"
#include "ui_wconfig.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>


wConfig::wConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wConfig)
{
    ui->setupUi(this);

    QVBoxLayout *hLayoutButton = new QVBoxLayout;
    QLabel *label1 = new QLabel;
    label1->setText("Servidor:");
    QLineEdit *textEdit1 = new QLineEdit;
    textEdit1->setText("127.0.0.1");

    QLabel *label2 = new QLabel;
    label2->setText("Puerto:");
    QLineEdit *textEdit2 = new QLineEdit;
    textEdit2->setText("5432");

    QLabel *label3 = new QLabel;
    label3->setText("Base de Datos:");
    QLineEdit *textEdit3 = new QLineEdit;
    textEdit3->setText("");

    QLabel *label4 = new QLabel;
    label4->setText("Usuario:");
    QLineEdit *textEdit4 = new QLineEdit;
    textEdit4->setText("Administrador");

    QLabel *label5 = new QLabel;
    label5->setText("Contraseña:");
    QLineEdit *textEdit5 = new QLineEdit;
    textEdit5->setText("");


    hLayoutButton->addWidget(label1);
    hLayoutButton->addWidget(textEdit1);
    hLayoutButton->addWidget(label2);
    hLayoutButton->addWidget(textEdit2);
    hLayoutButton->addWidget(label3);
    hLayoutButton->addWidget(textEdit3);
    hLayoutButton->addWidget(label4);
    hLayoutButton->addWidget(textEdit4);
    hLayoutButton->addWidget(label5);
    hLayoutButton->addWidget(textEdit5);

    //main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayoutButton);

   this->setLayout(mainLayout);
    this->setWindowTitle("Configuración");

}

wConfig::~wConfig()
{
    delete ui;
}
