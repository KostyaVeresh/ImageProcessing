#include "imageProc.h"
#include <QDebug>

namespace ImageProc
{
QImage threshold(const QImage &scr, int thresh) {
    QImage dst(scr.size(), scr.format());
	for (int i = 0; i < dst.width(); ++i) {
		for (int j = 0; j < dst.height(); ++j) {
                        int index = qRed(scr.pixel(i, j));
			if (index < thresh) {
				dst.setPixel(i, j, qRgb(0, 0, 0));
			} else {
				dst.setPixel(i, j, qRgb(255, 255, 255));
			}
		}
	}
        return dst;
}

QImage negative(const QImage &scr) {
    QImage dst(scr.size(), scr.format());
    for (int i = 0; i < dst.width(); ++i) {
        for (int j = 0; j < dst.height(); ++j) {
            int inverted = 255 - qRed(scr.pixel(i, j));
            dst.setPixel(i, j, qRgb(inverted, inverted, inverted));
        }
    }
    return dst;
}

void displayImage(const QImage &scr, QGraphicsView *view) {
    view->setFixedSize(scr.width() + 4, scr.height() + 4);
    QGraphicsScene *scene = new QGraphicsScene(view);
    scene->addPixmap(QPixmap::fromImage(scr));
    scene->setSceneRect(scr.rect());
    view->setScene(scene);
}

QVector<double> histohram(const QImage &scr) {
    QVector<double> hist(256);
    for (int i = 0; i < scr.width(); ++i) {
        for (int j = 0; j < scr.height(); ++j) {
            hist[qRed(scr.pixel(i, j))] += 1;
        }
    }
    int numberOfPixels = scr.width() * scr.height();
    for (int i = 0; i < hist.size(); ++i) {
        hist[i] /= numberOfPixels;
    }
    return hist;
}

QImage equalisationHist(const QImage &scr) {
    QImage dst(scr.size(), scr.format());
    QVector<int> transform(256);
    QVector<double> hist = histohram(scr);
    double tmp = 0;
    for (int i = 0; i < hist.size(); ++i) {
        tmp += hist[i];
        transform[i] = int(255 * tmp);
    }
    for (int i = 0; i < scr.width(); ++i) {
        for (int j = 0; j < scr.height(); ++j) {
            int newColor = transform.at(qRed(scr.pixel(i, j)));
            dst.setPixel(i, j, qRgb(newColor, newColor, newColor));
        }
    }
    return dst;
}
}
