#ifndef ROD_H
#define ROD_H
#include <iostream>
#include <vector>
#include <cmath>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QFont>
#include <QPainterPath>

class Rod
{
public:
  explicit Rod();
  void addRoundSegment( double radius, double length, double force );
  void addRectangularSegment( double width, double height, double length, double force );
  void addRandomShapedSegment( double area, double length, double force );
  void deleteSegment( size_t index );
  double getSegmentLongitudinalForce( int segmentIndex ) const;
  void setForceAtTheEnd( double f );
  void rodCalculate();
  int getSegmentsCount();
  int64_t getSegmentStress( int segmentIndex ) const;
  double getSegmentExtension( int segmentIndex ) const;
  double getTotalExtension() const;
  void drawResult();
private:
  void nCalculate();
  void stressCalculate();
  void extensionCalculate();
private:
  enum class SegmentShape
  {
    CIRCLE,
    RECTANGLE,
    OTHER
  };
  struct RodSegment
  {
    RodSegment( double len, double a, double f )
      : length( len ), area( a ), force( f ) {}
    double length;
    double area;
    double force;
    SegmentShape shape;
    double longitudinalForce; //Продольная сила на участке
    int64_t stress; //Напряжения участка
    double extension; //Удлинение участка
    double radius = 0;
    double width = 0;
    double height = 0;
  };
  double forceAtTheEnd;
  double totalLenght;
  double totalRodExtensions;
  int E; //Модуль упругости * 10кПА
  std::vector<RodSegment> segments;
};

#endif // ROD_H
