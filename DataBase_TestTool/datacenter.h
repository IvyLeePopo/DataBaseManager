#ifndef DATACENTER_H
#define DATACENTER_H

#include <QObject>
#include <string>

using namespace std;

class DataCenter : public QObject
{
    Q_OBJECT

public:
    DataCenter(QObject *parent = nullptr);

    bool connectDB();

    // add
    bool addData(string strOrderId, string strEntry, string strMoney, string strPlate, string strType, string strWeight);
    bool addMoreData(int count);
    void setCount(int count){ m_nCount = count; }

    // delete
    bool DeleteDataById(string strOrderd);

    // update
    bool UpdateDataById(string strOrderId, string strEntry, string strMoney, string strPlate, string strType, string strWeight);

    // query
    bool getDataById(string strOrderId, string& strEntry, string& strMoney, string& strPlate, string& strType, string& strWeight);

signals:
    void sigConnectStatus(bool falg);
    void sigAddMoreDataStatus(bool falg);


public slots:
    bool slotConnectDB();
    bool slotAddMoreData();

private:
    int m_nCount = 0;

};

#endif // DATACENTER_H
