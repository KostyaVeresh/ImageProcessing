#include "imageProc.h"

#include <QDebug>
#include <QtMath>

namespace ImageProc
{
QImage nearestNeighbourScale(const QImage &scr, double scale) {
	QImage dst(qRound((scr.width()) * scale), qRound((scr.height()) * scale), scr.format());
	for (int i = 0; i < dst.width(); ++i) {
		for (int j = 0; j < dst.height(); ++j) {
			int nearestI = int((i + 0.5) / scale);
			int nearestJ = int((j + 0.5) / scale);
			int color = qRed(scr.pixel(nearestI, nearestJ));
			dst.setPixel(i, j, qRgb(color, color, color));
		}
	}
	return dst;
}

QImage bilinearScale(const QImage &scr, double scale) {
	QImage frameScr(scr.width() + 2, scr.height() + 2, scr.format());
	for (int i = 1; i < frameScr.width() - 1; ++i) {
		for (int j = 1; j < frameScr.height() - 1; ++j) {
			int color = qRed(scr.pixel(i - 1, j - 1));
			frameScr.setPixel(i, j, qRgb(color, color, color));
		}
	}
	for (int i = 1; i < frameScr.width() - 1; ++i) {
		int color = qRed(scr.pixel(i - 1, 0));
		frameScr.setPixel(i, 0, qRgb(color, color, color));
		color = qRed(scr.pixel(i - 1, scr.height() - 1));
		frameScr.setPixel(i, frameScr.height() - 1, qRgb(color, color, color));

	}
	for (int i = 1; i < frameScr.height() - 1; ++i) {
		int color = qRed(scr.pixel(0, i - 1));
		frameScr.setPixel(0, i, qRgb(color, color, color));
		color = qRed(scr.pixel(scr.width() - 1, i - 1));
		frameScr.setPixel(frameScr.width() - 1, i, qRgb(color, color, color));

	}
	QImage dst(qRound(scr.width() * scale), qRound(scr.height() * scale), scr.format());
	for (int i = 0; i < dst.width(); ++i) {
		for (int j = 0; j < dst.height(); ++j) {
			double x = (i + 0.5) / scale + 0.5;
			double y = (j + 0.5) / scale + 0.5;
			double x1 = (int) x;
			double x2 = x1 + 1;
			double y1 = (int) y;
			double y2 = y1 + 1;
			int val_11 = qRed(frameScr.pixel(x1, y1));
			int val_12 = qRed(frameScr.pixel(x1, y2));
			int val_21 = qRed(frameScr.pixel(x2, y1));
			int val_22 = qRed(frameScr.pixel(x2, y2));
			double result = (val_11 * (x2 - x) * (y2 - y)
							 + val_21 * (x - x1) * (y2 - y)
							 + val_12 * (x2 - x) * (y - y1)
							 + val_22 * (x - x1) * (y - y1))
					/ ((x2 - x1) * (y2 - y1));
			int color = int(result);
			dst.setPixel(i, j, qRgb(color, color, color));
		}
	}
	return dst;
}

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

QImage powerTransform(const QImage &scr, double power) {
	QImage dst(scr.size(), scr.format());
	double coef = 255 / qPow(255, power);
	for (int i = 0; i < dst.width(); ++i) {
		for (int j = 0; j < dst.height(); ++j) {
			int color = coef * qPow(qRed(scr.pixel(i, j)), power);
			dst.setPixel(i, j, qRgb(color, color, color));
		}
	}
	return dst;
}

QImage logTransform(const QImage &scr) {
	QImage dst(scr.size(), scr.format());
	double coef = 255 / qLn(256);
	for (int i = 0; i < dst.width(); ++i) {
		for (int j = 0; j < dst.height(); ++j) {
			int color = coef * qLn(1 + qRed(scr.pixel(i, j)));
			dst.setPixel(i, j, qRgb(color, color, color));
		}
	}
	return dst;
}

void displayImage(const QImage &scr, QGraphicsView *view) {
	view->setFixedSize(scr.width() + 2, scr.height() + 2);
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

QImage castHist(const QImage &scr) {
	QImage dst(scr.size(), scr.format());
	QVector<double> distribution(256);
	QVector<double> hist = histohram(scr);

	return dst;
}
}
