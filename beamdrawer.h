#ifndef BEAMDRAWER_H
#define BEAMDRAWER_H

#include <QImage>
#include <QPainter>
#include <QRect>
#include <QFont>
#include <QPainterPath>
#include <QPen>
#include "rod.h"

class BeamDrawer
{
private:
    int width;
    int height;
    int currentHeight;
    int center;
    QPen grayPen;
    QPen boldGrayPen;
    QPen greenPen;
    int segmentRectHeight;
    double segmentRectWidth;
    double widestSegment;
public:
    explicit BeamDrawer(const Rod& rod);
    void drawResult();
private:
    const Rod& rod;
    void drawRod();
    void drawLongitudinalForce();
    void drawStress();
    void drawExtension();
    void drawBeam(QPainter& painter, int segmentIndex);
    void drawDimensionArrow(QPainter& painter, int x, int y, bool isUpward);
    void drawForceArrow(QPainterPath& arrow, int x, int y, int d);
};

#endif // BEAMDRAWER_H
