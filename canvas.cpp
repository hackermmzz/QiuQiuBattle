#include "canvas.h"
//////////////////////////////////////////
Timer*SporeSpitTimer=new Timer(50,[&](){
    static QMediaPlayer*player=0;
    if(!player){
        player=new QMediaPlayer();
        player->setMedia(QUrl::fromLocalFile("src/SpitBGM/SpitBGM.wav"));
    }
    else{
        player->setPosition(0);
        player->play();
    }
});
Timer*ShootTimer=new Timer(50,[&]()
{
    static QMediaPlayer*player=0;
    if(!player){
        player=new QMediaPlayer();
        player->setMedia(QUrl::fromLocalFile("src/ShootBGM/ShootBGM.wav"));
    }
    else{
        player->setPosition(0);
        player->play();
    }
});
//////////////////////////////////////////
Canvas::Canvas(QWidget *parent) : QOpenGLWidget(parent)
{
    /////允许接受键盘事件
    setFocusPolicy(Qt::StrongFocus);
    //////鼠标移动事件
    setMouseTracking(true);
    //////
    srand(time(0));
}

Canvas::~Canvas()
{

}

void Canvas::initializeGL()
{
    initializeOpenGLFunctions();
    InitOpenGLAPIPointer(this);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    ///////////////////初始化游戏
    GameInit();
}

void Canvas::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Canvas::paintGL()
{
    static const auto GameBeginTime=QDateTime::currentMSecsSinceEpoch();
    //////////////////////////////////
    openglAPIPointer->glEnable(GL_POINT_SPRITE);
    openglAPIPointer->glEnable(GL_POINT_SMOOTH);
    openglAPIPointer->glEnable(GL_PROGRAM_POINT_SIZE);
    openglAPIPointer->glClear(GL_COLOR_BUFFER_BIT);
    //////////////////////////////////
    if(IsGameFinish)
        GameWin();
    else{
        UpdateAllTimers();
        static Timer*GameUpdateTimer=new Timer(floor(1000.0/(GameFps*3/2)),[&]()
        {
            RenderTime=GameUpdateTimer->GetRealIntervals();
            GameUpdate();
            GameUpdateTimer->Reset();
            //////更新人物体重
            GLuint roleWeight=0;
            for(auto&ball:controller->role.balls)roleWeight+=ball.weight;
            Score->setText(QString("Score:")+to_string(roleWeight).c_str());
            ///////
            if(roleWeight>=WeightWin)IsGameFinish=1;
            //////更新帧率
            uint fps=round(1000.0/RenderTime);
            fps=min(fps,GameFps);
            Fps->setText(QString("FPS:")+to_string(fps).c_str());
            ////
        });
        GameUpdateTimer->start();
        //////////////////////////////////
        //////////////////////////////////
        GameRender();
    }
    glFinish();
    update();
    //////////////////////////////////
    //更新全局脉搏
    GlobalClock=QDateTime::currentMSecsSinceEpoch()-GameBeginTime;
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    auto key=event->key();
    if(key==Qt::Key_Shift){
        if(ShootTimer->complete){
            if(controller->role.Shoot())
                ShootTimer->Reset();
        }
    }
    else if(key==Qt::Key_Space)
    {
        if(SporeSpitTimer->complete)
        {
            if(controller->role.Spit())
                SporeSpitTimer->Reset();
        }

    }
}


void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    auto pos=event->pos();
    float x=pos.x()*2.0/width()-1,y=1-pos.y()*2.0/height();
    actionBar.MoveBar(x,y);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    auto pos=event->pos();
    float x=pos.x()*2.0/width()-1,y=1-pos.y()*2.0/height();
    actionBar.SetClicked(x,y);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    actionBar.ResetBarPos();
}

void Canvas::wheelEvent(QWheelEvent *event)
{

}

void UpdateAllTimers()
{
    SporeSpitTimer->start();
    ShootTimer->start();
}

void GameWin()
{
    ///////////////////////////////////////播放音乐
    static QMediaPlayer*player=0;
    if(!player){
        player=new QMediaPlayer();
        player->setMedia(QUrl::fromLocalFile("src/Win/WinMusic/WLW.wav"));
        player->play();
    }
    ///////////////////////////////////////
    static ShaderProgram*TipRenderProgram=0;
    static VertexArrayObj*vao;
    static GLfloat xsize=1.0;
    static GLfloat ysize=0.5;
    static Texture2D*wintip;
    static vec2 off={0,-1-ysize};
    if(!TipRenderProgram)
    {
        TipRenderProgram=new ShaderProgram("src/Shader/WinTipRender.vert","src/Shader/WinTipRender.frag");
        TipRenderProgram->setuint("sampler",0);
        wintip=new Texture2D("src/Win/WinTip/WinTip.png");
        vao=new VertexArrayObj;
        GLfloat vertex[][4]={{-xsize,ysize,0,0},{xsize,ysize,1,0},{xsize,-ysize,1,1},{-xsize,-ysize,0,1}};
        GLuint indices[]={0,1,2,0,2,3};
        VertexBufferObj*vbo=new VertexBufferObj(sizeof(vertex),vertex);
        vbo->setpoint(0,GL_FLOAT,4,sizeof(GL_FLOAT)*4,0);
        ElementBufferObj*ebo=new ElementBufferObj(sizeof(indices),indices);
        vao->unbind();
    }
     static uint WinTipPlayTime=2000;
     static auto TipPlayBeginTime=QDateTime::currentMSecsSinceEpoch();
     float fac=(QDateTime::currentMSecsSinceEpoch()-TipPlayBeginTime)*1.0/WinTipPlayTime;
     fac=fmin(fac,1.0);
     off.y=(-1-ysize)*(1-fac);
     TipRenderProgram->bind();
     TipRenderProgram->setvec2("off",off);
     wintip->bind(0);
     vao->bind();
     openglAPIPointer->glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
     ///////////////////////////////////////

}
