#include "rod.h"
#include <cmath>
#include <memory>
#include <vector>
#include <QTransform>

Rod::Rod() : totalLenght(0), totalRodExtensions(0), E(21000000)
{
  segments = std::vector<std::shared_ptr<Segment>>();
}

void Rod::addRoundSegment(double radius, double length, double force)
{
  std::shared_ptr<Segment> circularSegment = std::make_shared<CircularSegment>(radius, length, force);
  addSegment(circularSegment);
}

void Rod::addRectangularSegment( double width, double height, double length, double force )
{
  std::shared_ptr<Segment> rectangularSegment = std::make_shared<RectangularSegment>(width, height, length, force);
  addSegment(rectangularSegment);
}

void Rod::addRandomSegment(double area, double length, double force)
{
  std::shared_ptr<Segment> randomSegment = std::make_shared<RandomSegment>(area, length, force);
  addSegment(randomSegment);
}

void Rod::deleteSegment( size_t index )
{
  segments.erase( segments.begin() + index );
}

void Rod::setForceAtTheEnd( double f )
{
  forceAtTheEnd = f;
}

void Rod::rodCalculate()
{
  calculateLongitudinalForce();
  calculateStress();
  calculateSxtension();
}

size_t Rod::getSegmentsCount()
{
  return segments.size();
}

double Rod::getSegmentLongitudinalForce( int segmentIndex ) const
{
  return segments[segmentIndex]->longitudinalForce;
}

int64_t Rod::getSegmentStress( int segmentIndex ) const
{
  return segments[segmentIndex]->stress;
}

double Rod::getSegmentExtension( int segmentIndex ) const
{
  return segments[segmentIndex]->extension;
}

double Rod::getTotalExtension() const
{
  return totalRodExtensions;
}

void Rod::calculateLongitudinalForce()
{
  segments[0]->longitudinalForce = -forceAtTheEnd * 1000;

  for ( size_t i = 1; i < segments.size(); ++i )
    segments[i]->longitudinalForce = segments[i - 1]->longitudinalForce - segments[i - 1]->force * 1000;
}
void Rod::calculateStress()
{
  for ( size_t i = 0; i < segments.size(); ++i )
    segments[i]->stress = segments[i]->longitudinalForce / segments[i]->area * 10000;
}
void Rod::calculateSxtension()
{
  for ( size_t i = 0; i < segments.size(); ++i )
  {
    segments[i]->extension = segments[i]->longitudinalForce * segments[i]->length / ( E * segments[i]->area );
    totalRodExtensions += segments[i]->extension;
  }
}

void Rod::addSegment(std::shared_ptr<Segment> segment)
{
  segment->area = segment->getArea();
  segments.push_back(segment);
  totalLenght += segment->length;
}




