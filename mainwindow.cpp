#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logviewer.h"
#include <QDebug>
#include <QListView>
#include <QStringListModel>
#include <QFileDialog>
#include <QAbstractItemView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timer(new QTimer(this))
{
    m_timer->setInterval(0);
    auto v1=QString(LVIEW_VERSION);
    auto v2=QString(GIT_VERSION);
    auto logan_version=v1+"-"+v2;

    ui->setupUi(this);

    m_timer_widget=new TimerForm();

    ui->mainToolBar->addWidget(m_timer_widget);

    setMouseTracking(true);
    setAutoFillBackground(true);

    m_controller=new Controller(this);
    setWindowTitle("Logan - "+logan_version);
    m_tabbar=new QTabWidget(ui->centralWidget);
    m_tabbar->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
    ui->gridLayout->addWidget(m_tabbar);

    m_autoscroll_enabled=ui->actionautoscroll_enabled->isChecked();
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFileSlot);
    connect(ui->actionreolad_current, &QAction::triggered, this, &MainWindow::reloadCurentSlot);
    connect(ui->actionclose_current_tab, &QAction::triggered, this, &MainWindow::closeCurentSlot);
    connect(ui->actionautoscroll_enabled, &QAction::triggered, this, &MainWindow::autoscrollChangedSlot);
    connect(m_timer_widget, &TimerForm::timerParamChangedSignal,this, &MainWindow::timerIntervalChangedSlot);
    connect(m_timer_widget, &TimerForm::timerIsEnabledSignal,this, &MainWindow::timerIntervalEnabledSlot);
    connect(m_timer, &QTimer::timeout,this, &MainWindow::reloadAllSlot);
    m_timer_widget->defaultState();
}

void MainWindow::openFileSlot(){
    qDebug()<<"openFileSlot()";

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (!dialog.exec()){
        return;
    }
    fileNames = dialog.selectedFiles();
    for(auto v: fileNames){
        auto log=m_controller->openFile(v);
        if(log==nullptr){
            continue;
        }
        auto lb=new LogViewer(m_tabbar);
        lb->setModel(log);
        lb->setAutoScroll(m_autoscroll_enabled);
        m_tabbar->addTab(lb, v);
    }
}

void MainWindow::reloadCurentSlot(){
    //TODO refact!
    qDebug()<<"reloadCurentSlot()";
    int current=m_tabbar->currentIndex();
    if(current<0){
        return;
    }
    auto widget=m_tabbar->widget(current);
    auto log=dynamic_cast<LogViewer*>(widget)->model();
    m_controller->updateAllSlot(log->filename());
}

void MainWindow::closeCurentSlot(){
    //TODO refact!
    qDebug()<<"closeCurentSlot()";
    int current=m_tabbar->currentIndex();
    if(current<0){
        return;
    }
    auto widget=m_tabbar->widget(current);
    auto log=dynamic_cast<LogViewer*>(widget)->model();
    auto fname=log->filename();
    m_tabbar->removeTab(current);
    m_controller->closeFileSlot(fname);
}

void MainWindow::autoscrollChangedSlot(){
    qDebug()<<"autoscrollChangedSlot()"<<ui->actionautoscroll_enabled->isChecked();
    m_autoscroll_enabled=ui->actionautoscroll_enabled->isChecked();

    for(int i=0;i<m_tabbar->count();++i){
        auto widget=m_tabbar->widget(i);
        auto log=dynamic_cast<LogViewer*>(widget);
        log->setAutoScroll(m_autoscroll_enabled);
    }
}

void MainWindow::timerIntervalChangedSlot(int v){
    qDebug()<<"MainWindow::timerIntervalChangedSlot()"<<v;
    m_timer->setInterval(v*1000);

    qDebug()<<"interval: "<<m_timer->interval()<<"isActive:"<<m_timer->isActive();
}

void MainWindow::reloadAllSlot(){
    qDebug()<<"MainWindow::reloadAllSlot()";
    m_controller->updateAllSlot("null");
}

void MainWindow::timerIntervalEnabledSlot(bool b){
    if(b){
        m_timer->start();
    }else{
        m_timer->stop();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
    delete m_controller;

}
