#include <iostream>
#include <vector>
#include <ios>
#include <limits>
#include "rod.h"

int main()
{
    Rod rod;
    rod.addRandomShapedSegment(0.3, 0.8, 20);
    rod.addRandomShapedSegment(0.8, 2, 50);
    rod.addRandomShapedSegment(0.5, 1.2, 0);
    rod.setForceAtTheEnd(-30);

    rod.rodCalculate();

    for(int i=0;i<rod.getSegmentsCount();++i ){
        std::cout << "Segment" << i+1 << std::endl;
        std::cout << "longitudinal force = " << rod.getSegmentLongitudinalForce(i) << std::endl;
        std::cout << "Stress = " << rod.getSegmentStress(i) << std::endl;
        std::cout << "Extensions = " << rod.getSegmentExtension(i) << std::endl<< std::endl;
    }
    std::cout << "total Extensions of rod - " << rod.getTotalExtension() << std::endl;
  system( "pause" );
  return 0;
}
