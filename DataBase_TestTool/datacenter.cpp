#include "datacenter.h"

#include<QDebug>
#include<QSqlDatabase>

#include "robotdatabaseapi.h"

DataCenter::DataCenter(QObject *parent)
    : QObject(parent)
{

}

bool DataCenter::connectDB()
{
    return DB_connect();
}

bool DataCenter::addData(string strOrderId, string strEntry, string strMoney, string strPlate, string strType, string strWeight)
{
    return true;
}

bool DataCenter::addMoreData(int count)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection1");
    db.setDatabaseName("trawe.db"); // 数据库文件名

    if (!db.open())
    {
        qDebug() << "Failed to connect to database 1.";
        emit sigAddMoreDataStatus(false);
        return false;
    }


    QStringList types;
    types << "客一"<< "客二" << "客三" << "客四" << "货一" << "货二" << "货三" << "货四";


    QStringList entrys;
    entrys << "天河"<< "滘口" << "南沙" << "海珠" << "白云" << "增城" << "荔湾" << "越秀";


    tagOrderParam tempParam;


    for(int index = 1; index < (count +index); ++index)
    {
        tempParam.strOrderID = QString::number(index).toStdString();

        tempParam.strEntry = entrys[index%8].toStdString();
        tempParam.strMoney = "99";
        tempParam.strPlate = "粤A777Q1";
        tempParam.strType = types[index%8].toStdString();
        tempParam.strWeight = QString::number(index).toStdString();


        bool ret = DB_addData(tempParam.strOrderID, tempParam.strEntry, tempParam.strMoney,
                              tempParam.strPlate, tempParam.strType, tempParam.strWeight);

        if(ret)
        {
            qDebug() << "序号为 (" << index << ") 添加数据成功！";
        }
        else
        {
            qDebug() << "添加数据失败！";
            break;
        }
    }

    db.close();
    emit sigAddMoreDataStatus(true);
    return true;
}

bool DataCenter::DeleteDataById(string strOrderd)
{

    return true;
}

bool DataCenter::UpdateDataById(string strOrderId, string strEntry, string strMoney, string strPlate, string strType, string strWeight)
{

    return true;
}

bool DataCenter::getDataById(string strOrderId, string &strEntry, string &strMoney, string &strPlate, string &strType, string &strWeight)
{

    return true;
}

bool DataCenter::slotConnectDB()
{
    bool ret = DB_connect();
    emit sigConnectStatus(ret);

    return ret;
}

bool DataCenter::slotAddMoreData()
{
    int count = 100;
    addMoreData(count);
}
