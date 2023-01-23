#ifndef WORLD_H
#define WORLD_H

#include <QObject>
#include <Box2D/Box2D.h>

class World : public QObject
{
    Q_OBJECT
public:
    b2World world;
    explicit World(QObject *parent = nullptr);
    void stepSimulation();
    void giveForceToCueBall(float, float);
    void collisionDetected(b2Contact*);
    void levelOne();
    void levelTwo();
    void levelThree();
    void levelFour();

    enum PhysicsObj {redBallEnum, yellowBallEnum, wallEnum};

private:
    const float halfTableWidth = 1.5f;
    const float halfTableHeight = 0.75f;
    const float ballRadius = 0.0305f;
    b2BodyDef ball;
    b2CircleShape ballShape;
    b2FixtureDef ballFixture;

    b2Body* topWall;
    b2Body* rightWall;
    b2Body* bottomWall;
    b2Body* leftWall;

    b2Body* cueBall;
    b2Body* redBall;
    b2Body* yellowBall;

    void createWorld();
    void checkCollision(b2Fixture*);
    void checkBallStates();

signals:
    void updateCueBallPosition(float, float);
    void updateRedBallPosition(float, float);
    void updateYellowBallPosition(float, float);
    void collisionDetermined(World::PhysicsObj);
    void ballsStopped();
};

#endif // WORLD.H
