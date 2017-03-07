#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QGraphicsView>
#include <QImage>
#include <QVector>

namespace ImageProc
{
QVector<double> histohram(const QImage &scr);
QImage nearestNeighbourScale(const QImage &scr, double scale);
QImage bilinearScale(const QImage &scr, double scale);
QImage threshold(const QImage &scr, int thresh);
QImage negative(const QImage &scr);
QImage powerTransform(const QImage &scr, double power);
QImage logTransform(const QImage &scr);
QImage equalisationHist(const QImage &scr);
QImage castHist(const QImage &scr);
void displayImage(const QImage &scr, QGraphicsView *view);
}

#endif // IMAGEPROC_H
