#ifndef DATABASE_TESTTOOL_H
#define DATABASE_TESTTOOL_H

#include <QWidget>
#include <string>

#include "datacenter.h"

class QThread;

using namespace std;

namespace Ui {
class DataBase_TestTool;
}


class DataBase_TestTool : public QWidget
{
    Q_OBJECT

public:

public:
    explicit DataBase_TestTool(QWidget *parent = nullptr);
    ~DataBase_TestTool();

signals:

private slots:
    void on_btn_connectDB_clicked();
    void on_btn_add_clicked();
    void on_btn_query_clicked();
    void on_btn_showAll_clicked();
    void on_btn_delete_clicked();
    void on_btn_update_clicked();
    void on_btn_queryExec_clicked();
    void on_btn_addDataMore_clicked();
    void on_btn_deleteAll_clicked();
    void on_btn_dbTotalCount_clicked();



private slots:
    void slotConnectStatus(bool flag);
    void slotAddDataStatus(bool flag);
    void slotAddMoreDataStatus(bool flag);


private:
    void initAddData();
    void updateDataTabel();
    bool testAddData();
    void testQueryData();

private:
    Ui::DataBase_TestTool *ui;

    QThread *m_pThread;
    DataCenter *m_pDataCenter;
};

#endif // DATABASE_TESTTOOL_H
