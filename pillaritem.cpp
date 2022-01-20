#include "pillaritem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include "scene.h"
#include "planeitem.h"
#include <QDebug>

PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar.png"))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillar.png"))),
    pastPlane(false)
{

    topPillar->setPos(QPointF(0,0) - QPointF(topPillar->boundingRect().width()/2,
                                             topPillar->boundingRect().height() + 60));


    bottomPillar->setPos(QPointF(0,0) + QPointF(-bottomPillar->boundingRect().width()/2,
                                                60));

    addToGroup(topPillar);
    addToGroup(bottomPillar);

    yPos = QRandomGenerator::global()->bounded(150);
    int xRandomizer = QRandomGenerator::global()->bounded(200);

    setPos(QPoint(0,0) + QPoint(260+ xRandomizer,yPos));

    xAnimation = new QPropertyAnimation(this,"x",this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-260);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation,&QPropertyAnimation::finished,[=](){
        qDebug() << "Animation finished";
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();


}

PillarItem::~PillarItem()
{
    qDebug() << "Deleting Pillar";
    delete topPillar;
    delete bottomPillar;

}

qreal PillarItem::x() const
{
    return m_x;
}

void PillarItem::freezeInPlace()
{
    xAnimation->stop();
}

void PillarItem::setX(qreal x)
{
    m_x = x;

    if(x < 0 && !pastPlane){
        pastPlane = true;
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene * >(mScene);
        if(myScene){
            myScene->incrementScore();
        }
    }

    if(collideWithPlane()){
        emit collideFail();
    }
    setPos(QPointF(0,0) + QPointF(x,yPos));
}

bool PillarItem::collideWithPlane()
{
    QList<QGraphicsItem*> collidingItems = topPillar->collidingItems();
    collidingItems.append(bottomPillar->collidingItems());

    foreach (QGraphicsItem * item, collidingItems) {
        PlaneItem * planeItem = dynamic_cast<PlaneItem*>(item);
        if(planeItem){
            return true;
        }
    }
    return false;
}
