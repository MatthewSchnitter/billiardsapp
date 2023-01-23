#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include "world.h"

class GameModel : public QObject
{
    Q_OBJECT

public:
    GameModel(World*);

private:
    int wallTouches;
    bool yellowBallHit;
    bool redBallHit;
    bool stopScoring;

    void turnOver();
    void processCollision(World::PhysicsObj);

signals:
    void pointEarned();
};

#endif // GAMEMODEL_H
