#ifndef ROD_H
#define ROD_H
#include <iostream>
#include <vector>
#include <cmath>

class Rod
{
public:
  explicit Rod( size_t _segmentsNum, double _forceAtTheEnd );
  void fillSegmentsСharacteristics();
  void rodCalculate();
  void setForceAtTheEnd( double f );
  void printResult();
  double circleAreaCalculate( double D );
  double circleMomentOfInertiaCalculate( double D,
                                         double d = 0 ); //D-внешний,d-внутренний(для трубы)
  double rectangleAreaCalculate( double width, double height );
  double rectangleMomentOfInertiaCalculate( double width, double height );
private:
  void nCalculate();
  void stressCalculate();
  void extensionCalculate();
private:
  size_t segmentsNum;
  double forceAtTheEnd;
  double totalRodExtensions;
  int E; //Модуль упругости * 10кПА
  enum class SegmentShape
  {
    CIRCLE,
    RECTANGLE,
    OTHER
  };
  class RodSegment
  {
  public:
    SegmentShape shape; //форма
    double length;
    double area;
    double force;
    double N; //Продольная сила на участке
    int64_t stress; //Напряжения в стержне
    double extension; //Удлинение участка
    double momentOfInertia; // Момент инерции
  };
  std::vector<RodSegment> segments;
};

#endif // ROD_H
