#ifndef ROBOTDATABASEMANAGER_H
#define ROBOTDATABASEMANAGER_H

#include "robot_database_global.h"
#include <QObject>

#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QMutex>

#include <string>
#include "LinuxTimerManager.h"

using namespace std;

class RobotDataBaseManager : public QObject
{

    Q_OBJECT

public:
    static RobotDataBaseManager* instance();

    bool connectDB();

    bool disConnectDB();

    // add
    bool addData2DB(string strOrderId, string strEntry, string strMoney, string strPlate, string strType, string strWeight);

    // delete
    bool DeleteDataById(string strOrderd);

    // update
    bool UpdateDataById(string strOrderId, string strEntry, string strMoney, string strPlate, string strType, string strWeight);

    // query
    int getTypeById(string id);
    bool getDataById(string strOrderId, string& strEntry, string& strMoney, string& strPlate, string& strType, string& strWeight);

    // 执行sql语句
    bool queryExec(string strSql);

    // 查询数据库的大小
    int countDB();

    QString getCurrentOfCancelPayId();

    bool bConnected() {return m_bConnected;}

private:
    RobotDataBaseManager();
    ~RobotDataBaseManager();

    bool connDB();
    void closeDB();
    bool openDB();
    QSqlDatabase database();
    bool isOrderIdExist(string strOrderId);

private:
    static RobotDataBaseManager *self;
    QSqlDatabase db;

    QString dbDriver;
    QString dbName;
    QString userName;
    QString userPwd;
    QString hostName;

    int hostPort;

    bool m_bConnected;

private:
    QString m_curOrderId;
    QMutex m_mutex4OrderId;

    timer_t m_deleteTimerId;

public :

   static  void deleteTimerProc(timer_t data);

public:

    bool checkColumnExists2();

};

#endif // ROBOTDATABASEMANAGER_H
