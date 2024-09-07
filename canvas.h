#ifndef CANVAS_H
#define CANVAS_H
#include"Packaged/include.h"
#include"QKeyEvent"
#include"Game/Game.h"
#include"qthread.h"
/////////////////////////////////////
extern Timer*SporeSpitTimer;
extern Timer*ShootTimer;
/////////////////////////////////////
class Canvas  : public QOpenGLWidget, OPENGLAPIVERSION
{
    Q_OBJECT

public:
    Canvas(QWidget *parent);
    ~Canvas();
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
};
void UpdateAllTimers();
void GameWin();
#endif // CANVAS_H
