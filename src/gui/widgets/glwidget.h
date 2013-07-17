/*
 * glwidget.h
 *
 * Created on: May 03, 2012
 * @author Ralph Schurade
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "GL/glew.h"

#include "../gl/scenerenderer.h"

#include "../../data/enums.h"

#include <QtOpenGL/QGLWidget>

class ArcBall;
class SceneRenderer;
class QItemSelectionModel;

class GLWidget: public QGLWidget
{
    Q_OBJECT

public:
    GLWidget( QString name, QItemSelectionModel* roiSelectionModel, QWidget *parent = 0 );
    GLWidget( QString name, QItemSelectionModel* roiSelectionModel, QWidget *parent, const QGLWidget *shareWidget );
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void setView( Fn::Orient view );
    QImage* screenshot();

    ArcBall* getArcBall();

    float m_x_shift;

private:
    QItemSelectionModel* m_roiSelectionModel;

    ArcBall* m_arcBall;
    SceneRenderer* m_sceneRenderer;

    QMatrix4x4 m_mvMatrix;
    QMatrix4x4 m_pMatrix;

    bool m_visible;

    float m_nx;
    float m_ny;
    float m_nz;

    int m_picked;
    QVector2D m_rightMouseDown;
    QVector2D m_pickOld;
    int m_sliceXPosAtPick;
    int m_sliceYPosAtPick;
    int m_sliceZPosAtPick;
    bool skipDraw;

    int m_width;
    int m_height;

    void calcMVPMatrix();

    void rightMouseDown( QMouseEvent* event );
    void rightMouseDrag( QMouseEvent* event );

protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void mousePressEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    void enterEvent( QEvent* event );
    void wheelEvent( QWheelEvent* event );
    void keyPressEvent( QKeyEvent* event );

public slots:
    void update();
    void visibilityChanged( bool visible );

signals:
    void signalKeyPressed( int key, Qt::KeyboardModifiers mods );
};

#endif
