#include "siviso.h"
#include "ui_siviso.h"

#include <QFile>
#include <QDebug>
#include <QtCore>
#include <QtGui>

//#define localdir QHostAddress("192.168.1.178")        //de donde nos comunicamos
#define puertolocal 5002

siviso::siviso(QWidget *parent) :
    QMainWindow(parent),
     ui( new Ui::siviso)
{
    ui->setupUi(this);

    myppi = new PPI();
    mysignal = new Signal();
    proceso1 = new QProcess(this);
    proceso2 = new QProcess(this);
    proceso3 = new QProcess(this);
    numCatchSend = 0;
    catchSend = "";
    compGraf = "";

    udpsocket = new QUdpSocket(this);
    //udpsocket->bind(localdir,puertolocal);
    udpsocket->bind(QHostAddress::LocalHost, puertolocal);
    serialPortDB9 = new QSerialPort();
    serialPortUSB = new QSerialPort();
    connect(udpsocket,SIGNAL(readyRead()),this,SLOT(leerSocket()));
    connect(serialPortDB9, SIGNAL(readyRead()),this,SLOT(leerSerialDB9()));
    connect(serialPortUSB, SIGNAL(readyRead()),this,SLOT(leerSerialUSB()));

    direccionSSF = "192.168.1.177";                   //direccion del SPP
    puertoSSF = 8888;                                 //puerto del SPP
    //direccionApp = "192.168.1.178";                   //direccion que usaran las aplicaciones
    direccionApp = "127.0.0.1";                   //direccion que usaran las aplicaciones
    //udpsocket->writeDatagram(ui->view->text().toLatin1(),direccionPar,puertoPar); //visualiza la direcion IP y puerto del que envia
    //pSocket->connectToHost("192.168.1.10",6001);

    ui->frecuencia->setValue(mysignal->get_frec());
    ui->bw->setValue(mysignal->get_bw());
    ui->it->setValue(mysignal->get_it());
    ui->dt->setValue(mysignal->get_dt());
    ui->gan_sen->setValue(mysignal->get_ganancia_sensor());
    ui->dt->setValue(mysignal->get_dt());
    ui->prob_falsa->setValue(mysignal->get_prob_falsa());
    ui->prob_deteccion->setValue(mysignal->get_prob_deteccion());
    ui->edo_mar->setValue(mysignal->get_edo_mar());
    ui->origenOmni->setChecked(true);
    ui->dial->setDisabled(true);
    ui->ang->setVisible(false);
    ui->frecP->setValue(mysignal->get_frecP());
    ui->nP->setValue(mysignal->get_nP());
    ui->anchoP->setValue(mysignal->get_anchoP());

    bToolButton=false;
    ui->textTestGrap->setVisible(false);
    ui->view->setVisible(false);
    ui->save->setVisible(false);
    ui->startCom->setVisible(false);
    ui->endCom->setVisible(false);
    ui->sensor0->setVisible(false);
    ui->sensor1->setVisible(false);
    ui->closeJars->setVisible(false);
    ui->openJars->setVisible(false);
    ui->textSend->setVisible(false);
    ui->send->setVisible(false);
    ui->infoSignal->setVisible(false);

    serialPortUSB->write("GAIN 3\n");

    QFile file1("resource/colorUp.txt");
    if(file1.open(QIODevice::WriteOnly)){
        QTextStream stream(&file1);
        stream<<"255";
    } else {
        qDebug();
    }
    file1.close();

    QFile file2("resource/colorDw.txt");
    if(file2.open(QIODevice::WriteOnly)){
        QTextStream stream(&file2);
        stream<<"0";
    } else {
        qDebug();
    }
    file2.close();

    proceso1->startDetached("java -jar Lofar.jar");
    proceso2->startDetached("java -jar BTR.jar");
    proceso3->startDetached("java -jar PPI.jar");



//This use for TEST the class DBasePostgreSQL by Misael M Del Valle -- Status: Functional
//    myDB = new DBasePostgreSQL("172.16.1.3","PruebaQT",5432,"Administrador","nautilus");
//    myDB->executeSql("Select * from usuarios");

//Config Widget TEST -- Status: In construction and Functional
//   wConfig *w = new wConfig();
//   w->show();



}

siviso::~siviso()
{
    QString s = "EXIT";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);

    serialPortUSB->write("END COMMUNICATION\n");
    serialPortDB9->close();
    serialPortUSB->close();
    delete serialPortDB9;
    delete serialPortUSB;
    delete ui;
    proceso1->close();
    proceso2->close();
    proceso3->close();
}

void siviso::changeStyleSheet(int iStyle)
{
    QDir dir;

    if(m_pApplication != NULL)
        {

        //Change directory of the file
        QString strFilePath(dir.absolutePath()+"/");

            if(iStyle == DAY_STYLE)
                strFilePath.append("siviso_day.css");

            if(iStyle == DUSK_STYLE)
                strFilePath.append("siviso_dusk.css");

            if(iStyle == NIGHT_STYLE)
                strFilePath.append("siviso_night.css");

            QFile file(strFilePath);

            if(!file.open( QIODevice::ReadOnly | QIODevice::Text ))
            {
                qDebug() << "SIVISO: Failed to open- Copy the CSS File under the folder build...";
            }
            else
            {
                m_pApplication->setStyleSheet(QString(file.readAll()));
                file.close();
            }
    }
}

void siviso::leerSocket()
{
    while(udpsocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpsocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpsocket->readDatagram(datagram.data(),datagram.size(), &sender, &senderPort);
        QString info = datagram.data();
        ui->textTestGrap->appendPlainText(" port-> " + QString("%1").arg(senderPort));
        ui->textTestGrap->appendPlainText(info);
        //s = " ";

        QString s;
        if(info == "runPPI"){
            puertoPPI = senderPort;
            //s = "LONG";
            //udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
            s = "OFF";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
        } else if(info == "runBTR"){
            puertoBTR = senderPort;
            s = "LONG";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
            s = "OFF";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
        } else if(info == "runLF"){
            puertoLF = senderPort;
            s = "LONG";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
            s = "OFF";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
        } else if(info == "runREC"){
            puertoREC = senderPort;
        } else if(info == "PPI OK"){
            deshabilitado(false);
        } else if(puertoSSF == senderPort){
             udpsocket->writeDatagram(info.toLatin1(),direccionApp,puertoBTR);
             udpsocket->writeDatagram(info.toLatin1(),direccionApp,puertoLF);
        } else if(info == "BTR"){
            serialPortUSB->write("BTR\n");
        } else if(info == "LOFAR"){
            serialPortUSB->write("LOFAR\n");
        }
    }
}

void siviso::on_btOpenPort_clicked()
{
    /*serialPortDB9->setPortName("/dev/ttyS0");
    if(serialPortDB9->open(QIODevice::ReadWrite))
        ui->view->appendPlainText("Puerto serial abierto\n");
        //qDebug("Puerto serial abierto\n");
    else
        ui->view->appendPlainText("Error de coexion con el puerto serial\n");
        //qDebug("Error de coexion con el puerto serial\n");
    serialPortDB9->setBaudRate(QSerialPort::Baud9600);
    serialPortDB9->setDataBits(QSerialPort::Data8);
    serialPortDB9->setStopBits(QSerialPort::OneStop);
    serialPortDB9->setParity(QSerialPort::NoParity);
    serialPortDB9->setFlowControl(QSerialPort::NoFlowControl);*/

    serialPortUSB->setPortName("/dev/ttyUSB0");
    if(serialPortUSB->open(QIODevice::ReadWrite)){
        ui->view->appendPlainText("Puerto USB serial abierto\n");
    }else{
        ui->view->appendPlainText("Error de coexion con el puerto USB serial\n");
    }
    serialPortUSB->setBaudRate(QSerialPort::Baud9600);
    serialPortUSB->setDataBits(QSerialPort::Data8);
    serialPortUSB->setStopBits(QSerialPort::OneStop);
    serialPortUSB->setParity(QSerialPort::NoParity);
    serialPortUSB->setFlowControl(QSerialPort::NoFlowControl);

    serialPortUSB->write("START COMMUNICATION P\n");
    serialPortUSB->write("START COMMUNICATION A\n");
}

/*void siviso::leerSerialDB9()
{
    char buffer[101];
    int nDatos;

    nDatos = serialPortDB9->read(buffer,100);
    buffer[nDatos] = '\0';
    ui->textTestGrap->appendPlainText(buffer);

}*/

void siviso::leerSerialUSB()
{
    char buffer[101];
    int nDatos;
    numCatchSend++;
    serialPortUSB->flush();
    nDatos = serialPortUSB->read(buffer,100);

    buffer[nDatos] = '\0';
    ui->textTestGrap->appendPlainText(buffer);

    QString str;
    str=QString(buffer);
    int n =str.size();
    //ui->textTestGrap->appendPlainText(QString::number(n));


    numCatchSend += n;
    for(int x=0;x<str.size();x++){
        if(str[x]=='#'){
            bSensor = true;
            catchSensor = "";
            nSensor = 0;
            tipoSensor = 9; //esta variable es para indicar que sensor se comunica, si activo "1" o pasivo "0", se inicializa en "9"
        }
        if(str[x]=='1'||str[x]=='2'||str[x]=='3'||str[x]=='4'||str[x]=='5'||str[x]=='6'||str[x]=='7'||str[x]=='8'||str[x]=='9'||str[x]=='0'||str[x]==','||str[x]==';'||str[x]=='.'||str[x]==':'||str[x]=='-'){
            if(bSensor){
                if(str[x]==','||str[x]==';'){
                    switch(nSensor){
                    case 0:
                        if(catchSensor=="0"){
                            ui->B0Nom->setText("SSPF");
                            tipoSensor = 0;
                        }else if (catchSensor=="1"){
                            ui->B1Nom->setText("SSAF");
                            tipoSensor = 1;
                        }else{
                            ui->B1Nom->setText("error");
                        }
                        catchSensor = "";
                        nSensor++;
                        break;
                    case 1:
                        if(tipoSensor == 0){
                            ui->B0Or->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Or->setText(catchSensor);
                        }
                        catchSensor = "";
                        nSensor++;
                        break;
                    case 2:
                        if(tipoSensor == 0){
                            ui->B0Pr->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Pr->setText(catchSensor);
                        }
                        catchSensor = "";
                        nSensor++;
                        break;
                    case 3:
                        if(tipoSensor == 0){
                            ui->B0Temp->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Temp->setText(catchSensor);
                        }
                        catchSensor = "";
                        nSensor++;
                        break;
                    case 4:
                        if(tipoSensor == 0){
                            ui->B0Time->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Time->setText(catchSensor);
                        }
                        catchSensor = "";
                        nSensor++;
                        break;
                    case 5:
                        if(tipoSensor == 0){
                            ui->B0Lat->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Lat->setText(catchSensor);
                        }
                        catchSensor = "";
                        nSensor++;
                        break;
                    case 6:
                        if(tipoSensor == 0){
                            ui->B0Long->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Long->setText(catchSensor);
                        }
                        catchSensor = "";
                        nSensor++;
                        break;
                    case 7:
                        if(tipoSensor == 0){
                            ui->B0Carg->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Carg->setText(catchSensor);
                        }
                        nSensor++;
                        if(catchSensor.toInt()<=20)
                            ui->Alert->setText("ALERTA BATERIA BAJA");
                        catchSensor = "";
                        break;
                    case 8:
                        if(tipoSensor == 0){
                            ui->B0Volt->setText(catchSensor);
                        } else if(tipoSensor == 1){
                            ui->B1Volt->setText(catchSensor);
                        }
                        catchSensor = "";
                        break;
                    }
                } else{
                    catchSensor += str[x];
                }
            } else{
                if(str[x]==','||str[x]==';')
                    nWords++;
                catchSend += str[x];
            }
        }
        if(str[x]==';'){
            if(!bSensor){
                ui->textTestGrap->appendPlainText("esto enviare: "+catchSend);
                if(compGraf=="BTR"){
                    if(nWords==longBTR||catchSend.size()>=(longBTR*2))
                        udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoBTR);
                    else
                        ui->Alert->setText("Error, Datos recividos para graficar BTR son incomprensibles");
                }
                if(compGraf=="LF"){
                    if(nWords==longLF||catchSend.size()>=(longLF*2))
                        udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoLF);
                    else
                        ui->Alert->setText("Error, Datos recividos para graficar Lofar son incomprensibles");
                }

                numCatchSend = 0;

                //ui->textTestGrap->appendPlainText(catchSend);
                catchSend="";
            }
            nWords=0;
        }
    }
}

void siviso::on_tipo_norte_clicked()
{
    //PPI *myppi = new PPI();
    if(myppi->get_tipo_norte())
    {
        ui->tipo_norte->setText("Norte\nRelativo");
        myppi->set_tipo_norte(false);
    }
    else
    {
        ui->tipo_norte->setText("Norte\nVerdadero");
        myppi->set_tipo_norte(true);
    }
}

void siviso::on_lf_clicked()
{
    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    compGraf="LF";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
}

void siviso::on_btr_clicked()
{
    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    compGraf="BTR";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
}


void siviso::on_ppi_clicked()
{
    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    compGraf="PPI";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_origen_buque_clicked()
{
    ui->textTestGrap->appendPlainText("se cambio el origen a buque");
}

void siviso::on_origen_target_clicked()
{
    ui->textTestGrap->appendPlainText("se cambio el origen a blanco");
}

void siviso::on_frecuencia_valueChanged(int value)
{
    mysignal->set_frec(value);

    ui->view->appendPlainText("frec: ");
    QString s = QString::number(value);
    ui->view->appendPlainText(s);
}

void siviso::on_bw_valueChanged(double arg1)
{
    mysignal->set_bw(arg1);

    ui->view->appendPlainText("bw: ");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
}

void siviso::on_edo_mar_valueChanged(int arg1)
{
    mysignal->set_edo_mar(arg1);

    ui->view->appendPlainText("edo_mar: ");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
}

void siviso::on_prob_falsa_valueChanged(double arg1)
{
    mysignal->set_prob_falsa(arg1);

    ui->view->appendPlainText("prob_falsa: ");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
}

void siviso::on_prob_deteccion_valueChanged(double arg1)
{
    mysignal->set_prob_deteccion(arg1);

    ui->view->appendPlainText("prob_deteccion: ");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
}

void siviso::on_escala_ppi_valueChanged(double arg1)
{
    ui->textTestGrap->appendPlainText("escala PPI: ");
    QString s = QString::number(arg1);
    ui->textTestGrap->appendPlainText(s);
}

void siviso::on_escala_despliegue_tactico_valueChanged(double arg1)
{
    ui->textTestGrap->appendPlainText("desp_tact: ");
    QString s = QString::number(arg1);
    ui->textTestGrap->appendPlainText(s);\
}

void siviso::on_gan_sen_valueChanged(int arg1)
{
    mysignal->set_ganancia_sensor(arg1);

    ui->view->appendPlainText("ganancia_sensor: ");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
    //QByteArray ba ="GAIN "+s.toLatin1()+"\n";
    serialPortUSB->write("SET GAIN P\n");
    serialPortUSB->write(s.toLatin1()+"\n");
}

void siviso::on_it_valueChanged(int arg1)
{
    mysignal->set_it(arg1);

    ui->view->appendPlainText("It: ");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
}

void siviso::on_rec_clicked()
{
    QString s;
    if(bRec){
        bRec=false;
        ui->rec->setText("Stop");
        s = "REC_ON";
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoREC);
    }else{
        bRec=true;
        ui->rec->setText("Rec");
        s = "REC_OFF";
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoREC);
    }
}

void siviso::on_play_clicked()
{
    if(bPlay){
        bPlay=false;
        ui->play->setText("Stop");
        proceso3->startDetached("pactl load-module module-loopback");
        //proceso3->startDetached("java -jar recSound.jar");
        //QSound::play("2016-09-14_14:27:16.wav");
    }else{
        bPlay=true;
        ui->play->setText("Play");
        proceso3->startDetached("pactl unload-module module-loopback");
    }
}

void siviso::on_toolButton_clicked()
{
    if(bToolButton){
        bToolButton=false;
        ui->textTestGrap->setVisible(false);
        ui->view->setVisible(false);
        ui->save->setVisible(false);
        ui->startCom->setVisible(false);
        ui->endCom->setVisible(false);
        ui->sensor0->setVisible(false);
        ui->sensor1->setVisible(false);
        ui->closeJars->setVisible(false);
        ui->openJars->setVisible(false);
        ui->textSend->setVisible(false);
        ui->send->setVisible(false);
        ui->infoSignal->setVisible(false);
    }else{
        bToolButton=true;
        ui->textTestGrap->setVisible(true);
        ui->view->setVisible(true);
        ui->save->setVisible(true);
        ui->startCom->setVisible(true);
        ui->endCom->setVisible(true);
        ui->sensor0->setVisible(true);
        ui->sensor1->setVisible(true);
        ui->closeJars->setVisible(true);
        ui->openJars->setVisible(true);
        ui->textSend->setVisible(true);
        ui->send->setVisible(true);
        ui->infoSignal->setVisible(true);
    }
}

void siviso::on_save_clicked()
{
    QString s;
    s = "SAVE";
    if(compGraf=="BTR")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    if(compGraf=="LF")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    if(compGraf=="PPI")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_setColorUp_valueChanged(int value)
{
    colorUp = value;
    if(colorUp <= colorDw){
        colorDw = colorUp-1;
        ui->setColorDw->setValue(colorDw);
    }

    QFile file("resource/colorUp.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream<<value;
    } else {
        qDebug();
    }
    file.close();

    QString s;
    s = "RP";
    if(compGraf=="BTR")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    if(compGraf=="LF")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    if(compGraf=="PPI")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_setColorDw_valueChanged(int value)
{
    colorDw = value;
    if(colorDw >= colorUp){
        colorUp = colorDw+1;
        ui->setColorUp->setValue(colorUp);
    }

    QFile file("resource/colorDw.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream<<value;
    } else {
        qDebug();
    }
    file.close();

    QString s;
    s = "RP";
    if(compGraf=="BTR")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    if(compGraf=="LF")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    if(compGraf=="PPI")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_frecP_valueChanged(int arg1)
{
    serialPortUSB->write("SET CENTRAL FREQUENCY\n");
    QString s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
}

void siviso::on_nP_valueChanged(int arg1)
{
    serialPortUSB->write("SET N PULSOS\n");
    QString s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
}

void siviso::on_anchoP_valueChanged(int arg1)
{
    serialPortUSB->write("SET ANCHO PULSO\n");
    QString s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
}

void siviso::on_cw_clicked()
{
    //deshabilitado(true);
    QString s;
    //serialPortUSB->write("RANGO 1\n");
    if(ui->origenManual->isChecked()){
        serialPortUSB->write("SET ANGLE A\n");
        int n = ui->ang->value();
        if(n<0)
            n+=360;
        s = QString::number(n);
        serialPortUSB->write(s.toLatin1() + "\n");
    }
    serialPortUSB->write("ENCENDER\n");
    s = "PULSO";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_startCom_clicked()
{
    serialPortUSB->write("START COMMUNICATION P\n");
    serialPortUSB->write("START COMMUNICATION A\n");
}

void siviso::on_endCom_clicked()
{
    serialPortUSB->write("END COMMUNICATION\n");
}

void siviso::on_sensor0_clicked()
{
    serialPortUSB->write("SENSORES P\n");
}

void siviso::on_sensor1_clicked()
{
    serialPortUSB->write("SENSORES A\n");
}

void siviso::on_closeJars_clicked()
{
    QString s;
    s = "EXIT";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_openJars_clicked()
{
    QString s;
    s = "EXIT";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);

    proceso1->startDetached("java -jar Lofar.jar");
    proceso2->startDetached("java -jar BTR.jar");
    proceso3->startDetached("java -jar PPI.jar");
}


void siviso::on_dial_valueChanged(int value)
{
    ui->ang->setValue(value);
}

void siviso::on_ang_valueChanged(int arg1)
{
    if(arg1<=180)
        ui->dial->setValue(arg1);
    else{
        int n = arg1-360;
        ui->dial->setValue(n);
    }
}

void siviso::on_origenOmni_clicked()
{
    serialPortUSB->write("SET HIDROFONO\n");
    serialPortUSB->write("5 \n");
    ui->ang->setVisible(false);
    ui->dial->setDisabled(true);
}

void siviso::on_origenManual_clicked()
{
    serialPortUSB->write("SET HIDROFONO\n");
    serialPortUSB->write("0 \n");
    ui->ang->setVisible(true);
    ui->dial->setDisabled(false);
}

void siviso::on_dial_sliderReleased()
{
    serialPortUSB->write("SET ANGLE A\n");
    ui->textTestGrap->appendPlainText("SET ANGLE A\n");
    int n = ui->ang->value();
    QString s = QString::number(n);
    serialPortUSB->write(s.toLatin1() + "\n");
    ui->textTestGrap->appendPlainText(s);
}

void siviso::on_ang_editingFinished()
{
    serialPortUSB->write("SET ANGLE A\n");
    ui->textTestGrap->appendPlainText("SET ANGLE A\n");
    int n = ui->ang->value();
    QString s = QString::number(n);
    serialPortUSB->write(s.toLatin1() + "\n");
    ui->textTestGrap->appendPlainText(s);
}

void siviso::on_send_clicked()
{
    QString s;
    s = ui->textSend->text();

   /* QByteArray data = "prueba"; // <-- fill with data
    data = "ui->textSend->text()";

        pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * pSocket;
        connect( pSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );

        pSocket->connectToHost("192.168.1.10", 6001);
        if( pSocket->waitForConnected() ) {
            pSocket->write( data );
        }*/

    serialPortUSB->write(s.toLatin1()+"\n");
    ui->textSend->clear();
}

void siviso::on_infoSignal_clicked()
{
    ui->view->appendPlainText("info: ");
    QString s = mysignal->get_info_signal();
    ui->view->appendPlainText(s);
    ui->view->appendPlainText("portSSF " + QString("%1").arg(puertoSSF));
    ui->view->appendPlainText("portBTR " + QString("%1").arg(puertoBTR));
    ui->view->appendPlainText("portLF " + QString("%1").arg(puertoLF));
    ui->view->appendPlainText("portPPI " + QString("%1").arg(puertoPPI));
}

void siviso::deshabilitado(bool value){
    ui->frecuencia->setDisabled(value);
    ui->bw->setDisabled(value);
    ui->it->setDisabled(value);
    ui->dt->setDisabled(value);
    ui->cw->setDisabled(value);
    ui->frecP->setDisabled(value);
    ui->nP->setDisabled(value);
    ui->anchoP->setDisabled(value);
    ui->ran_det->setDisabled(value);
    ui->tipo_norte->setDisabled(value);
    ui->origenBuque->setDisabled(value);
    ui->origenSensor->setDisabled(value);
    ui->origenBlanco->setDisabled(value);
    ui->origenOmni->setDisabled(value);
    ui->origenAuto->setDisabled(value);
    ui->origenManual->setDisabled(value);
    ui->ang->setDisabled(value);
    ui->dial->setDisabled(value);
    ui->rec->setDisabled(value);
    ui->play->setDisabled(value);
    ui->vol_dw->setDisabled(value);
    ui->vol_up->setDisabled(value);
    ui->et_blancos->setDisabled(value);
    ui->clas_blancos->setDisabled(value);
    ui->edo_mar->setDisabled(value);
    ui->radio_boya->setDisabled(value);
    ui->prob_falsa->setDisabled(value);
    ui->prob_deteccion->setDisabled(value);
    ui->escala_ppi->setDisabled(value);
    ui->escala_desp->setDisabled(value);
    ui->gan_sen->setDisabled(value);
    ui->btOpenPort->setDisabled(value);
    ui->lf->setDisabled(value);
    ui->btr->setDisabled(value);
    ui->ppi->setDisabled(value);
}
