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
#include <memory>

class BeamDrawer;

class Rod
{
private:
    class Segment;
public:
    explicit Rod();
    void addRoundSegment(double radius, double length, double force );
    void addRectangularSegment( double width, double height, double length, double force );
    void addRandomSegment( double area, double length, double force );
    void deleteSegment( size_t index );
    double getSegmentLongitudinalForce( int segmentIndex ) const;
    void setForceAtTheEnd( double f );
    void rodCalculate();
    size_t getSegmentsCount();
    int64_t getSegmentStress( int segmentIndex ) const;
    double getSegmentExtension( int segmentIndex ) const;
    double getTotalExtension() const;
private:
    void calculateLongitudinalForce();
    void calculateStress();
    void calculateSxtension();
    void addSegment(std::shared_ptr<Segment> segment);
    friend class BeamDrawer;

private:
    enum class SegmentShape
    {
        CIRCLE,
        RECTANGLE,
        OTHER
    };
    class Segment
    {
    public:
        Segment( double len, double f )
            : length( len ), force( f ) {}
        double length;
        double area;
        double force;
        double longitudinalForce; //Продольная сила на участке
        int64_t stress; //Напряжения участка
        double extension; //Удлинение участка
        virtual double getArea() const = 0;
        virtual SegmentShape getShape() const = 0;
    };

    class CircularSegment : public Segment
    {
    public:
        CircularSegment(double radius, double length, double force)
            : Segment(length, force), radius(radius) {}
        double radius;
        double getArea() const override { return M_PI * radius * radius; }
        SegmentShape getShape() const override { return SegmentShape::CIRCLE; }
    };

    class RectangularSegment : public Segment
    {
    public:
        RectangularSegment(double width, double height, double length, double force)
            : Segment(length, force), width(width), height(height) {}
        double width;
        double height;
        double getArea() const override { return width * height; }
        SegmentShape getShape() const override { return SegmentShape::RECTANGLE; }
    };

    class RandomSegment : public Segment
    {
    public:
        RandomSegment(double area, double length, double force)
            : Segment(length, force), area(area) {}
        double area;
        double getArea() const override { return area; }
        SegmentShape getShape() const override { return SegmentShape::OTHER; }
    };
    double forceAtTheEnd;
    double totalLenght;
    double totalRodExtensions;
    int E; //Модуль упругости * 10кПА
    std::vector<std::shared_ptr<Segment>> segments;

};

#endif // ROD_H
