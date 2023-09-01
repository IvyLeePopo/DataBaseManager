#include "database_testtool.h"
#include "ui_database_testtool.h"

#include<QPushButton>
#include<QLibrary>
#include<QDebug>
#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QThread>

#include "robotdatabaseapi.h"

#define DB_NAME  "trawe.db"

DataBase_TestTool::DataBase_TestTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataBase_TestTool)
{
    ui->setupUi(this);
    ui->textEdit_sql->setLineWrapMode(QTextEdit::WidgetWidth);

    m_pThread = new QThread();
    m_pDataCenter = new DataCenter();
    m_pDataCenter->moveToThread(m_pThread);

    //connect(m_pThread, &QThread::started, m_pDataCenter, &DataCenter::slotAddMoreData);
    //connect(m_pDataCenter, &DataCenter::sigAddMoreDataStatus, this, &DataBase_TestTool::slotAddMoreDataStatus);

}

DataBase_TestTool::~DataBase_TestTool()
{
    delete ui;
    DB_disConnect();
}

void DataBase_TestTool::on_btn_connectDB_clicked()
{
    qDebug() << __LINE__ << __FUNCTION__ << "开始连接数据库:" ;

    bool ret = DB_connect();

    if(ret)
    {
        qDebug() << "连接数据库成功！";
        ui->label_connectStatus->setText("连接数据库成功！");
    }
    else
    {
        qDebug() << "连接数据库失败！";
        ui->label_connectStatus->setText("连接数据库失败！");
    }
}

void DataBase_TestTool::initAddData()
{
    QUuid id = QUuid::createUuid();
    QString strId = id.toString();
    ui->lineEdit_orderId->setText(strId);
}

void DataBase_TestTool::updateDataTabel()
{
    //on_btn_showAll_clicked();
}

bool DataBase_TestTool::testAddData()
{
    QStringList types;
    types << "客一"<< "客二" << "客三" << "客四" << "货一" << "货二" << "货三" << "货四";


    QStringList entrys;
    entrys << "天河"<< "滘口" << "南沙" << "海珠" << "白云" << "增城" << "荔湾" << "越秀";


    int count = ui->lineEdit_count->text().toInt();
    int index = ui->lineEdit_index->text().toInt();

    tagOrderParam tempParam;
    int sum = index + count;
    for(; index < sum; ++index)
    {
        tempParam.strOrderID = QString::number(index).toStdString();

        tempParam.strEntry = entrys[index%8].toStdString();
        tempParam.strMoney = "99";
        tempParam.strPlate = "粤A777Q1";
        tempParam.strType = types[index%8].toStdString();
        tempParam.strWeight = QString::number(index).toStdString();


        bool ret = DB_addData(tempParam.strOrderID, tempParam.strEntry, tempParam.strMoney,
                              tempParam.strPlate, tempParam.strType, tempParam.strWeight);

        if(!ret)
        {
            qDebug() << "添加数据失败！";
            return false;
        }
    }

    qDebug() << count << " 条数据，添加成功！";
    return true;
}

void DataBase_TestTool::testQueryData()
{

}

void DataBase_TestTool::on_btn_add_clicked()
{
    qDebug() << __LINE__ << __FUNCTION__ << "开始添加数据:" ;

    tagOrderParam tempParam;
    tempParam.strOrderID = ui->lineEdit_orderId->text().toStdString();
    tempParam.strEntry = ui->lineEdit_entry->text().toStdString();
    tempParam.strMoney = ui->lineEdit_money->text().toStdString();
    tempParam.strPlate = ui->lineEdit_plate->text().toStdString();
    tempParam.strType = ui->comboBox_type->currentText().toStdString();
    tempParam.strWeight = ui->lineEdit_weight->text().toStdString();

    bool ret = DB_addData(tempParam.strOrderID, tempParam.strEntry, tempParam.strMoney,
                          tempParam.strPlate, tempParam.strType, tempParam.strWeight);

    if(ret)
    {
        qDebug() << "添加数据成功！";
        ui->label_addDataStatus->setText("添加成功！");
        updateDataTabel();
    }
    else
    {
        qDebug() << "添加数据失败！";
        ui->label_addDataStatus->setText("添加失败！");
    }
}

void DataBase_TestTool::on_btn_query_clicked()
{
    qDebug() << __LINE__ << __FUNCTION__ << "开始查询数据:" ;

    string orderId = ui->lineEdit_queryOrderID->text().toStdString();

    tagOrderParam tempParam;

    bool ret = DB_queryDataById(orderId, tempParam.strEntry, tempParam.strMoney, tempParam.strPlate, tempParam.strType, tempParam.strWeight);
    if(ret)
    {
        qDebug() << "查询数据成功！";
        ui->label_queryStatus->setText("查询成功！");
        qDebug() << "入口信息" << QString::fromStdString(tempParam.strEntry);

        ui->label_entryQuery->setText(QString::fromStdString(tempParam.strEntry));
        ui->label_moneyQuery->setText(QString::fromStdString(tempParam.strMoney));
        ui->label_plateQuery->setText(QString::fromStdString(tempParam.strPlate));
        ui->label_typeQuery->setText(QString::fromStdString(tempParam.strType));
        ui->label_weightQuery->setText(QString::fromStdString(tempParam.strWeight));
    }
    else
    {
        qDebug() << "查询数据失败！";
        ui->label_queryStatus->setText("查询失败！");
    }
}

void DataBase_TestTool::on_btn_showAll_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection1");
    db.setDatabaseName("trawe.db"); // 数据库文件名


    if (!db.open())
    {
        qDebug() << "Failed to connect to database 1.";
        return;
    }

    QSqlQuery query(db);
    QString selectQuery = "SELECT * FROM orderData";
    if (!query.exec(selectQuery))
    {
        qDebug() << "Failed to select data from trawe.db.";
        return;
    }

    QSqlTableModel * model = new QSqlTableModel(this,db);
    model->setTable("orderData");
    ui->tableView->setModel(model);
    model->select();

    QStringList tables;
    tables << "id"<< "订单id" << "入口" << "金额" << "车牌" << "车型" << "重量" << "时间";
    for(int i = 1 ; i < tables.count(); i++)
        model->setHeaderData(i,Qt::Horizontal,tables[i]);//设置显示框表头显示
    model->setSort(1, Qt::AscendingOrder);//设置按照第0列排序
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    ui->tableView->horizontalHeader()->setStretchLastSection(true);//设置最后一列填充后面表格
    //ui->tableView->setColumnHidden(0,true);//设置第0行隐藏
    ui->tableView->setColumnWidth(1,100);//设置列宽，界面看起来更舒适
    ui->tableView->setColumnWidth(2,100);//设置列宽，界面看起来更舒适

    db.close();
}

void DataBase_TestTool::on_btn_delete_clicked()
{
    qDebug() << __LINE__ << __FUNCTION__ << "开始删除数据:" ;


    string orderId = ui->lineEdit_orderIdDelete->text().toStdString();

    bool ret = false;// DB_deleteData(orderId);

    if(ret)
    {
        qDebug() << "删除数据成功！";
        ui->label_deleteDataStatus->setText("删除成功！");
        updateDataTabel();
    }
    else
    {
        qDebug() << "删除数据失败！";
        ui->label_deleteDataStatus->setText("删除失败！");
    }
}

void DataBase_TestTool::on_btn_update_clicked()
{
    qDebug() << __LINE__ << __FUNCTION__ << "开始更新数据:" ;


    tagOrderParam tempParam;

    tempParam.strOrderID = ui->lineEdit_orderIdUpdate->text().toStdString();
    tempParam.strEntry = ui->lineEdit_entryUpdate->text().toStdString();
    tempParam.strMoney = ui->lineEdit_moneyUpdate->text().toStdString();
    tempParam.strPlate = ui->lineEdit_plateUpdate->text().toStdString();
    tempParam.strType = ui->comboBox_typeUpdate->currentText().toStdString();
    tempParam.strWeight = ui->lineEdit_weightUpdate->text().toStdString();

    bool ret = DB_updateData(tempParam.strOrderID, tempParam.strEntry, tempParam.strMoney,
                             tempParam.strPlate, tempParam.strType, tempParam.strWeight);

    if(ret)
    {
        qDebug() << "更新数据成功！";
        ui->label_updateDataStatus->setText("更新成功！");
        updateDataTabel();
    }
    else
    {
        qDebug() << "更新数据失败！";
        ui->label_updateDataStatus->setText("更新失败！");
    }
}

/*
(1)INSERT INTO orderData (orderId, entry, money, plate, type, weight) VALUES ("63945", "滘口", "99", "粤A777Q1", "客一", "1800");
(2)UPDATE orderData SET entry = "滘口", money = "99", plate = "粤A777Q1", type = "客一", weight = "客一" WHERE orderId="22834";
(3)DELETE FROM orderData WHERE orderId=1234567;
*/

void DataBase_TestTool::on_btn_queryExec_clicked()
{
    qDebug() << __LINE__ << __FUNCTION__ << "开始执行sql语句:" ;

    string sql = ui->textEdit_sql->toPlainText().toStdString();

    bool ret = DB_queryExec(sql);

    if(ret)
    {
        qDebug() << "执行sql语句成功！";
        ui->label_execSqlStatus->setText("执行成功！");
        updateDataTabel();
    }
    else
    {
        qDebug() << "执行sql语句失败！";
        ui->label_execSqlStatus->setText("执行失败！");
    }
}

void DataBase_TestTool::on_btn_addDataMore_clicked()
{
    ui->label_addStatusMore->setText("添加中...");
    bool ret = testAddData();

    if(ret)
        ui->label_addStatusMore->setText("添加成功");
    else
        ui->label_addStatusMore->setText("添加失败");

    //int count = ui->lineEdit_count->text().toInt();
    //m_pDataCenter->setCount(count);
    //m_pThread->start();
}

void DataBase_TestTool::on_btn_deleteAll_clicked()
{
    return;
    qDebug() << __LINE__ << __FUNCTION__ << "开始删除整个表的数据" ;


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection1");
    db.setDatabaseName(DB_NAME); // 数据库文件名

    if (!db.open())
    {
        qDebug() << "Failed to connect to database 1.";
        return;
    }

    QSqlQuery query(db);
    QString sql = "DELETE FROM orderData";
    if (!query.exec(sql))
    {
        qDebug() << "Failed to delete data from trawe.db.";
        return;
    }

    ui->label_deleteAllStatus->setText("删除成功");
    db.close();
}

void DataBase_TestTool::slotConnectStatus(bool flag)
{
    if(flag)
    {
        qDebug() << "连接数据库成功！";
        ui->label_connectStatus->setText("成功！");
    }
    else
    {
        qDebug() << "连接数据库失败！";
        ui->label_connectStatus->setText("失败！");
    }
}

void DataBase_TestTool::slotAddDataStatus(bool flag)
{
    Q_UNUSED(flag);
}

void DataBase_TestTool::slotAddMoreDataStatus(bool flag)
{
    if(flag)
    {
        ui->label_addStatusMore->setText("添加成功");
    }
    else
    {
        ui->label_addStatusMore->setText("添加失败");
    }
}

void DataBase_TestTool::on_btn_dbTotalCount_clicked()
{
    qDebug() << __LINE__ << __FUNCTION__ << "开始查询整个表的数据总数" ;

    int count = DB_totalCount();
    qDebug() << "整个表的数据总数:" << QString::number(count);
    ui->label_dbTotalCount->setText(QString::number(count));
}
