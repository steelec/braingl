/*
 * colormapwidget.h
 *
 * Created on: Jan 29, 2013
 * @author Ralph Schurade
 */

#ifndef COLORMAPWIDGET_H_
#define COLORMAPWIDGET_H_

#include "../../../data/enums.h"

#include <QFrame>

class QImage;
class QLabel;

class ColormapWidget : public QFrame
{
    Q_OBJECT

public:
    ColormapWidget( int width, float min, float max );
    virtual ~ColormapWidget();

    void resizeEvent( QResizeEvent* event );

private:
    QImage* createImage( int width );
    QColor getInverseColor( QColor color );

    QImage* m_image;
    QLabel* m_clabel;

    int m_colormap;
    int m_width;
    float m_min;
    float m_max;
    float m_selectedMin;
    float m_selectedMax;
    float m_lowerThreshold;
    float m_upperThreshold;



public slots:
    void setLowerThreshold( float value );
    void setUpperThreshold( float value );
    void setMin( float value );
    void setMax( float value );
    void setColormap( int value );
};

#endif /* COLORMAPWIDGET_H_ */
