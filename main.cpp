#include <iostream>
#include <vector>
#include <ios>
#include <limits>
#include <QtGui>

#include "rod.h"
#include "beamdrawer.h"

int main( int argc, char* argv[] )
{
  QGuiApplication app( argc, argv );
  Rod rod;
  BeamDrawer beamDrawer(rod);
  rod.addRandomSegment( 0.5, 1.2, 0 );
  rod.addRandomSegment( 0.8, 2, 50 );
  rod.addRandomSegment( 0.3, 0.8, 20 );
  rod.setForceAtTheEnd( -30 );
  rod.rodCalculate();
  for(int i=0;i<rod.getSegmentsCount();++i ){
      std::cout << "Segment" << i+1 << std::endl;
      std::cout << "longitudinal force = " << rod.getSegmentLongitudinalForce(i) << std::endl;
      std::cout << "Stress = " << rod.getSegmentStress(i) << std::endl;
      std::cout << "Extensions = " << rod.getSegmentExtension(i) << std::endl<< std::endl;
  }
  std::cout << "total Extensions of test rod = " << rod.getTotalExtension() << std::endl;

  beamDrawer.drawResult();
  return app.exec();
}
