#include <iostream>
#include <vector>
#include <ios>
#include <limits>

using std::cin;
using std::cout;

struct RodSegment {
    double length;
    double area;
    double force;
    double N;                               //Продольная сила на участке
    int64_t stress;                         //Напряжения в стержне
    double extension;			    //Удлинение участка
};

int main()
{
    int E = 21000000;                      //Модуль упругости * 10кПА
    size_t segmentsNum;
    double forceAtTheEnd;
    double totalRodExtensions = 0;
    cout << "Enter the number of rod segments ";
    cin >> segmentsNum;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<RodSegment> segments(segmentsNum);
    for (size_t i = 0; i < segmentsNum; ++i) {
        cout << "Segment " << i + 1 << ":\n";
        cout << "rod lenght (m): ";
        cin >> segments[i].length;
        cout << "rod area (cm2): ";
        cin >> segments[i].area;
        cout << "applied force (kH): ";
        cin >> segments[i].force;
    }
    cout << "Force at the end of the rod: ";
    cin >> forceAtTheEnd;

    segments[0].N = -forceAtTheEnd*1000;
    for (size_t i = 0; i< segments.size(); ++i){
        if(i!=0){
            segments[i].N = segments[i-1].N - segments[i-1].force * 1000;
        }
        segments[i].stress = segments[i].N / segments[i].area * 10000;
        segments[i].extension = segments[i].N * segments[i].length/(E*segments[i].area);
        totalRodExtensions+=segments[i].extension;
    }
    cout << "longitudinal forces of the rod segment\n";
    for (size_t i=0; i<segments.size();++i){
        cout << "N"<<i+1<< " = "<<segments[i].N <<";   ";
    }
    cout << "\nStress in the rod segment\n";
    for (size_t i=0; i<segments.size();++i){
        cout << "s"<<i+1<< " = "<<segments[i].stress<<";   ";
    }
    cout << "\nExtensions of rod sections\n";
    for (size_t i=0; i<segments.size();++i){
        cout << "dl"<<i+1<< " = "<<segments[i].extension<<";   ";
    }

    cout << "\ntotal Extensions of rod - " << totalRodExtensions << std::endl;

    system("pause");
    return 0;
}
