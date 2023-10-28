#include "rod.h"
#include "qdebug.h"
#include <cmath>

Rod::Rod()
{
  E = 21000000;
  totalLenght = 0;
  totalRodExtensions = 0;
}

void Rod::addRoundSegment( double length, double radius, double force )
{
  ;
  double pi = std::acos( -1 );
  double area = pi * radius * radius;
  segments.push_back( RodSegment( length, area, force ) );
  segments.back().shape = SegmentShape::CIRCLE;
  segments.back().radius = radius;
  totalLenght += length;
}

void Rod::addRectangularSegment( double width, double height, double length, double force )
{
  segments.push_back( RodSegment( length, width * height, force ) );
  segments.back().shape = SegmentShape::RECTANGLE;
  segments.back().width = width;
  segments.back().height = height;
  totalLenght += length;
}

void Rod::addRandomShapedSegment( double area, double length, double force )
{
  segments.push_back( RodSegment( length, area, force ) );
  segments.back().shape = SegmentShape::OTHER;
  totalLenght += length;
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
  nCalculate();
  stressCalculate();
  extensionCalculate();
}

int Rod::getSegmentsCount()
{
  return segments.size();
}

double Rod::getSegmentLongitudinalForce( int segmentIndex ) const
{
  return segments[segmentIndex].longitudinalForce;
}

int64_t Rod::getSegmentStress( int segmentIndex ) const
{
  return segments[segmentIndex].stress;
}

double Rod::getSegmentExtension( int segmentIndex ) const
{
  return segments[segmentIndex].extension;
}

double Rod::getTotalExtension() const
{
  return totalRodExtensions;
}

void Rod::drawResult()
{
  int width = 300;
  int height = 600;
  int center = width / 2;
  int currentHeight = 50;
  QPen boldGrayPen( QColor( 120, 120, 120 ), 4 );
  QPen grayPen ( Qt::gray, 2 );

  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );
  QPainterPath arrow;
  QFont font( painter.font().family(), 12 );
  painter.setFont( font );

  painter.setPen( grayPen );

  for ( int i = 20; i <= width - 40; i += 20 )
    painter.drawLine( i, currentHeight, i + 20, 10 );

  painter.drawLine( width - 25, currentHeight, width - 5, currentHeight );
  painter.drawLine( width - 100, height - currentHeight, width - 5, height - currentHeight );
  painter.drawLine( width - 10, currentHeight, width - 10, height - currentHeight );
  painter.drawLine( width - 10, currentHeight, width - 13,
                    currentHeight + 15 ); //стрелки серые на краях
  painter.drawLine( width - 10, currentHeight, width - 7, currentHeight + 15 );
  painter.drawLine( width - 10, height - currentHeight, width - 13, height - currentHeight - 15 );
  painter.drawLine( width - 10, height - currentHeight, width - 7, height - currentHeight - 15 );
  int d; //направление силы

  if ( forceAtTheEnd != 0 ) //синяя стрелка для forceAtTheEnd
  {
    forceAtTheEnd > 0 ? d = -1 : d = 1;
    arrow.moveTo( center, height - currentHeight );
    arrow.lineTo( center - 6, height - currentHeight );
    arrow.lineTo( center - 6, height - currentHeight + ( d * 20 ) );
    arrow.lineTo( center - 10, height - currentHeight + ( d * 20 ) );
    arrow.lineTo( center, height - currentHeight + ( d * 30 ) );
    arrow.lineTo( center + 10, height - currentHeight + ( d * 20 ) );
    arrow.lineTo( center + 6, height - currentHeight + ( d * 20 ) );
    arrow.lineTo( center + 6, height - currentHeight );
    arrow.lineTo( center, height - currentHeight );
    painter.drawText( center + 10, height - currentHeight + ( d * 20 ), "F=" + QString::number( forceAtTheEnd ) + "кН" );
  }

  painter.setPen( QPen( Qt::black, 5 ) );
  painter.drawLine( 20, currentHeight, width - 25, currentHeight );
  painter.setPen( grayPen );
  painter.setBrush( QColor( 199, 199, 199, 99 ) );
  int segmentRectHeight;
  double segmentRectWidth;
  double temp = 0; //самый широкий сегмент

  for ( const auto& segment : segments )
    temp = ( segment.area > temp ) ? segment.area : temp;

  for ( int i = 0; i < segments.size(); ++i )
  {
    if ( segments[i].force != 0 ) //стрелки синие для segments[].force
    {
      segments[i].force > 0 ? d = -1 : d = 1;
      arrow.moveTo( center, currentHeight );
      arrow.lineTo( center - 6, currentHeight );
      arrow.lineTo( center - 6, currentHeight + ( d * 20 ) );
      arrow.lineTo( center - 10, currentHeight + ( d * 20 ) );
      arrow.lineTo( center, currentHeight + ( d * 30 ) );
      arrow.lineTo( center + 10, currentHeight + ( d * 20 ) );
      arrow.lineTo( center + 6, currentHeight + ( d * 20 ) );
      arrow.lineTo( center + 6, currentHeight );
      arrow.lineTo( center, currentHeight );
      painter.drawText( center + 10, currentHeight + ( d * 20 ), "F=" + QString::number( segments[i].force ) + "кН" );
    }

    segmentRectHeight = static_cast<int>( segments[i].length / totalLenght * ( height - 100 ) );
    segmentRectWidth = static_cast<int>( segments[i].area / temp * ( width - 120 ) );
    painter.setPen( boldGrayPen ); //прямоугольник сегмента
    painter.drawRect( QRect( center - segmentRectWidth / 2, currentHeight, segmentRectWidth, segmentRectHeight ) );
    currentHeight += segmentRectHeight;
    painter.setPen( grayPen );

    if ( i + 1 < segments.size() ) //серые стрелки внутренние
    {
      painter.drawLine( width - 10, currentHeight, width - 13, currentHeight + 15 );
      painter.drawLine( width - 10, currentHeight, width - 7, currentHeight + 15 );
      painter.drawLine( width - 10, currentHeight, width - 13, currentHeight - 15 );
      painter.drawLine( width - 10, currentHeight, width - 7, currentHeight - 15 );
    }

    painter.drawLine( center + segmentRectWidth / 2, currentHeight, width - 5, currentHeight );
    painter.save(); // подпись длины сегмента
    painter.translate( width - 15, currentHeight - segmentRectHeight / 2 );
    painter.rotate( -90 );
    painter.drawText( 0, 0, QString::number( segments[i].length ) );
    painter.restore();
  }

  painter.setPen( Qt::black );
  painter.setBrush( Qt::blue );
  painter.drawPath( arrow );
  painter.setBrush( Qt::NoBrush );

  painter.end();

  image.save( "results.png" );
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
