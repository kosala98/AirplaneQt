#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>
Scene::Scene(QObject *parent) : QGraphicsScene(parent),
    gameOn(false),score(0),
    bestScore(0)
{
    setUpPillarTimer();
}

void Scene::addPlane()
{
    plane = new PlaneItem(QPixmap(":/images/airplane_up.png"));
    addItem(plane);
}

void Scene::startGame()
{
    //plane
    plane->startFlying();
    //pillars
    if(!pillarTimer->isActive()){
        cleanPillars();
        setGameOn(true);
        setScore(0);
        hideGameOverGraphics();
        pillarTimer->start(1000);
    }
}

void Scene::setUpPillarTimer()
{
    pillarTimer = new QTimer(this);
    connect(pillarTimer,&QTimer::timeout,[=](){

        PillarItem * pillarItem = new PillarItem();
        connect(pillarItem,&PillarItem::collideFail,[=](){
            pillarTimer->stop();
            freezePlaneAndPillarsInPlace();

            setGameOn(false);
            showGameOverGraphics();

        });

        addItem(pillarItem);

    });

    pillarTimer->start(1000);
}

void Scene::freezePlaneAndPillarsInPlace()
{
    //Freeze plane
    plane->freezeInPalce();

    //Freeze pillars
    QList<QGraphicsItem *> sceneItems = items();
    foreach (QGraphicsItem *item, sceneItems) {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar){
            pillar->freezeInPlace();
        }
    }
}

void Scene::setScore(int newScore)
{
    score = newScore;
}

bool Scene::getGameOn() const
{
    return gameOn;
}

void Scene::setGameOn(bool value)
{
    gameOn = value;
}

void Scene::incrementScore()
{
    score++;
    if(score >bestScore)
        bestScore = score;
    qDebug() << "Score : " << score << "Best Score : " << bestScore;
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        if(gameOn){
            plane->shootUp();
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(gameOn){
            plane->shootUp();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::showGameOverGraphics()
{
    gameOverPix = new QGraphicsPixmapItem(QPixmap(":/images/game_over_red new.png"));
    addItem(gameOverPix);
    gameOverPix->setPos(QPointF(0,0) - QPointF(gameOverPix->boundingRect().width()/2,
                                               gameOverPix->boundingRect().height()/2));
    scoreTextItem = new QGraphicsTextItem();

    QString htmlString = "<p> Score : " + QString::number(score) + "</p>"
            + "<p> Best Score : " + QString::number(bestScore) + "</p>";

    QFont mfont("Consolas", 30, QFont::Bold);

    scoreTextItem->setHtml(htmlString);
    scoreTextItem->setFont(mfont);
    scoreTextItem->setDefaultTextColor(Qt::red);
    addItem(scoreTextItem);

    scoreTextItem->setPos(QPointF(0,0) - QPointF(scoreTextItem->boundingRect().width()/2,
                                                 -gameOverPix->boundingRect().height()/2));
}

void Scene::hideGameOverGraphics()
{
    if(gameOverPix){
        removeItem(gameOverPix);
        delete gameOverPix;
        gameOverPix=nullptr;
    }
    if(scoreTextItem){
        removeItem(scoreTextItem);
        delete scoreTextItem;
        scoreTextItem = nullptr;
    }
}

void Scene::cleanPillars()
{
    QList<QGraphicsItem *> sceneItem = items();
    foreach (QGraphicsItem *item, sceneItem) {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar){
            removeItem(pillar);
            delete pillar;
        }
    }
}
