#include "planeitem.h"
#include <QTimer>
#include <QGraphicsScene>

PlaneItem::PlaneItem(QPixmap pixmap)  :
    wingPosition(WingPosition::Up),
    wingDirection(0)
{
    setPixmap(pixmap);

    QTimer * planeWingsTimer = new QTimer(this);
    connect(planeWingsTimer,&QTimer::timeout,[=](){
        updatePixmap();
    });

    planeWingsTimer->start(80);

    groundPosition = scenePos().y() + 290;


    yAnimation = new QPropertyAnimation(this,"y",this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);

    //yAnimation->start();

    rotationAnimation = new QPropertyAnimation(this,"rotation",this);
    //rotateTo(90,1200,QEasingCurve::InQuad);
}

void PlaneItem::updatePixmap()
{
    if(wingPosition == WingPosition::Middle){

        if(wingDirection){
            //Up
            setPixmap(QPixmap(":/images/airplane_middle.png"));
            wingPosition = WingPosition::Up;
            wingDirection = 0;
        }else{
            //Down
            //Up
            setPixmap(QPixmap(":/images/airplane_middle.png"));
            wingPosition = WingPosition::Down;
            wingDirection = 1;
        }

    }else{
        setPixmap(QPixmap(":/images/airplane_middle.png"));
        wingPosition = WingPosition::Middle;
    }
}

qreal PlaneItem::rotation() const
{
    return m_rotation;
}

void PlaneItem::setRotation(qreal rotation)
{
    m_rotation = rotation;

    QPointF c = boundingRect().center();

    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(rotation);
    t.translate(-c.x(), -c.y());
    setTransform(t);
}

qreal PlaneItem::y() const
{
    return m_y;
}

void PlaneItem::shootUp()
{
    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPosY = y();

    yAnimation->setStartValue(curPosY);
    yAnimation->setEndValue(curPosY - scene()->sceneRect().height()/8);
    yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    yAnimation->setDuration(285);


    connect(yAnimation,&QPropertyAnimation::finished,[=](){
        fallToGroundIfNecessary();
    });


    yAnimation->start();

    rotateTo(-20,200,QEasingCurve::OutCubic);
}

void PlaneItem::startFlying()
{
    yAnimation->start();
    rotateTo(90,1200,QEasingCurve::InQuad);
}

void PlaneItem::freezeInPalce()
{
    yAnimation->stop();
    rotationAnimation->stop();
}

void PlaneItem::setY(qreal y)
{
    moveBy(0,y-m_y);
    m_y = y;
}

void PlaneItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);

    rotationAnimation->start();
}

void PlaneItem::fallToGroundIfNecessary()
{
    if( y() < groundPosition){

        rotationAnimation->stop();
        //yAnimation->stop();

        yAnimation->setStartValue(y());
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setEndValue(groundPosition);
        yAnimation->setDuration(1200);
        yAnimation->start();

        rotateTo(90,1100,QEasingCurve::InCubic);
    }
}
