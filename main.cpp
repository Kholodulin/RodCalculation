#include <iostream>
#include <vector>
#include <ios>
#include <limits>
#include "rod.h"

using std::cin;
using std::cout;

int main()
{
  size_t segmentsNum;
  double f;
  cout << "Enter the number of rod segments ";
  cin >> segmentsNum;
  cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
  cout << "Force at the end of the rod: ";
  cin >> f;
  Rod rod( segmentsNum, f );
  rod.rodCalculate();
  rod.printResult();

  system( "pause" );
  return 0;
}
