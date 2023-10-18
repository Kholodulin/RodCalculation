#include "rod.h"

Rod::Rod( size_t _segmentsNum, double _forceAtTheEnd )
{
  E = 21000000;
  totalRodExtensions = 0;
  segmentsNum = _segmentsNum;
  segments.resize( segmentsNum );
  forceAtTheEnd = _forceAtTheEnd;
  fillSegmentsСharacteristics();
}
void Rod::nCalculate()
{
  segments[0].N = -forceAtTheEnd * 1000;

  for ( size_t i = 1; i < segments.size(); ++i )
    segments[i].N = segments[i - 1].N - segments[i - 1].force * 1000;
}
void Rod::stressCalculate()
{
  for ( size_t i = 0; i < segments.size(); ++i )
    segments[i].stress = segments[i].N / segments[i].area * 10000;
}
void Rod::extensionCalculate()
{
  for ( size_t i = 0; i < segments.size(); ++i )
  {
    segments[i].extension = segments[i].N * segments[i].length / ( E * segments[i].area );
    totalRodExtensions += segments[i].extension;
  }
}

void Rod::fillSegmentsСharacteristics()
{
  for ( size_t i = 0; i < segmentsNum; ++i )
  {
    std::cout << "Segment " << i + 1 << ":\n";
    std::cout << "rod lenght (m): ";
    std::cin >> segments[i].length;
    std::cout << "rod area (cm2): ";
    std::cin >> segments[i].area;
    //std::cout << "rod moment of inertia (cm^4): ";
    //std::cin >> segments[i].momentOfInertia;
    std::cout << "applied force (kH): ";
    std::cin >> segments[i].force;
  }
}

void Rod::rodCalculate()
{
  nCalculate();
  stressCalculate();
  extensionCalculate();
}

void Rod::setForceAtTheEnd( double f )
{
  forceAtTheEnd = f;
}

void Rod::printResult()
{
  std::cout << "longitudinal forces of the rod segment\n";

  for ( size_t i = 0; i < segments.size(); ++i )
    std::cout << "N" << i + 1 << " = " << segments[i].N << ";   ";

  std::cout << "\nStress in the rod segment\n";

  for ( size_t i = 0; i < segments.size(); ++i )
    std::cout << "s" << i + 1 << " = " << segments[i].stress << ";   ";

  std::cout << "\nExtensions of rod sections\n";

  for ( size_t i = 0; i < segments.size(); ++i )
    std::cout << "dl" << i + 1 << " = " << segments[i].extension << ";   ";

  std::cout << "\ntotal Extensions of rod - " << totalRodExtensions << std::endl;

}

double Rod::circleAreaCalculate( double D )
{
  return std::acos(-1) * D * D / 4;
}

double Rod::circleMomentOfInertiaCalculate( double D, double d )
{
  return std::acos(-1) / 32 * ( pow( D, 4 ) - pow( d, 4 ) );
}

double Rod::rectangleAreaCalculate( double width, double height )
{
  return width * height;
}

double Rod::rectangleMomentOfInertiaCalculate( double width, double height )
{
  return 1.0 / 12.0 * width * pow( height, 3 );
}
