#include "robotdatabaseapi.h"

#include "robotdatabasemanager.h"

bool DB_connect()
{
    //链接数据库
    RobotDataBaseManager::instance();
    return RobotDataBaseManager::instance()->bConnected();
}

bool DB_disConnect()
{
    return RobotDataBaseManager::instance()->disConnectDB();
}

bool DB_addData(string orderId, string entry, string money, string plate, string type, string weight)
{
    return RobotDataBaseManager::instance()->addData2DB(orderId, entry, money, plate, type, weight);
}

bool DB_deleteData(string id)
{
    //return RobotDataBaseManager::instance()->DeleteDataById(id);

    return false;
}

bool DB_updateData(string orderId, string entry, string money, string plate, string type, string weight)
{
    return RobotDataBaseManager::instance()->UpdateDataById(orderId, entry, money, plate, type, weight);
}

bool DB_queryDataTypeById(string id)
{
    return RobotDataBaseManager::instance()->getTypeById(id);
}

bool DB_queryDataById(string id, string &entry, string &money, string &plate, string &type, string &weight)
{
    return RobotDataBaseManager::instance()->getDataById(id, entry, money, plate, type, weight);
}

bool DB_queryExec(string strSql)
{
    return RobotDataBaseManager::instance()->queryExec(strSql);
}

int DB_totalCount()
{
    return RobotDataBaseManager::instance()->countDB();
}
