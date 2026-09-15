#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QFileSystemModel(this);
    model->setRootPath(QDir::homePath() + "/Downloads");
    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setUniformItemSizes(true);
    ui->listView->setGridSize(QSize(120, 100));
    ui->listView->setMovement(QListView::Static);
    ui->listView->setResizeMode(QListView::Adjust);
    ui->listView->setWrapping(true);
    ui->listView->setLayoutMode(QListView::Batched);
    ui->listView->setModel(model);
    ui->listView->setRootIndex(model->index(QDir::homePath() + "/Downloads"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
