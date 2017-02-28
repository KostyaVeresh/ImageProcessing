#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageProc.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    QImage source("/home/kveresh/prog/ImageProcessing/HollywoodLC.jpg");
	QImage binaryImage = ImageProc::threshold(source, 128);
	QImage negativeImage = ImageProc::negative(source);
    QImage equalisedImage = ImageProc::equalisationHist(source);

	ImageProc::displayImage(source, ui->sourceImage);
	ImageProc::displayImage(binaryImage, ui->binaryImage);
	ImageProc::displayImage(negativeImage, ui->negativeImage);
    ImageProc::displayImage(equalisedImage, ui->equalisedImage);
}

MainWindow::~MainWindow()
{
	delete ui;
}
