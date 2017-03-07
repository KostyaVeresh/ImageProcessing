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
	//QImage source("/home/kveresh/prog/ImageProcessing/HollywoodLC.jpg");
	QImage source("C:\\Users\\kveresh\\Documents\\prog\\ImageProcessing\\HollywoodLC.jpg");
	QImage binaryImage = ImageProc::threshold(source, 128);
	QImage negativeImage = ImageProc::negative(source);
	QImage powerImage = ImageProc::powerTransform(source, 1.7);
	QImage logImage = ImageProc::logTransform(source);
    QImage equalisedImage = ImageProc::equalisationHist(source);
	QImage NNScaledImage = ImageProc::nearestNeighbourScale(source, 1.75);
	QImage BiScaledImage = ImageProc::bilinearScale(source, 1.75);

	ImageProc::displayImage(source, ui->sourceImage);
	ImageProc::displayImage(binaryImage, ui->binaryImage);
	ImageProc::displayImage(negativeImage, ui->negativeImage);
	ImageProc::displayImage(powerImage, ui->powerImage);
	ImageProc::displayImage(logImage, ui->logImage);
    ImageProc::displayImage(equalisedImage, ui->equalisedImage);
	ImageProc::displayImage(NNScaledImage, ui->NNScaledImage);
	ImageProc::displayImage(BiScaledImage, ui->BiScaledImage);
}

MainWindow::~MainWindow()
{
	delete ui;
}
