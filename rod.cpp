#include "rod.h"
#include "qdebug.h"
#include <cmath>
#include <memory>
#include <vector>

Rod::Rod()
{
  E = 21000000;
  totalLenght = 0;
  totalRodExtensions = 0;
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
void Rod::drawResult(){
  drawRod();
  drawLongitudinalForce();
  drawStress();
  drawExtension();
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

void Rod::drawRod()
{
  int width = 300;
  int height = 600;
  int center = width / 2;
  int currentHeight = 50;
  QPen boldGrayPen( QColor( 140, 140, 140 ), 4 );
  QPen grayPen ( QColor( 90, 90, 90 ), 2 );

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
  for ( const auto& segment : segments ){
    if(segment->area>0){
      temp = ( segment->area > temp ) ? segment->area : temp;
    }
    else{
      temp = ( segment->area*-1 > temp ) ? segment->area*-1 : temp;
    }
  }
  for ( int i = segments.size()-1; i >= 0; --i )
  {
    if ( segments[i]->force != 0 ) //стрелки синие для segments[].force
    {
        segments[i]->force > 0 ? d = -1 : d = 1;
      arrow.moveTo( center, currentHeight );
      arrow.lineTo( center - 6, currentHeight );
      arrow.lineTo( center - 6, currentHeight + ( d * 20 ) );
      arrow.lineTo( center - 10, currentHeight + ( d * 20 ) );
      arrow.lineTo( center, currentHeight + ( d * 30 ) );
      arrow.lineTo( center + 10, currentHeight + ( d * 20 ) );
      arrow.lineTo( center + 6, currentHeight + ( d * 20 ) );
      arrow.lineTo( center + 6, currentHeight );
      arrow.lineTo( center, currentHeight );
      painter.drawText( center + 10, currentHeight + ( d * 20 ), "F=" + QString::number( segments[i]->force ) + "кН" );
    }

    segmentRectHeight = static_cast<int>( segments[i]->length / totalLenght * ( height - 100 ) );
    segmentRectWidth = static_cast<int>( segments[i]->area / temp * ( width - 120 ) );
    painter.setPen( boldGrayPen ); //прямоугольник сегмента
    painter.drawRect( QRect( center - segmentRectWidth / 2, currentHeight, segmentRectWidth, segmentRectHeight ) );
    currentHeight += segmentRectHeight;
    painter.setPen( grayPen );

    if ( i != 0 ) //серые стрелки внутренние
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
    painter.drawText( 0, 0, QString::number( segments[i]->length ) );
    painter.restore();
  }

  painter.setPen( Qt::black );
  painter.setBrush( Qt::blue );
  painter.drawPath( arrow );
  painter.setBrush( Qt::NoBrush );

  painter.end();

  image.save( "image1.png" );
}

void Rod::drawLongitudinalForce()
{
  int width = 300;
  int height = 600;
  int center = width / 2;
  int currentHeight = 50;
  QPainterPath path;
  QPen boldGrayPen( QColor( 120, 120, 120 ), 4 );
  QPen grayPen ( QColor( 90, 90, 90 ), 2 );

  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );

  int segmentRectHeight;
  double segmentRectWidth;
  double temp = 0; //самый широкий сегмент
  for ( const auto& segment : segments ){
    if(segment->longitudinalForce>0){
      temp = ( segment->longitudinalForce > temp ) ? segment->longitudinalForce : temp;
    }
    else{
      temp = ( segment->longitudinalForce*-1 > temp ) ? segment->longitudinalForce*-1 : temp;
    }
  }
  path.moveTo(center,currentHeight);
  for ( int i = segments.size()-1; i >= 0; --i )
  {
    segmentRectHeight = static_cast<int>( segments[i]->length / totalLenght * ( height - 100 ) );
    segmentRectWidth = static_cast<int>( segments[i]->longitudinalForce / temp * ( center - 60 ) );
    painter.setPen( grayPen );
    path.lineTo(center+segmentRectWidth, currentHeight);
    segments[i]->longitudinalForce > 0 ?
        painter.drawText( center+segmentRectWidth*1.1, currentHeight+10, QString::number( segments[i]->longitudinalForce/1000 ) )
                                       : painter.drawText( center+segmentRectWidth*1.4, currentHeight+10, QString::number( segments[i]->longitudinalForce/1000 ) );
    currentHeight += segmentRectHeight;
    path.lineTo(center+segmentRectWidth, currentHeight);
    segments[i]->longitudinalForce > 0 ?
        painter.drawText( center+segmentRectWidth*1.1, currentHeight-5, QString::number( segments[i]->longitudinalForce/1000 ) )
                                       : painter.drawText( center+segmentRectWidth*1.4, currentHeight-5, QString::number( segments[i]->longitudinalForce/1000 ) );
  }
  path.lineTo(center,currentHeight);
  painter.setPen( boldGrayPen );
  path.lineTo(center,50);
  painter.setPen( boldGrayPen );
  painter.setBrush(Qt::HorPattern);
  painter.drawPath(path);
  painter.setPen( QPen( QColor(0,120,0), 6 ) );
  painter.drawLine( center, 50, center, height-50 );
  painter.end();
  image.save( "image2.png" );
}
void Rod::drawStress()
{
  int width = 300;
  int height = 600;
  int center = width / 2;
  int currentHeight = 50;
  QPainterPath path;
  QPen boldGrayPen( QColor( 120, 120, 120 ), 4 );
  QPen grayPen ( QColor( 90, 90, 90 ), 2 );

  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );

  int segmentRectHeight;
  double segmentRectWidth;
  double stress;
  double temp = 0; //самый широкий сегмент
  for ( const auto& segment : segments ){
    stress = static_cast<double>(segment->stress/1000);
    if(segment->stress>0){
      temp = ( stress > temp ) ? stress : temp;
    }
    else{
      temp = ( stress > temp ) ? stress*-1 : temp;
    }
  }
  path.moveTo(center,currentHeight);
  for ( int i = segments.size()-1; i >= 0; --i )
  {
    stress = static_cast<double>(segments[i]->stress/1000);
    segmentRectHeight = static_cast<int>( segments[i]->length / totalLenght * ( height - 100 ) );
    segmentRectWidth = stress / temp * ( center - 60.0 );
    path.lineTo(center+segmentRectWidth, currentHeight);
    stress > 0 ? painter.drawText( center+segmentRectWidth*1.1, currentHeight+10, QString::number( segments[i]->stress/1000000.0 ) )
                 :painter.drawText( center+segmentRectWidth*1.5, currentHeight+10, QString::number( segments[i]->stress/1000000.0 ) );
    currentHeight += segmentRectHeight;
    path.lineTo(center+segmentRectWidth, currentHeight);
    stress > 0 ? painter.drawText( center+segmentRectWidth*1.1, currentHeight-5, QString::number( segments[i]->stress/1000000.0 ) )
               : painter.drawText( center+segmentRectWidth*1.5, currentHeight-5, QString::number( segments[i]->stress/1000000.0 ) );
  }
  path.lineTo(center,currentHeight);
  path.lineTo(center,50);
  painter.setPen( boldGrayPen );
  painter.setBrush(Qt::HorPattern);
  painter.drawPath(path);
  painter.setPen( QPen( QColor(0,120,0), 6 ) );
  painter.drawLine( center, 50, center, height-50 );
  painter.end();
  image.save( "image3.png" );
}

void Rod::drawExtension()
{
  int width = 300;
  int height = 600;
  int center = width / 2;
  int currentHeight = 50;
  double extension;
  QPainterPath path;
  QPen boldGrayPen( QColor( 120, 120, 120 ), 4 );
  QPen grayPen ( QColor( 90, 90, 90 ), 2 );

  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );

  int segmentRectHeight;
  double segmentRectWidth;
  double temp = 0; //самый широкий сегмент
  for ( const auto& segment : segments ){
    if(segment->stress>0){
      temp = ( segment->extension > temp ) ? segment->extension : temp;
    }
    else{
      temp = ( segment->extension > temp ) ? segment->extension*-1 : temp;
    }
  }
  path.moveTo(center,currentHeight);
  extension = segments.back()->extension;
  for ( int i = segments.size()-1; i >= 0; --i )
  {
    if(i != segments.size()-1){
      extension+=segments[i]->extension;
    }
    segmentRectHeight = static_cast<int>( segments[i]->length / totalLenght * ( height - 100 ) );
    segmentRectWidth = extension / temp * ( center - 60.0 );
    currentHeight += segmentRectHeight;
    painter.setPen( grayPen );
    path.lineTo(center+segmentRectWidth, currentHeight);
    QString formattedNumber = QString::number(extension, 'f', 6);
    extension > 0 ? painter.drawText( center+segmentRectWidth*0.95, currentHeight-5, formattedNumber )
               :painter.drawText( center+segmentRectWidth*2, currentHeight-5, formattedNumber );
  }
  path.lineTo(center,currentHeight);
  path.lineTo(center,50);
  painter.setPen( boldGrayPen );
  painter.setBrush(Qt::HorPattern);
  painter.drawPath(path);
  painter.setPen( QPen( QColor(0,120,0), 6 ) );
  painter.drawLine( center, 50, center, height-50 );
  painter.end();
  image.save( "image4.png" );
}

