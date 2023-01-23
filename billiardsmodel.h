#ifndef BILLIARDSMODEL_H
#define BILLIARDSMODEL_H

#include <QObject>
#include <QPoint>
#include <math.h>
#include "world.h"
#include "gamemodel.h"
#include "contactlistener.h"

class BilliardsModel : public QObject
{
    Q_OBJECT

public:
    explicit BilliardsModel(QObject *parent = nullptr);
    void stepSimulation();
    void onShot();
    void onChangeDirectionLeft();
    void onChangeDirectionRight();
    void onIncreasePower();
    void onDecreasePower();
    void switchLevel(int);
    void resetLevel();

private:
    World world;
    GameModel gameModel;
    int currentLevel;
    float direction;
    float magnitude;
    bool shotAvailable;
    ContactListener listener;

    void createWorld();
    void addBalls();
    void updateCueBall(float, float);
    void updateRedBall(float, float);
    void updateYellowBall(float, float);
    void calculatePath(float, float);
    void reflectPath(b2Vec2 startPoint, b2Vec2 endPoint, QVector<QPoint> ballPath);
    void drawPath();
    void calculatePath();
    void pointEarned();
    void ballsStopped();

signals:
    void updateCueBallPosition(float, float);
    void updateRedBallPosition(float, float);
    void updateYellowBallPosition(float, float);
    void updatePower(float);
    void addOneScore();
    void drawLine(QVector<QPoint> ballPath);
};

#endif // BILLIARDSMODEL_H
