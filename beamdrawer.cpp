#include "beamdrawer.h"

BeamDrawer::BeamDrawer(const Rod &rod) : width(300), height(600), widestSegment(0), rod(rod)
{
    center = width / 2;
    grayPen = QPen(QColor( 90, 90, 90 ), 2) ;
    boldGrayPen = QPen(QColor( 120, 120, 120 ), 4);
    greenPen = QPen(QColor(0,120,0), 6);
}

void BeamDrawer::drawResult(){
    drawRod();
    drawLongitudinalForce();
    drawStress();
    drawExtension();
}

void BeamDrawer::drawRod()
{
  currentHeight = 50;
  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );
  QPainterPath arrow;
  painter.setFont( QFont( painter.font().family(), 12 ));
  painter.setPen( grayPen );

  for ( int i = 20; i <= width - 40; i += 20 ){
    painter.drawLine( i, currentHeight, i + 20, 10 );
  }
  painter.drawLine( center, currentHeight, width - 5, currentHeight );
  painter.drawLine( width - 10, currentHeight, width - 10, height - currentHeight );
  drawDimensionArrow(painter, width - 10, currentHeight,0);
  drawDimensionArrow(painter, width - 10, height - currentHeight, 1);

  int d; //направление силы
  if (rod.forceAtTheEnd)
  {
    rod.forceAtTheEnd > 0 ? d = -1 : d = 1;
    drawForceArrow( arrow, center, height - currentHeight, d);
    painter.drawText( center + 10, height - currentHeight + ( d * 20 ), "F=" + QString::number( rod.forceAtTheEnd ) + "кН" );
  }

  widestSegment = 0;
  for ( const auto& segment : rod.segments ){
    widestSegment = std::max(widestSegment, std::abs(segment->area));
  }
  painter.setBrush( QColor( 199, 199, 199, 99 ) );
  for ( int i = rod.segments.size()-1; i >= 0; --i )
  {
    if ( rod.segments[i]->force != 0 )
    {
      rod.segments[i]->force > 0 ? d = -1 : d = 1;
      drawForceArrow( arrow, center, currentHeight, d);
      painter.drawText( center + 10, currentHeight + ( d * 20 ), "F=" + QString::number( rod.segments[i]->force ) + "кН" );
    }
    segmentRectHeight = static_cast<int>( rod.segments[i]->length / rod.totalLenght * ( height - 100 ) );
    segmentRectWidth = static_cast<int>( rod.segments[i]->area / widestSegment * ( width - 120 ) );
    drawBeam(painter, i);

    painter.setPen( grayPen );
    if ( i != 0 )
    {
      drawDimensionArrow(painter, width - 10, currentHeight,1);
      drawDimensionArrow(painter, width - 10, currentHeight,0);
    }
  }
  painter.setPen( QPen( Qt::black, 5 ) );
  painter.drawLine( 20, height - currentHeight, width - 25, height - currentHeight );
  painter.setPen( Qt::black );
  painter.setBrush( Qt::blue );
  painter.drawPath( arrow );
  painter.end();

  image.save( "rod.png" );
}

void BeamDrawer::drawLongitudinalForce()
{
  currentHeight = 50;
  QPainterPath path;
  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );

  widestSegment = 0;
  for ( const auto& segment : rod.segments ){
    widestSegment = std::max(widestSegment, std::abs(segment->longitudinalForce));
  }
  path.moveTo(center,currentHeight);
  for ( int i = rod.segments.size()-1; i >= 0; --i )
  {
    segmentRectHeight = static_cast<int>( rod.segments[i]->length / rod.totalLenght * ( height - 100 ) );
    segmentRectWidth = static_cast<int>( rod.segments[i]->longitudinalForce / widestSegment * ( center - 60 ) );
    painter.setPen( grayPen );
    path.lineTo(center+segmentRectWidth, currentHeight);
    rod.segments[i]->longitudinalForce > 0 ?
        painter.drawText( center+segmentRectWidth*1.1, currentHeight+10, QString::number( rod.segments[i]->longitudinalForce/1000 ) )
                                       : painter.drawText( center+segmentRectWidth*1.4, currentHeight+10, QString::number( rod.segments[i]->longitudinalForce/1000 ) );
    currentHeight += segmentRectHeight;
    path.lineTo(center+segmentRectWidth, currentHeight);
    rod.segments[i]->longitudinalForce > 0 ?
        painter.drawText( center+segmentRectWidth*1.1, currentHeight-5, QString::number( rod.segments[i]->longitudinalForce/1000 ) )
                                       : painter.drawText( center+segmentRectWidth*1.4, currentHeight-5, QString::number( rod.segments[i]->longitudinalForce/1000 ) );
  }
  path.lineTo(center,currentHeight);
  path.lineTo(center,50);
  painter.setPen( boldGrayPen );
  painter.setBrush(Qt::HorPattern);
  painter.drawPath(path);
  painter.setPen( greenPen );
  painter.drawLine( center, 50, center, height-50 );
  painter.end();
  image.save( "longitudinal_force.png" );
}

void BeamDrawer::drawStress()
{
  currentHeight = 50;
  QPainterPath path;
  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );
  double stress;

  widestSegment = 0;
  for ( const auto& segment : rod.segments ){
    stress = static_cast<double>(segment->stress/1000);
    widestSegment = std::max(widestSegment, std::abs(stress));
  }
  path.moveTo(center,currentHeight);
  for ( int i = rod.segments.size()-1; i >= 0; --i )
  {
    stress = static_cast<double>(rod.segments[i]->stress/1000);
    segmentRectHeight = static_cast<int>( rod.segments[i]->length / rod.totalLenght * ( height - 100 ) );
    segmentRectWidth = stress / widestSegment * ( center - 60.0 );
    path.lineTo(center+segmentRectWidth, currentHeight);
    stress > 0 ? painter.drawText( center+segmentRectWidth*1.1, currentHeight+10, QString::number( rod.segments[i]->stress/1000000.0 ) )
               :painter.drawText( center+segmentRectWidth*1.5, currentHeight+10, QString::number( rod.segments[i]->stress/1000000.0 ) );
    currentHeight += segmentRectHeight;
    path.lineTo(center+segmentRectWidth, currentHeight);
    stress > 0 ? painter.drawText( center+segmentRectWidth*1.1, currentHeight-5, QString::number( rod.segments[i]->stress/1000000.0 ) )
               : painter.drawText( center+segmentRectWidth*1.5, currentHeight-5, QString::number( rod.segments[i]->stress/1000000.0 ) );
  }
  path.lineTo(center,currentHeight);
  path.lineTo(center,50);
  painter.setPen( boldGrayPen );
  painter.setBrush(Qt::HorPattern);
  painter.drawPath(path);
  painter.setPen( greenPen );
  painter.drawLine( center, 50, center, height-50 );
  painter.end();
  image.save( "stress.png" );
}

void BeamDrawer::drawExtension()
{
  currentHeight = 50;
  double extension;
  QPainterPath path;
  QImage image( width, height, QImage::Format_ARGB32 );
  image.fill( Qt::white );
  QPainter painter( &image );

  widestSegment = 0;
  for ( const auto& segment : rod.segments ){
    widestSegment = std::max(widestSegment, std::abs(segment->extension));
  }
  path.moveTo(center,currentHeight);
  extension = rod.segments.back()->extension;
  for ( int i = rod.segments.size()-1; i >= 0; --i )
  {
    if(i != rod.segments.size()-1){
      extension+=rod.segments[i]->extension;
    }
    segmentRectHeight = static_cast<int>( rod.segments[i]->length / rod.totalLenght * ( height - 100 ) );
    segmentRectWidth = extension / widestSegment * ( center - 60.0 );
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
  painter.setPen( greenPen );
  painter.drawLine( center, 50, center, height-50 );
  painter.end();
  image.save( "extension.png" );
}

void BeamDrawer::drawBeam(QPainter &painter, int segmentIndex)
{
  painter.setPen( QPen(QColor( 140, 140, 140 ), 4) );
  painter.drawRect( QRect( center - segmentRectWidth / 2, currentHeight, segmentRectWidth, segmentRectHeight ) );
  currentHeight += segmentRectHeight;
  painter.drawLine( center, currentHeight, width - 5, currentHeight );
  painter.save();
  painter.translate( width - 15, currentHeight - segmentRectHeight / 2 );
  painter.rotate( -90 );  // подпись длины сегмента
  painter.drawText( 0, 0, QString::number( rod.segments[segmentIndex]->length ) );
  painter.restore();
}

void BeamDrawer::drawDimensionArrow(QPainter &painter, int x, int y, bool isUpward)
{
  int yOffset = isUpward ? -15 : 15;
  painter.drawLine(x, y, x - 3, y + yOffset);
  painter.drawLine(x, y, x + 3, y + yOffset);
}

void BeamDrawer::drawForceArrow(QPainterPath &arrow, int x, int y, int d)
{
  arrow.moveTo( x, y );
  arrow.lineTo( x - 6, y );
  arrow.lineTo( x - 6, y + ( d * 20 ) );
  arrow.lineTo( x - 10, y + ( d * 20 ) );
  arrow.lineTo( x, y + ( d * 30 ) );
  arrow.lineTo( x + 10, y + ( d * 20 ) );
  arrow.lineTo( x + 6, y + ( d * 20 ) );
  arrow.lineTo( x + 6, y );
  arrow.lineTo( x, y );
}
