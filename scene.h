#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "pillaritem.h"
#include "planeitem.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addPlane();

    void startGame();

    bool getGameOn() const;
    void setGameOn(bool value);

    void incrementScore();

    void setScore(int newScore);

signals:

public slots:

    // QGraphicsScene interface
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);


private:
    void showGameOverGraphics();
    void hideGameOverGraphics();
    void cleanPillars();
    void setUpPillarTimer();
    void freezePlaneAndPillarsInPlace();

    QTimer * pillarTimer;
    PlaneItem * plane;
    bool gameOn;
    int score;
    int bestScore;

    QGraphicsPixmapItem * gameOverPix;
    QGraphicsTextItem * scoreTextItem;

};

#endif // SCENE_H
