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
    proceso4 = new QProcess(this);
    proceso5 = new QProcess(this);
    numCatchSend = 0;
    catchSend = "";
    catchCmd = "";
    compGraf = "";
    bSensor = false;
    puertoLF = 0;
    puertoBTR = 0;
    puertoPPI = 0;
    puertoDEMON = 0;
    puertoComSF = 0;
    bAutoSend = true;
    graf = 'o';
    nWords = 0;

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

    bAudio = false;
    bPulso = false;

    //ui->frecuencia->setValue(mysignal->get_frec());
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
    //ui->startCom->setVisible(false);
    ui->btOpenPort->setVisible(false);
    ui->endCom->setVisible(false);
    ui->sensor0->setVisible(false);
    ui->sensor1->setVisible(false);
    ui->closeJars->setVisible(false);
    ui->openJars->setVisible(false);
    ui->textSend->setVisible(false);
    //ui->send->setVisible(false);
    ui->infoSignal->setVisible(false);
    ui->ApagarP->setVisible(false);
    ui->ApagarA->setVisible(false);
    //ui->sensorOFF->setVisible(false);
    //ui->sensorON->setVisible(false);
    ui->setColorUp->setVisible(false);
    ui->setColorDw->setVisible(false);
    ui->dataSim->setVisible(false);
    ui->dist->setVisible(false);
    ui->pulso->setVisible(false);
    ui->GPSOr->setVisible(false);
    ui->autoSend->setVisible(false);
    //ui->btLF->setVisible(false);
    ui->prog->setVisible(false);
    ui->factor_p->setVisible(false);
    ui->central_frec->setVisible(false);
    ui->dTotal->setVisible(false);
    ui->dPaso->setVisible(false);
    ui->Uu->setVisible(false);
    ui->Ud->setVisible(false);
    ui->limProm->setVisible(false);
    ui->clean->setVisible(false);
    ui->label_70->setVisible(false);
    ui->label_71->setVisible(false);
    ui->label_72->setVisible(false);
    ui->label_76->setVisible(false);
    ui->label_78->setVisible(false);
    ui->label_79->setVisible(false);
    ui->windowing->setVisible(false);
    ui->prefilter->setVisible(false);


    ui->anchoP->setDisabled(true);
    //ui->frecuencia->setDisabled(true);
    ui->bw->setDisabled(true);
    ui->it->setDisabled(true);
    ui->dt->setDisabled(true);
    ui->tipo_norte->setDisabled(true);
    ui->origenBuque->setDisabled(true);
    ui->origenSensor->setDisabled(true);
    ui->origenBlanco->setDisabled(true);
    ui->origenAuto->setDisabled(true);
    //ui->rec->setDisabled(true);
    //ui->play->setDisabled(true);
    //ui->vol_dw->setDisabled(true);
    //ui->vol_up->setDisabled(true);
    //ui->et_blancos->setDisabled(true);
    //ui->clas_blancos->setDisabled(true);
    ui->edo_mar->setDisabled(true);
    ui->radio_boya->setDisabled(true);
    ui->prob_falsa->setDisabled(true);
    //ui->prob_deteccion->setDisabled(true);
    //ui->escala_ppi->setDisabled(true);
    ui->escala_desp->setDisabled(true);
    ui->ran_det->setDisabled(true);

    /*ui->cw->setDisabled(true);
    ui->chirpDw->setDisabled(true);
    ui->chirpUp->setDisabled(true);
    ui->chype->setDisabled(true);
    ui->chirpFrecDw->setDisabled(true);
    ui->chirpFrecUp->setDisabled(true);
    ui->chirpTime->setDisabled(true);
    ui->frecP->setDisabled(true);
    ui->nP->setDisabled(true);*/

    edoPas = 0;
    edoAct = 0;
    serialPortUSB->write("GAIN 3\n");

    QFile file1("resource/colorUp.txt");
    if(file1.open(QIODevice::WriteOnly)){
        QTextStream stream(&file1);
        stream<<"255";
    } else {
        qDebug();
    }
    file1.close();
    colorUp = 255;

    QFile file2("resource/colorDw.txt");
    if(file2.open(QIODevice::WriteOnly)){
        QTextStream stream(&file2);
        stream<<"0";
    } else {
        qDebug();
    }
    file2.close();
    colorDw = 0;

    QFile file("resource/angMarc.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream<<0;
    } else {
        qDebug();
    }
    file.close();

    QFile file3("resource/targets.txt");
    if(file3.open(QIODevice::WriteOnly)){
        QTextStream stream(&file3);
        stream<<"0;";
    } else {
        qDebug();
    }
    file3.close();

    thread()->sleep(1);
    proceso2->startDetached("java -jar BTR.jar");
    thread()->sleep(1);
    proceso1->startDetached("java -jar Lofar.jar");
    thread()->sleep(1);
    proceso4->startDetached("java -jar demon.jar");
    thread()->sleep(1);
    proceso3->startDetached("java -jar PPI.jar");
    thread()->sleep(1);
    proceso5->startDetached("java -jar ConexionSF.jar");
    thread()->sleep(1);

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
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);

    serialPortUSB->write("END COMMUNICATION A\n");
    serialPortUSB->write("END COMMUNICATION P\n");
    serialPortDB9->close();
    serialPortUSB->close();
    delete serialPortDB9;
    delete serialPortUSB;
    delete ui;
    proceso1->close();
    proceso2->close();
    proceso3->close();
    proceso4->close();
    proceso5->close();
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



        QString s;
        if(info == "runDEMON"){
            s = "EXIT";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
            puertoDEMON = senderPort;
            s = "LONG";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
            s = "OFF";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
        } else if(info == "runPPI"){
            s = "EXIT";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
            puertoPPI = senderPort;
            s = "OFF";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
            s = "LONG";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
        } else if(info == "runBTR"){
            s = "EXIT";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
            puertoBTR = senderPort;
            s = "OFF";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
            s = "LONG";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
        } else if(info == "runLF"){
            s = "EXIT";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
            puertoLF = senderPort;
            s = "LONG";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
            s = "OFF";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
        } else if(info == "runConxSF"){
            s = "EXIT";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
            puertoComSF = senderPort;
            s = "RP";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
        } else if(info == "runREC"){
            puertoREC = senderPort;
        } else if(info == "PPI OK"){
            deshabilitado(false);
        } else if(puertoSSF == senderPort){
             udpsocket->writeDatagram(info.toLatin1(),direccionApp,puertoBTR);
             udpsocket->writeDatagram(info.toLatin1(),direccionApp,puertoLF);
        } else if(info == "BTR"){
            if(bAutoSend){
            serialPortUSB->write("BTR P\n");
            }
        } else if(info == "LOFAR"){
            if(bAutoSend){
            serialPortUSB->write("LOFAR P\n");
            }
        } else if(info == "DEMON"){
            if(bAutoSend){
            serialPortUSB->write("DEMON P\n");
            }
        } else if(info == "SENSOR P"){
            if(bAutoSend){
            serialPortUSB->write("SENSORES P\n");
            thread()->msleep(100);
            }
        } else if(info == "SENSOR A"){
            if(bAutoSend){
            serialPortUSB->write("SENSORES A\n");
            thread()->msleep(100);
            }
        } else if(info == "PLAY OK"){
            ui->play->setDisabled(false);
        } else if(info == "A1"){
            ui->B1estado->setText("Desconectado");
            s = "A_DW";
            /*ui->cw->setDisabled(true);
            ui->chirpDw->setDisabled(true);
            ui->chirpUp->setDisabled(true);
            ui->chype->setDisabled(true);
            ui->chirpFrecDw->setDisabled(true);
            ui->chirpFrecUp->setDisabled(true);
            ui->chirpTime->setDisabled(true);
            ui->frecP->setDisabled(true);
            ui->nP->setDisabled(true);*/
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
            edoAct = 1;
        } else if(info == "A2"){
            ui->B1estado->setText("En espera");
            s = "A_ESP";
            /*ui->cw->setDisabled(true);
            ui->chirpDw->setDisabled(true);
            ui->chirpUp->setDisabled(true);
            ui->chype->setDisabled(true);
            ui->chirpFrecDw->setDisabled(true);
            ui->chirpFrecUp->setDisabled(true);
            ui->chirpTime->setDisabled(true);
            ui->frecP->setDisabled(true);
            ui->nP->setDisabled(true);*/
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
            edoAct = 2;
        } else if(info == "P1"){
            ui->B0estado->setText("Desconectado");
            s = "P_DW";
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
            edoPas = 1;
        } else if(info == "P2"){
            ui->B0estado->setText("En espera");
            s = "P_ESP";
            edoPas = 2;
            udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
        } else if(info == "ANT"){
            serialPortUSB->setPortName("/dev/ttyUSB0");
            if(serialPortUSB->open(QIODevice::ReadWrite)){
                ui->view->appendPlainText("Puerto USB serial abierto\n");
                s = "ANT_UP";
                udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
                //serialPortUSB->setBaudRate(QSerialPort::Baud115200);
                serialPortUSB->setBaudRate(QSerialPort::Baud9600);
                serialPortUSB->setDataBits(QSerialPort::Data8);
                serialPortUSB->setStopBits(QSerialPort::OneStop);
                serialPortUSB->setParity(QSerialPort::NoParity);
                serialPortUSB->setFlowControl(QSerialPort::NoFlowControl);
                //serialPortUSB->write("START COMMUNICATION P\n");
                thread()->msleep(100);
                //serialPortUSB->write("START COMMUNICATION A\n");
                thread()->msleep(100);
            }else{
                s = "ANT_DW";
                udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
                ui->view->appendPlainText("Error de coexion con el puerto USB serial\n");
            }
        /*} else if(info == "Conx0"){
            serialPortUSB->setPortName("/dev/ttyUSB0");
            if(serialPortUSB->open(QIODevice::ReadWrite)){
                ui->view->appendPlainText("Puerto USB serial abierto\n");
                serialPortUSB->setBaudRate(QSerialPort::Baud9600);
                serialPortUSB->setDataBits(QSerialPort::Data8);
                serialPortUSB->setStopBits(QSerialPort::OneStop);
                serialPortUSB->setParity(QSerialPort::NoParity);for (int x = 0; x < 100; x++) {                                         //inicializa el waterfall en cero
            for (int y = 0; y < longPPI; y++) {
                waterfall[x][y] = 0;
                waterfallTargets[x][y] = 0;
            }
        }
                serialPortUSB->setFlowControl(QSerialPort::NoFlowControl);
            }else{
                ui->view->appendPlainText("Error de coexion con el puerto USB serial\n");
                s = "USB_DW";
                udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
            }*/
        } else if(info == "ERROR"){
            ui->view->appendPlainText("Comando no valido");
        } else if(info[0]=='#'){
            s = "";
            for(int x=1;x<info.size();x++){
                s+=info[x];
            }
            if(puertoDEMON == senderPort){
                longDEMON = s.toInt();
            }else if(puertoBTR == senderPort){
                longBTR = s.toInt();
            } else if(puertoLF == senderPort){
                longLF = s.toInt();
            }
        }else if(info[0]=='C'){
            for(int x=2;x<info.size();x++){
                s+=info[x];
            }
            if(info[1]=='u'){
                ui->setColorUp->setValue(s.toInt());
            }else if(info[1]=='d'){
                ui->setColorDw->setValue(s.toInt());
            }
        }

        if((info == "BTR"&&bAutoSend)||(info == "LOFAR"&&bAutoSend)||(info == "DEMON"&&bAutoSend)||(info == "SENSOR A"&&bAutoSend)||(info == "SENSOR P"&&bAutoSend)){
                    if(senderPort==puertoBTR){
                        ui->view->appendPlainText("BTR: "+info);
                    } else if(senderPort==puertoLF){
                        ui->view->appendPlainText("LF : "+info);
                    } else if(senderPort==puertoPPI){
                        ui->view->appendPlainText("PPI: "+info);
                    } else if(senderPort==puertoDEMON){
                        ui->view->appendPlainText("DMN: "+info);
                    } else if(senderPort==puertoComSF){
                        ui->view->appendPlainText("CSF: "+info);
                    } else if(senderPort==puertoREC){
                        ui->view->appendPlainText("REC: "+info);
                    } else {
                        ui->view->appendPlainText("port-> " + QString("%1").arg(senderPort)+": "+info);
                    }
                }
    }
}

void siviso::on_btOpenPort_clicked()
{
    QString s;
    serialPortUSB->setPortName("/dev/ttyUSB0");
    if(serialPortUSB->open(QIODevice::ReadWrite)){
        ui->view->appendPlainText("Puerto USB serial abierto\n");
        s = "ANT_UP";
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
        serialPortUSB->setBaudRate(QSerialPort::Baud115200);
        serialPortUSB->setDataBits(QSerialPort::Data8);
        serialPortUSB->setStopBits(QSerialPort::OneStop);
        serialPortUSB->setParity(QSerialPort::NoParity);
        serialPortUSB->setFlowControl(QSerialPort::NoFlowControl);

        serialPortUSB->write("START COMMUNICATION P\n");
        thread()->msleep(100);
        serialPortUSB->write("START COMMUNICATION A\n");
        thread()->msleep(100);
    }else{
        s = "ANT_DW";
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
        ui->view->appendPlainText("Error de coexion con el puerto USB serial\n");
    }
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
    char buffer[4095];
    int nDatos;
    //numCatchSend++;
    serialPortUSB->flush();
    nDatos = serialPortUSB->read(buffer,4095);
    bool bSend = false;

    buffer[nDatos] = '\0';
    ui->textTestGrap->appendPlainText(buffer);

    QString sComSF;
    QString s;
    QString str;
    double lat;
    double log;
    QString sLat;
    QString sLog;
    str=QString(buffer);
    int n =str.size();
    //ui->textTestGrap->appendPlainText(QString::number(n));
    char c;

    numCatchSend += n;
    for(int x=0;x<str.size();x++){
        if(str[x]=='$'||str[x]=='%'||str[x]=='&'||str[x]=='@'){
            if(str[x]=='$'){
                graf = 'B';
                catchSend = "$";
            }else if(str[x]=='%'){
                graf = 'L';
                catchSend = "";
            }else if(str[x]=='&'){
                graf = 'B';
                catchSend = "";
            }else if(str[x]=='@'){
                graf = 'D';
                catchSend = "";
            }
        }else if(str[x]=='1'||str[x]=='2'||str[x]=='3'||str[x]=='4'||str[x]=='5'||str[x]=='6'||str[x]=='7'||str[x]=='8'||str[x]=='9'||str[x]=='0'||str[x]==','||str[x]==';'||str[x]=='.'||str[x]=='-'){
            bSend = true;
            ui->B0estado->setText("Enlazado");
            sComSF="P_UP";
            udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
            sComSF="";
            catchSend += str[x];
            if(str[x]==','){
                nWords++;
            }else if(str[x]==';'){
                ui->textTestGrap->appendPlainText("RECIBÍ EL PUNTO Y COMA");
                sComSF="INFO";
                udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                sComSF="";
                if(bPulso){
                    s = "CLOSE";
                    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
                    bPulso = false;
                    deshabilitado(false);
                }
                if(bAudio){
                    //if(numCatchSend>10000){
                    if(catchSend[numCatchSend-1]=='0'&&catchSend[numCatchSend]==';'){//prueba para probar el fin de la cadena---------------------------------------------------------------------------------
                        s = "CLOSE";
                        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
                        deshabilitado(false);
                        bAudio = false;
                        numCatchSend = 0;
                    } else {
                        numCatchSend = 0;
                        catchCmd = "";
                    }
                }else{
                    if(catchSend[0] == ','){
                        bSend = false;
                        ui->textTestGrap->appendPlainText("FALSE por coma inicial");
                        for(int x=0;x<catchSend.size()-1;x++){
                            if(catchSend[x] == ','){
                                if(catchSend[x+1] == ','){
                                    ui->textTestGrap->appendPlainText("FALSE por coma seguidas");
                                    bSend = false;
                                }
                            }
                        }
                    }
                }
                if(bSend){
                    sComSF="P_UP";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    ui->textTestGrap->appendPlainText("esto enviaré: "+catchSend + " \n Long de: " + QString::number(numCatchSend)+ " \n words de: " + QString::number(nWords));
                    nWords = 0;

                    if(graf=='B'&& compGraf=="BTR"){
                        udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoBTR);
                    }else if(graf=='L'&& compGraf=="LF"){
                        udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoLF);
                    }else if(graf=='D'&& compGraf=="DEMON"){
                        udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoDEMON);
                    }else if(graf=='P'&& compGraf=="PPI"){
                        //udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoPPI);
                        for(int x=0;x<catchSend.size()-1;x++){
                            if(catchSend[x] == '$'){
                                catchSend="T"+catchSend;
                                udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoPPI);
                                ui->view->appendPlainText(catchSend);
                                x=catchSend.size()-1;
                            }
                        }
                        //ui->view->appendPlainText(catchSend);
                        /*QFile file("resource/targets.txt");
                        if(file.open(QIODevice::WriteOnly)){
                            QTextStream stream(&file);
                            stream<<catchSend;
                        } else {
                            qDebug();
                        }
                        file.close();*/

                        s = "RP";
                        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
                    }
                    if(compGraf=="BAUDIO"){
                        QFile file("resource/audio.txt");
                        if(file.open(QIODevice::WriteOnly)){
                            QTextStream stream(&file);
                            stream<<catchSend;
                        } else {
                            qDebug();
                        }
                        file.close();
                        compGraf = "BTR";
                        s = "ON";
                        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
                        //ui->rec->setDisabled(false);
                        deshabilitado(false);
                    }
                    if(compGraf=="LAUDIO"){
                        //udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoComSF);
                        QFile file("resource/audio.txt");
                        if(file.open(QIODevice::WriteOnly)){
                            QTextStream stream(&file);
                            stream<<catchSend;
                        } else {
                            qDebug();
                        }
                        file.close();
                        compGraf = "LF";
                        s = "ON";
                        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
                        //ui->rec->setDisabled(false);
                        deshabilitado(false);
                    }
                    if(compGraf=="DAUDIO"){
                        //udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoComSF);
                        QFile file("resource/audio.txt");
                        if(file.open(QIODevice::WriteOnly)){
                            QTextStream stream(&file);
                            stream<<catchSend;
                        } else {
                            qDebug();
                        }
                        file.close();
                        compGraf = "DEMON";
                        s = "ON";
                        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
                        //ui->rec->setDisabled(false);
                        deshabilitado(false);
                    }
                    if(compGraf=="PAUDIO"){
                        //udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoComSF);
                        QFile file("resource/audio.txt");
                        if(file.open(QIODevice::WriteOnly)){
                            QTextStream stream(&file);
                            stream<<catchSend;
                        } else {
                            qDebug();
                        }
                        file.close();
                        compGraf = "PPI";
                        s = "ON";
                        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
                        //ui->rec->setDisabled(false);
                        deshabilitado(false);
                    }
                    if(compGraf=="AUDIO"){
                        //udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoComSF);
                        QFile file("resource/audio.txt");
                        if(file.open(QIODevice::WriteOnly)){
                            QTextStream stream(&file);
                            stream<<catchSend;
                        } else {
                            qDebug();
                        }
                        file.close();
                        //ui->rec->setDisabled(false);
                        deshabilitado(false);
                    }
                }
                numCatchSend = 0;
                catchSend="";
            }
        } else if(str[x]=='!'||str[x]=='A'||str[x]=='B'||str[x]=='C'||str[x]=='D'||str[x]=='E'||str[x]=='F'||str[x]=='H'||str[x]=='I'||str[x]=='K'||str[x]=='M'||str[x]=='N'||str[x]=='O'||str[x]=='P'||str[x]=='R'||str[x]=='S'||str[x]=='T'||str[x]=='U'){
            if(str[x]!='!'){
                catchCmd += str[x];
            } else {
                sComSF="INFO";
                udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                sComSF="";
                ui->textTestGrap->appendPlainText("comando: " + catchCmd);
                if(catchCmd == "STARTOKP"){
                    sComSF="P_UP";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    ui->B0estado->setText("Encendido");
                    edoPas = 3;
                    serialPortUSB->write("SENSORES P");
                } else if(catchCmd == "STARTOKA"){
                    sComSF="A_UP";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    ui->B1estado->setText("Encendido");
                    ui->cw->setDisabled(false);
                    ui->chirpDw->setDisabled(false);
                    ui->chirpUp->setDisabled(false);
                    ui->chype->setDisabled(false);
                    ui->chirpFrecDw->setDisabled(false);
                    ui->chirpFrecUp->setDisabled(false);
                    ui->chirpTime->setDisabled(false);
                    ui->frecP->setDisabled(false);
                    ui->nP->setDisabled(false);
                    edoAct = 3;
                    serialPortUSB->write("SENSORES A");
                } else if(catchCmd == "OKP"){
                    ui->B0estado->setText("Enlazado");
                    sComSF="CONF";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="P_UP";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                } else if(catchCmd == "OKA"){
                    ui->B1estado->setText("Enlazado");
                    sComSF="CONF";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="A_UP";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    ui->cw->setDisabled(false);
                    ui->chirpDw->setDisabled(false);
                    ui->chirpUp->setDisabled(false);
                    ui->chype->setDisabled(false);
                    ui->chirpFrecDw->setDisabled(false);
                    ui->chirpFrecUp->setDisabled(false);
                    ui->chirpTime->setDisabled(false);
                    ui->frecP->setDisabled(false);
                    ui->nP->setDisabled(false);
                } else if(catchCmd == "AUDIOOK"){
                    sComSF="CONF";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="P_UP";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="AUDIO_OPEN";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    bAudio = true;
                    ui->rec->setDisabled(true);
                } else if(catchCmd == "BIESTATICOOK"){
                    sComSF="CONF";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="P_UP";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="PULSO_OPEN";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    bPulso = true;
                    deshabilitado(true);
                } else if(catchCmd == "FINISHCOMMUNICATIONP"){
                    sComSF="P_DW";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    ui->B0estado->setText("Apagado");
                    edoPas = 0;
                } else if(catchCmd == "FINISHCOMMUNICATIONA"){
                    sComSF="A_DW";
                    udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                    sComSF="";
                    ui->cw->setDisabled(true);
                    ui->chirpDw->setDisabled(true);
                    ui->chirpUp->setDisabled(true);
                    ui->chype->setDisabled(true);
                    ui->chirpFrecDw->setDisabled(true);
                    ui->chirpFrecUp->setDisabled(true);
                    ui->chirpTime->setDisabled(true);
                    ui->frecP->setDisabled(true);
                    ui->nP->setDisabled(true);
                    ui->B1estado->setText("Apagado");
                    edoAct = 0;
                } else if(catchCmd == "COMMUNICATIONERRORP"){

                } else if(catchCmd == "COMMUNICATIONERRORA"){

                }
                catchCmd = "";
            }
        } else if(str[x]=='#'||str[x]=='a'||str[x]=='b'||str[x]=='c'||str[x]=='d'||str[x]=='e'||str[x]=='f'||str[x]=='g'||str[x]=='h'||str[x]=='i'||str[x]=='j'||str[x]=='k'||str[x]=='m'||str[x]=='n'||str[x]=='o'||str[x]=='p'){
            c = str[x].toLatin1();
            if(str[x]=='#'){
                nSensor = 0;
                tipoSensor = 9; //esta variable es para indicar que sensor se comunica, si activo "1" o pasivo "0", se inicializa en "9"
            } else if(str[x]=='k'||str[x]=='m'){
                switch(nSensor){
                case 0:
                    if(catchSensor=="0"){
                        ui->B0Nom->setText("SSPF");
                        ui->B0estado->setText("Enlazado");
                        tipoSensor = 0;
                        sComSF="P_UP";
                        udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                        sComSF="";
                        edoPas = 3;
                    }else if (catchSensor=="1"){
                        ui->B1Nom->setText("SSAF");
                        ui->B1estado->setText("Enlazado");
                        tipoSensor = 1;
                        sComSF="A_UP";
                        udpsocket->writeDatagram(sComSF.toLatin1(),direccionApp,puertoComSF);
                        sComSF="";
                        edoAct = 3;
                    }
                    catchSensor = "";
                    nSensor++;
                    break;
                case 1:
                    if(tipoSensor == 0){
                        ui->B0Or->setText(catchSensor+"°");
                    } else if(tipoSensor == 1){
                        ui->B1Or->setText(catchSensor+"°");
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
                        ui->B0Temp->setText(catchSensor+"° C");
                    } else if(tipoSensor == 1){
                        ui->B1Temp->setText(catchSensor+"° C");
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
                    if(catchSensor.toDouble()!=0){
                        sLat="";
                        lat=0.0;
                        int i=2;
                        if(catchSensor[0]=='-'){
                            i++;
                        }
                        for(;i<catchSensor.length();i++){
                            sLat+=catchSensor[i];
                        }
                        lat=sLat.toDouble();
                        lat=lat/60;
                        sLat="";
                        i=0;
                        if(catchSensor[0]=='-'){
                            i++;
                        }
                        sLat+=catchSensor[i];
                        sLat+=catchSensor[i+1];
                        lat+=sLat.toDouble();
                        if(catchSensor[0]=='-'){
                            lat*=-1;
                        }
                        sLat=QString::number(lat);
                        if(tipoSensor == 0){
                            ui->B0Lat->setText(sLat);
                            sLat="Pt"+sLat;
                            udpsocket->writeDatagram(sLat.toLatin1(),direccionApp,puertoPPI);
                        } else if(tipoSensor == 1){
                            ui->B1Lat->setText(sLat);
                            sLat="At"+sLat;
                            udpsocket->writeDatagram(sLat.toLatin1(),direccionApp,puertoPPI);
                        }
                    }
                    catchSensor = "";
                    nSensor++;
                    break;
                case 6:
                    if(catchSensor.toDouble()!=0){
                        sLog="";
                        log=0.0;
                        int i=3;
                        if(catchSensor[0]=='-'){
                            i++;
                        }
                        for(;i<catchSensor.length();i++){
                            sLog+=catchSensor[i];
                        }
                        log=sLog.toDouble();
                        log=log/60;
                        sLog="";
                        i=0;
                        if(catchSensor[0]=='-'){
                            i++;
                        }
                        sLog+=catchSensor[i];
                        sLog+=catchSensor[i+1];
                        sLog+=catchSensor[i+2];
                        log+=sLog.toDouble();
                        if(catchSensor[0]=='-'){
                            log*=-1;
                        }
                        sLog=QString::number(log);
                        if(tipoSensor == 0){
                            ui->B0Long->setText(sLog);
                            sLog="Pg"+sLog;
                            udpsocket->writeDatagram(sLog.toLatin1(),direccionApp,puertoPPI);
                        } else if(tipoSensor == 1){
                            ui->B1Long->setText(sLog);
                            sLog="Ag"+sLog;
                            udpsocket->writeDatagram(sLog.toLatin1(),direccionApp,puertoPPI);
                        }
                    }
                    catchSensor = "";
                    nSensor++;
                    break;
                case 7:
                    if(tipoSensor == 0){
                        ui->B0Carg->setText(catchSensor+" %");
                        if(catchSensor.toDouble()>20){
                            ui->Alert->setText(" ");
                        } else{
                            ui->Alert->setText("ALERTA BOYA PASIVA CON BATERIA BAJA");
                        }
                    } else if(tipoSensor == 1){
                        ui->B1Carg->setText(catchSensor+" %");
                        if(catchSensor.toDouble()>20){
                            ui->Alert->setText(" ");
                        } else{
                            ui->Alert->setText("ALERTA BOYA ACTIVA CON BATERIA BAJA");
                        }
                    }
                    nSensor++;
                    catchSensor = "";
                    break;
                case 8:
                    if(tipoSensor == 0){
                        ui->B0Volt->setText(catchSensor+" V");
                    } else if(tipoSensor == 1){
                        ui->B1Volt->setText(catchSensor+" V");
                    }
                    catchSensor = "";
                    nSensor++;
                    break;
                case 9:
                    if(tipoSensor == 0){
                        ui->B0Hu->setText(catchSensor+" %");
                    } else if(tipoSensor == 1){
                        ui->B1Hu->setText(catchSensor+" "
                                                      "%");
                    }
                    catchSensor = "";
                    bSensor=false;
                    nSensor = 0;
                    break;
                }
            } else switch(c){
            case 'a':
                catchSensor += "1";
                break;
            case 'b':
                catchSensor += "2";
                break;
            case 'c':
                catchSensor += "3";
                break;
            case 'd':
                catchSensor += "4";
                break;
            case 'e':
                catchSensor += "5";
                break;
            case 'f':
                catchSensor += "6";
                break;
            case 'g':
                catchSensor += "7";
                break;
            case 'h':
                catchSensor += "8";
                break;
            case 'i':
                catchSensor += "9";
                break;
            case 'j':
                catchSensor += "0";
                break;
            case 'k':
                catchSensor += ",";
                break;
            case 'm':
                catchSensor += ";";
                break;
            case 'n':
                catchSensor += ".";
                break;
            case 'o':
                catchSensor += ":";
                break;
            case 'p':
                catchSensor += "-";
                break;
            }
        }

    }
    /*else{
         *                 bSensor=false;
         *                 if(tipoSensor == 0){
         *                     catchSend="P_UP";
         *                 } else if(tipoSensor == 1){
         *                     catchSend="A_UP";
         *                 }
         *                 udpsocket->writeDatagram(catchSend.toLatin1(),direccionApp,puertoComSF);
         *                 catchSend="";
         *                 }*/
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
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    compGraf="LF";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    ui->setColorUp->setVisible(true);
    ui->setColorDw->setVisible(true);
    serialPortUSB->write("LOFAR P\n");
}

void siviso::on_btr_clicked()
{
    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    compGraf="BTR";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    ui->setColorUp->setVisible(true);
    ui->setColorDw->setVisible(true);
    serialPortUSB->write("BTR P\n");
}


void siviso::on_ppi_clicked()
{
    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    compGraf="PPI";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    ui->setColorUp->setVisible(false);
    ui->setColorDw->setVisible(false);
}

void siviso::on_demon_clicked()
{
    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    compGraf="DEMON";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    ui->setColorUp->setVisible(true);
    ui->setColorDw->setVisible(true);
    serialPortUSB->write("DEMON P\n");
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
    serialPortUSB->write("SET GAIN P\n");
    serialPortUSB->write(s.toLatin1()+"\n");
    thread()->msleep(100);
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
    serialPortUSB->write("AUDIO P\n");
    deshabilitado(true);
    ui->rec->setDisabled(false);
    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
    if(compGraf=="BTR"){
        compGraf = "BAUDIO";
    }else if(compGraf=="LF"){
        compGraf = "LAUDIO";
    }else if(compGraf=="DEMON"){
        compGraf = "DAUDIO";
    }else if(compGraf=="PPI"){
        compGraf = "PAUDIO";
    }else{
        compGraf = "AUDIO";
    }
}

void siviso::on_play_clicked()
{
    QString s= "PLAY";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
    ui->play->setDisabled(true);
}

void siviso::on_toolButton_clicked()
{
    if(bToolButton){
        bToolButton=false;
        ui->textTestGrap->setVisible(false);
        ui->view->setVisible(false);
        ui->save->setVisible(false);
        //ui->startCom->setVisible(false);
        ui->btOpenPort->setVisible(false);
        ui->endCom->setVisible(false);
        ui->sensor0->setVisible(false);
        ui->sensor1->setVisible(false);
        ui->closeJars->setVisible(false);
        ui->openJars->setVisible(false);
        ui->textSend->setVisible(false);
        //ui->send->setVisible(false);
        ui->infoSignal->setVisible(false);
        ui->ApagarP->setVisible(false);
        ui->ApagarA->setVisible(false);
        //ui->sensorOFF->setVisible(false);
        //ui->sensorON->setVisible(false);
        ui->dataSim->setVisible(false);
        ui->dist->setVisible(false);
        ui->pulso->setVisible(false);
        ui->GPSOr->setVisible(false);
        ui->autoSend->setVisible(false);
        //ui->btLF->setVisible(false);
        ui->prog->setVisible(false);
        ui->factor_p->setVisible(false);
        ui->central_frec->setVisible(false);
        ui->dTotal->setVisible(false);
        ui->dPaso->setVisible(false);
        ui->Uu->setVisible(false);
        ui->Ud->setVisible(false);
        ui->limProm->setVisible(false);
        ui->clean->setVisible(false);
        ui->label_70->setVisible(false);
        ui->label_71->setVisible(false);
        ui->label_72->setVisible(false);
        ui->label_76->setVisible(false);
        ui->label_78->setVisible(false);
        ui->label_79->setVisible(false);
        ui->windowing->setVisible(false);
        ui->prefilter->setVisible(false);
    }else{
        bToolButton=true;
        ui->textTestGrap->setVisible(true);
        ui->view->setVisible(true);
        ui->save->setVisible(true);
        //ui->startCom->setVisible(true);
        ui->btOpenPort->setVisible(true);
        ui->endCom->setVisible(true);
        ui->sensor0->setVisible(true);
        ui->sensor1->setVisible(true);
        ui->closeJars->setVisible(true);
        ui->openJars->setVisible(true);
        ui->textSend->setVisible(true);
        //ui->send->setVisible(true);
        ui->infoSignal->setVisible(true);
        ui->ApagarP->setVisible(true);
        ui->ApagarA->setVisible(true);
        //ui->sensorOFF->setVisible(true);
        //ui->sensorON->setVisible(true);
        ui->dataSim->setVisible(true);
        ui->dist->setVisible(true);
        ui->pulso->setVisible(true);
        ui->GPSOr->setVisible(true);
        ui->autoSend->setVisible(true);
        //ui->btLF->setVisible(true);
        ui->prog->setVisible(true);        
        ui->factor_p->setVisible(true);
        ui->central_frec->setVisible(true);
        ui->dTotal->setVisible(true);
        ui->dPaso->setVisible(true);
        ui->Uu->setVisible(true);
        ui->Ud->setVisible(true);
        ui->limProm->setVisible(true);
        ui->clean->setVisible(true);
        ui->label_70->setVisible(true);
        ui->label_71->setVisible(true);
        ui->label_72->setVisible(true);
        ui->label_76->setVisible(true);
        ui->label_78->setVisible(true);
        ui->label_79->setVisible(true);
        ui->windowing->setVisible(true);
        ui->prefilter->setVisible(true);
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
    if(compGraf=="DEMON")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    s = "P_UP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
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
        qDebug();bool test = true;
        test ? "1" : "0";
    }
    file.close();

    QString s="";
    s = "Cu"+QString::number(colorUp);
    //ui->view->appendPlainText(s);
    if(compGraf=="BTR")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    if(compGraf=="LF")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    if(compGraf=="DEMON")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);

    /*s = "RP";
    if(compGraf=="BTR")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    if(compGraf=="LF")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    if(compGraf=="DEMON")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);*/
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

    QString s="";
    s = "Cd"+QString::number(colorDw);
    //ui->view->appendPlainText(s);
    if(compGraf=="BTR")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    if(compGraf=="LF")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    if(compGraf=="DEMON")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);

    /*s = "RP";
    if(compGraf=="BTR")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    if(compGraf=="LF")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    if(compGraf=="DEMON")
        udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);*/
}

void siviso::on_frecP_valueChanged(int arg1)
{
    serialPortUSB->write("SET CENTRAL FREQUENCY A\n");
    QString s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
    serialPortUSB->write("SET CENTRAL FREQUENCY P\n");
    s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_nP_valueChanged(int arg1)
{
    serialPortUSB->write("SET N PULSOS A\n");
    QString s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_anchoP_valueChanged(int arg1)
{
    serialPortUSB->write("SET ANCHO PULSO A\n");
    QString s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_cw_clicked()
{
    //deshabilitado(true);
    //serialPortUSB->write("ENCENDER A\n");
    //serialPortUSB->write("ENCENDER P\n");
    /*QString s = "PULSO";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);*/



    QString s;
    s = "OFF";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    s = "ON";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    compGraf="PPI";
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    serialPortUSB->write("BIESTATICO\n");
    thread()->msleep(100);
    serialPortUSB->write("BURST A\n");
    thread()->msleep(100);
}

void siviso::on_startCom_clicked()
{
    serialPortUSB->write("END COMMUNICATION A\n");
    thread()->msleep(100);
    serialPortUSB->write("END COMMUNICATION P\n");
    thread()->msleep(100);
    //if(edoPas < 2){
        serialPortUSB->write("START COMMUNICATION P\n");
        thread()->msleep(100);
    //}
    //thread()->sleep(3);
    //if(edoAct < 2){
        serialPortUSB->write("START COMMUNICATION A\n");
    //}
    //QThread::msleep(1000);
    //serialPortUSB->write("SET CENTRAL FREQUENCY A\n");
    //serialPortUSB->write(mysignal->get_frecP()+"\n");
    //QThread::msleep(1000);
    //serialPortUSB->write("SET N PULSOS A\n");
    //serialPortUSB->write(mysignal->get_nP()+"\n");
}

void siviso::on_endCom_clicked()
{
    serialPortUSB->write("END COMMUNICATION A\n");
    thread()->sleep(1);
    serialPortUSB->write("END COMMUNICATION P\n");
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
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    //udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
}

void siviso::on_openJars_clicked()
{
    QString s;
    s = "EXIT";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoDEMON);
    //udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);

    compGraf="";
    thread()->sleep(1);
    proceso2->startDetached("java -jar BTR.jar");
    thread()->sleep(1);
    proceso1->startDetached("java -jar Lofar.jar");
    thread()->sleep(1);
    proceso4->startDetached("java -jar demon.jar");
    thread()->sleep(1);
    proceso3->startDetached("java -jar PPI.jar");
    thread()->sleep(1);
    //proceso5->startDetached("java -jar ConexionSF.jar");
    //thread()->sleep(1);
}


void siviso::on_dial_valueChanged(int value)
{
    ui->ang->setValue(value);
}

void siviso::on_ang_valueChanged(int arg1)
{
    if(arg1<=180){
        ui->dial->setValue(arg1);
    }else{
        int n = arg1-360;
        ui->dial->setValue(n);
    }
    if(arg1<0){
        arg1+=360;
    }
   /* QFile file("resource/angMarc.txt");
    if(file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream<<arg1;
    } else {
        qDebug();
    }
    file.close();*/
    QString s = "a"+QString::number(arg1);
    ui->view->appendPlainText(s);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_origenOmni_clicked()
{
    serialPortUSB->write("SET HIDROFONO P\n");
    serialPortUSB->write("5 \n");
    thread()->msleep(100);
    ui->ang->setVisible(false);
    ui->dial->setDisabled(true);
}

void siviso::on_origenManual_clicked()
{
    serialPortUSB->write("SET HIDROFONO P\n");
    serialPortUSB->write("0 \n");
    thread()->msleep(100);
    ui->ang->setVisible(true);
    ui->dial->setDisabled(false);
}

void siviso::on_dial_sliderReleased()
{
    serialPortUSB->write("SET ANGLE P\n");
    ui->textTestGrap->appendPlainText("SET ANGLE P\n");
    int n = ui->ang->value();
    if(n<0)
        n+=360;
    QString s = QString::number(n);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
    ui->textTestGrap->appendPlainText(s);
}

void siviso::on_ang_editingFinished()
{
    serialPortUSB->write("SET ANGLE P\n");
    ui->textTestGrap->appendPlainText("SET ANGLE P\n");
    int n = ui->ang->value();
    if(n<0)
        n+=360;
    QString s = QString::number(n);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
    ui->textTestGrap->appendPlainText(s);
}

/*void siviso::on_send_clicked()
{
    QString s;
    s = ui->textSend->text();

    QByteArray data = "prueba"; // <-- fill with data
    data = "ui->textSend->text()";

        pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * pSocket;
        connect( pSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );

        pSocket->connectToHost("192.168.1.10", 6001);
        if( pSocket->waitForConnected() ) {
            pSocket->write( data );
        }

    QString send = "";
    if(s[0] == '>'){
        if(s[1]=='B'||s[1]=='L'||s[1]=='P'){
            for(int n=2;n<s.length();n++){
                if(s[n]!=' '){
                    send += s[n];
                }
            }
            if(s[1]=='B'){
                ui->view->appendPlainText("BTR: "+send);
                udpsocket->writeDatagram(send.toLatin1(),direccionApp,puertoBTR);
            }
            if(s[1]=='L'){
                ui->view->appendPlainText("LF: "+send);
                udpsocket->writeDatagram(send.toLatin1(),direccionApp,puertoLF);
            }
            if(s[1]=='P'){
                ui->view->appendPlainText("PPI: "+send);
                udpsocket->writeDatagram(send.toLatin1(),direccionApp,puertoPPI);
            }
        }
    }
    serialPortUSB->write(s.toLatin1()+"\n");
    ui->textSend->clear();
}*/

void siviso::on_infoSignal_clicked()
{
    ui->view->appendPlainText("info: ");
    QString s = mysignal->get_info_signal();
   /* ui->view->appendPlainText(s);
    ui->view->appendPlainText("portSSF " + QString("%1").arg(puertoSSF));
    ui->view->appendPlainText("portBTR " + QString("%1").arg(puertoBTR));
    ui->view->appendPlainText("portLF " + QString("%1").arg(puertoLF));
    ui->view->appendPlainText("portPPI " + QString("%1").arg(puertoPPI));
    ui->view->appendPlainText("portDEMON " + QString("%1").arg(puertoDEMON));*/
    s = "P_DW";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
}

void siviso::deshabilitado(bool value){
    //ui->frecuencia->setDisabled(value);
    ui->bw->setDisabled(value);
    ui->it->setDisabled(value);
    ui->dt->setDisabled(value);
    ui->cw->setDisabled(value);
    ui->frecP->setDisabled(value);
    ui->nP->setDisabled(value);
    //ui->anchoP->setDisabled(value);
    ui->ran_det->setDisabled(value);
    ui->tipo_norte->setDisabled(value);
    ui->origenBuque->setDisabled(value);
    ui->origenSensor->setDisabled(value);
    ui->origenBlanco->setDisabled(value);
    ui->origenOmni->setDisabled(value);
    ui->origenAuto->setDisabled(value);
    ui->origenManual->setDisabled(value);
    ui->ang->setDisabled(value);
    ui->rec->setDisabled(value);
    ui->play->setDisabled(value);
    //ui->et_blancos->setDisabled(value);
    //ui->clas_blancos->setDisabled(value);
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
    ui->demon->setDisabled(value);
    ui->startCom->setDisabled(value);
    ui->chirpUp->setDisabled(value);
    ui->chirpDw->setDisabled(value);
    ui->chype->setDisabled(value);
    if(ui->origenOmni->isChecked()){
        ui->dial->setDisabled(value);
    }
}


void siviso::on_ApagarP_clicked()
{
    serialPortUSB->write("APAGAR P\n");
}

void siviso::on_ApagarA_clicked()
{
    serialPortUSB->write("APAGAR A\n");
}

void siviso::on_sensorOFF_clicked()
{
    QString s;
    s = "EXIT";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);
}

void siviso::on_sensorON_clicked()
{
    QString s;
    s = "EXIT";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoComSF);

    proceso4->startDetached("java -jar ConexionSF.jar");
}

void siviso::on_vol_dw_clicked()
{
    proceso3->startDetached("amixer sset Master 5%-");
}

void siviso::on_vol_up_clicked()
{
    proceso3->startDetached("amixer sset Master 5%+");
}

void siviso::on_chirpUp_clicked()
{
    serialPortUSB->write("UP LFM A\n");
}

void siviso::on_chirpDw_clicked()
{
    serialPortUSB->write("DOWN LFM A\n");
}

void siviso::on_chype_clicked()
{
    serialPortUSB->write("UP HFM A\n");
}

void siviso::on_chirpFrecUp_valueChanged(int arg1)
{
    if(arg1<=ui->chirpFrecDw->value()){
        ui->chirpFrecDw->setValue(arg1-1);
        thread()->msleep(100);
    }
    serialPortUSB->write("SET HIGH FREQUENCY A\n");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_chirpFrecDw_valueChanged(int arg1)
{
    if(arg1>=ui->chirpFrecUp->value()){
        ui->chirpFrecUp->setValue(arg1+1);
        thread()->msleep(100);
    }
    serialPortUSB->write("SET LOW FREQUENCY A\n");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_chirpTime_valueChanged(double arg1)
{
    serialPortUSB->write("SET TIME DURATION A\n");
    QString s = QString::number(arg1);
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_chirpFrecUp_editingFinished()
{
    if(ui->chirpFrecUp->value()<=ui->chirpFrecDw->value()){
        ui->chirpFrecDw->setValue(ui->chirpFrecUp->value()-1);
        thread()->msleep(100);
    }
    serialPortUSB->write("SET HIGH FREQUENCY A\n");
    QString s = QString::number(ui->chirpFrecUp->value());
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_chirpFrecDw_editingFinished()
{
    if(ui->chirpFrecDw->value()>=ui->chirpFrecUp->value()){
        ui->chirpFrecUp->setValue(ui->chirpFrecDw->value()+1);
        thread()->msleep(100);
    }
    serialPortUSB->write("SET LOW FREQUENCY A\n");
    QString s = QString::number(ui->chirpFrecDw->value());
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_chirpTime_editingFinished()
{
    serialPortUSB->write("SET TIME DURATION A\n");
    QString s = QString::number(ui->chirpTime->value());
    serialPortUSB->write(s.toLatin1() + "\n");
    thread()->msleep(100);
}

void siviso::on_prob_deteccion_valueChanged(int arg1)
{
    /*mysignal->set_ganancia_sensor(arg1);

    ui->view->appendPlainText("ganancia_sensor: ");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
    serialPortUSB->write("SET GAIN1 P\n");
    serialPortUSB->write(s.toLatin1()+"\n");
    thread()->msleep(100);*/

    //serialPortUSB->write("SET THRESHOLD P\n");
    QString s = QString::number(arg1);
    ui->view->appendPlainText(s);
    serialPortUSB->write("SET T P\n");
    thread()->msleep(100);
    serialPortUSB->write(s.toLatin1() + "\n");
    //serialPortUSB->write("\n");
    thread()->msleep(100);
}

void siviso::on_dataSim_clicked()
{
    QString s = "T150$100,99,220,70,798$100,99,220,70,799$100,99,220,70,802$175,100,22,70,812$130,160,74,63,817$175,100,227,70,820$0;";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_dist_clicked()
{
    QString s = "DIS";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_escala_ppi_valueChanged(int arg1)
{
    QString s = "S";
    s += QString::number(arg1);
    ui->view->appendPlainText(s);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "RP";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_textSend_editingFinished()
{
    QString s = "";
    QString send = "";
    s = ui->textSend->text();
    if(s[0] == '>'){
        if(s[1]=='B'||s[1]=='L'||s[1]=='P'||s[1]=='G'){
            ui->view->appendPlainText("[Comando]");
            for(int n=2;n<s.length();n++){
                if(s[n]!=' '){
                    send += s[n];
                }
            }
            if(s[1]=='B'){
                ui->view->appendPlainText("Enviar a BTR: "+send);
                udpsocket->writeDatagram(send.toLatin1(),direccionApp,puertoBTR);
            }
            if(s[1]=='L'){
                ui->view->appendPlainText("Enviar a LF: "+send);
                udpsocket->writeDatagram(send.toLatin1(),direccionApp,puertoLF);
            }
            if(s[1]=='P'){
                ui->view->appendPlainText("Enviar a PPI: "+send);
                udpsocket->writeDatagram(send.toLatin1(),direccionApp,puertoPPI);
            }
            if(s[1]=='G'){
                if(send.toDouble()!=0){
                    QString sLatLog="";
                    double latLog=0.0;
                    int i=2;
                    if(send[0]=='-'){
                        i++;
                    }
                    for(;i<send.length();i++){
                        sLatLog+=send[i];
                    }
                    latLog=sLatLog.toDouble();
                    latLog=latLog/60;
                    sLatLog="";
                    i=0;
                    if(send[0]=='-'){
                        i++;
                    }
                    sLatLog+=send[i];
                    sLatLog+=send[i+1];
                    latLog+=sLatLog.toDouble();
                    if(send[0]=='-'){
                        latLog*=-1;
                    }
                    sLatLog=QString::number(latLog);
                    ui->view->appendPlainText("Convertir GPS Lat/Log: "+sLatLog);
                }
            }
        }
    }
    serialPortUSB->write(s.toLatin1()+"\n");
    ui->textSend->clear();
}

void siviso::on_GPSOr_clicked()
{
    QString s = "Ot19.0445";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "Og-95.9717";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_pulso_clicked()
{
    serialPortUSB->write("BURST A\n");
    //thread()->msleep(2000);
    //serialPortUSB->write("LOFAR P\n");
    thread()->msleep(100);
}

void siviso::on_prog_clicked()
{
    QString s = "At19.04501";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "Ag-95.97261";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "Pt19.045";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "Pg-95.9726";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
    s = "DIS";
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoPPI);
}

void siviso::on_autoSend_clicked()
{
    if(bAutoSend){
        bAutoSend=false;
        ui->autoSend->setText("Manual");
    }else{
        bAutoSend=true;
        ui->autoSend->setText("Automatico");
    }
}

void siviso::on_btLF_clicked()
{
    serialPortUSB->write("LOFAR P\n");
}

void siviso::on_Uu_valueChanged(int arg1)
{
    QString s = "Uu";
    s += QString::number(arg1);
    ui->view->appendPlainText(s);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
}

void siviso::on_Ud_valueChanged(int arg1)
{
    QString s = "Ud";
    s += QString::number(arg1);
    ui->view->appendPlainText(s);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoBTR);
}

void siviso::on_limProm_editingFinished()
{
    QString s = "P";
    s += QString::number(ui->limProm->value());
    ui->view->appendPlainText(s);
    udpsocket->writeDatagram(s.toLatin1(),direccionApp,puertoLF);
}

void siviso::on_clean_clicked()
{
    ui->textTestGrap->clear();
    ui->view->clear();
}

void siviso::on_dTotal_editingFinished()
{
    serialPortUSB->write("SET D TOTAL P\n");
    QString s = QString::number(ui->dTotal->value());
    serialPortUSB->write(s.toLatin1()+"\n");
}

void siviso::on_dPaso_editingFinished()
{
    serialPortUSB->write("SET D PASO P\n");
    QString s = QString::number(ui->dPaso->value());
    serialPortUSB->write(s.toLatin1()+"\n");
}

void siviso::on_central_frec_editingFinished()
{
    serialPortUSB->write("SET CENTRAL FREQUENCY P\n");
    QString s = QString::number(ui->central_frec->value());
    serialPortUSB->write(s.toLatin1()+"\n");
}

void siviso::on_factor_p_editingFinished()
{
    serialPortUSB->write("SET R FACTOR P\n");
    QString s = QString::number(ui->factor_p->value());
    serialPortUSB->write(s.toLatin1()+"\n");
}

void siviso::on_water_vel_editingFinished()
{
    serialPortUSB->write("SET WATER VELOCITY P\n");
    QString s = QString::number(ui->water_vel->value());
    serialPortUSB->write(s.toLatin1()+"\n");
}

void siviso::on_windowing_clicked()
{
    serialPortUSB->write("SET WINDOWING P\n");
    if(ui->windowing->checkState()){
        serialPortUSB->write("1\n");
    }else{
        serialPortUSB->write("0\n");
    }
}

void siviso::on_prefilter_clicked()
{
    serialPortUSB->write("SET PREFILTER P\n");
    if(ui->prefilter->checkState()){
        serialPortUSB->write("1\n");
    }else{
        serialPortUSB->write("0\n");
    }
}
