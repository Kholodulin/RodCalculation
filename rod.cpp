#include "rod.h"
#include <cmath>

Rod::Rod()
{
  E = 21000000;
  totalRodExtensions = 0;
}

void Rod::addRoundSegment(double length, double radius, double force)
{;
  double pi = std::acos(-1);
  double area =  pi * radius * radius;
  segments.push_back(RodSegment(length,area,force));
}

void Rod::addRectangularSegment(double width, double height, double length, double force)
{
  segments.push_back(RodSegment(length, width * height, force));
}

void Rod::addRandomShapedSegment(double area, double length,  double force)
{
  segments.push_back(RodSegment(length,area,force));
}

void Rod::deleteSegment(size_t index)
{
  segments.erase(segments.begin()+index);
}


void Rod::setForceAtTheEnd( double f )
{
  forceAtTheEnd = f;
}

void Rod::rodCalculate()
{
  nCalculate();
  stressCalculate();
  extensionCalculate();
}

int Rod::getSegmentsCount()
{
  return segments.size();
}

double Rod::getSegmentLongitudinalForce(int segmentIndex) const
{
  return segments[segmentIndex].longitudinalForce;
}

int64_t Rod::getSegmentStress(int segmentIndex) const
{
  return segments[segmentIndex].stress;
}

double Rod::getSegmentExtension(int segmentIndex) const
{
  return segments[segmentIndex].extension;
}

double Rod::getTotalExtension() const
{
  return totalRodExtensions;
}
void Rod::nCalculate()
{
  segments[0].longitudinalForce = -forceAtTheEnd * 1000;

  for ( size_t i = 1; i < segments.size(); ++i )
    segments[i].longitudinalForce = segments[i - 1].longitudinalForce - segments[i - 1].force * 1000;
}
void Rod::stressCalculate()
{
  for ( size_t i = 0; i < segments.size(); ++i )
    segments[i].stress = segments[i].longitudinalForce / segments[i].area * 10000;
}
void Rod::extensionCalculate()
{
  for ( size_t i = 0; i < segments.size(); ++i )
  {
    segments[i].extension = segments[i].longitudinalForce * segments[i].length / ( E * segments[i].area );
    totalRodExtensions += segments[i].extension;
  }
}
