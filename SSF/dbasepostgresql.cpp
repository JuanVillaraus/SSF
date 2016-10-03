#include "dbasepostgresql.h"

/*DBasePostgreSQL::DBasePostgreSQL(QString server, QString dataBaseName, int port, QString user, QString password)
{
    connectDB = QSqlDatabase::addDatabase("QPSQL");
    connectDB.setHostName(server);
    connectDB.setPort(port);
    connectDB.setUserName(user);
    connectDB.setPassword(password);
    connectDB.setDatabaseName(dataBaseName);
}

void DBasePostgreSQL::openDB()
{
    connectDB.open();
    if (connectDB.isOpen()==true)
        connectionState = true;
    else
        connectionState = false;

    qDebug("Open DB");
}

void DBasePostgreSQL::closeDB()
{
    connectDB.close();
    connectionState = false;
    qDebug("Close DB");
}

QSqlDatabase DBasePostgreSQL::getConnection()
{
    return connectDB;
}

QSqlQuery DBasePostgreSQL::executeSql(QString command)
{
    if (connectDB.isOpen()==true)
    {
    openDB();
    QSqlQuery query(command, getConnection());
        return query;
    }
    else
    {
        openDB();
        QSqlQuery query(command, getConnection());
        return query;
    }
}*/
