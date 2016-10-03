#ifndef DBASEPOSTGRESQL_H
#define DBASEPOSTGRESQL_H

//---------Class dbasepostgresql for Misael M Del Valle

#include <QString>
/*#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QTableView>
#include <QStandardItemModel>*/
#include <QDebug>

class DBasePostgreSQL : public QObject
{

Q_OBJECT

public:
    DBasePostgreSQL(QString server, QString dataBaseName, int port, QString user, QString password);
    //QSqlDatabase getConnection();
    //QSqlQuery executeSql(QString command);
    bool getConnectionState();

 private:
    //QSqlDatabase connectDB;
    bool connectionState;

 protected:
    void openDB();
    void closeDB();

  signals:
    //void emitError(QSqlError error);

};

#endif // DBASEPOSTGRESQL_H
