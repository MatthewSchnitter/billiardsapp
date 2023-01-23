#include "world.h"

//constructs the box2d world
World::World(QObject *parent)
    : QObject{parent}, world(b2Vec2(0.0f, 0.0f))
{
    createWorld();

    //defines fixture def for each ball
    ball.type = b2_dynamicBody;
    ball.bullet = true;
    ball.angularDamping = 0.2f;
    ball.linearDamping = 0.35f;

    ballShape.m_radius = ballRadius;

    ballFixture.shape = &ballShape;
    ballFixture.density = 1.0f;
    ballFixture.restitution = 0.7f;

    //places initial balls
    ball.position.Set(-0.75f, 0.2f);
    cueBall = world.CreateBody(&ball);
    cueBall->CreateFixture(&ballFixture);

    ball.position.Set(0.75f, 0.0f);
    redBall = world.CreateBody(&ball);
    redBall->CreateFixture(&ballFixture);

    ball.position.Set(-0.75f, 0.0f);
    yellowBall = world.CreateBody(&ball);
    yellowBall->CreateFixture(&ballFixture);

}

//adds the walls needed to the world
void World::createWorld() {
    b2BodyDef cushion;
    cushion.type = b2_staticBody;

    b2PolygonShape longWallBox;
    longWallBox.SetAsBox(halfTableWidth, 0.1f);
    b2PolygonShape shortWallBox;
    shortWallBox.SetAsBox(0.1f, halfTableHeight);

    b2FixtureDef wallFixture;
    wallFixture.shape = &longWallBox;
    wallFixture.friction = 0.0f;

    cushion.position.Set(0.0f, (halfTableHeight) + 0.1f);
    topWall = world.CreateBody(&cushion);
    topWall->CreateFixture(&wallFixture);

    cushion.position.Set(0.0f, (-halfTableHeight) - 0.1f);
    bottomWall = world.CreateBody(&cushion);
    bottomWall->CreateFixture(&wallFixture);

    wallFixture.shape = &shortWallBox;

    cushion.position.Set((halfTableWidth) + 0.1f, 0.0f);
    rightWall = world.CreateBody(&cushion);
    rightWall->CreateFixture(&wallFixture);

    cushion.position.Set((-halfTableWidth) - 0.1f, 0.0f);
    leftWall = world.CreateBody(&cushion);
    leftWall->CreateFixture(&wallFixture);

}

//method that runs the simulation
void World::stepSimulation() {
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 8;
    int32 positionIterations = 3;

    world.Step(timeStep, velocityIterations, positionIterations);

    b2Vec2 cueBallPosition = cueBall->GetPosition();
    b2Vec2 redBallPosition = redBall->GetPosition();
    b2Vec2 yellowBallPosition = yellowBall->GetPosition();

    emit updateCueBallPosition(cueBallPosition.x, cueBallPosition.y);
    emit updateRedBallPosition(redBallPosition.x, redBallPosition.y);
    emit updateYellowBallPosition(yellowBallPosition.x, yellowBallPosition.y);

    checkBallStates();
}

//method called when a shot is taken
void World::giveForceToCueBall(float direction, float magnitude) {
    b2Vec2 force;
    force.x = magnitude * cos(direction);
    force.y = magnitude * sin(direction);
    cueBall->ApplyForceToCenter(force, true);
}

//determines if the collision is with the cue ball
void World::collisionDetected(b2Contact* contact) {
    if (contact->GetFixtureA() == cueBall->GetFixtureList())
        checkCollision(contact->GetFixtureB());

    if (contact->GetFixtureB() == cueBall->GetFixtureList())
        checkCollision(contact->GetFixtureA());
}

//sends out different signals depending on what the cue ball has hit
void World::checkCollision(b2Fixture* fixture) {
    if (fixture == redBall->GetFixtureList())
        emit collisionDetermined(redBallEnum);

    if (fixture == yellowBall->GetFixtureList())
        emit collisionDetermined(yellowBallEnum);

    if (fixture == topWall->GetFixtureList()    ||
        fixture == rightWall->GetFixtureList()  ||
        fixture == bottomWall->GetFixtureList() ||
        fixture == leftWall->GetFixtureList())
        emit collisionDetermined(wallEnum);
}

//checks if all of the balls have stopped moving
void World::checkBallStates() {
    if (cueBall->GetLinearVelocity().x == 0    && cueBall->GetLinearVelocity().y == 0 &&
        yellowBall->GetLinearVelocity().x == 0 && yellowBall->GetLinearVelocity().y == 0 &&
        redBall->GetLinearVelocity().x == 0    && redBall->GetLinearVelocity().y == 0)
        emit ballsStopped();
}

//puts the balls into position for level one
//this is the starting position in a normal game
void World::levelOne() {
    world.DestroyBody(cueBall);
    world.DestroyBody(redBall);
    world.DestroyBody(yellowBall);

    ball.position.Set(-0.75f, 0.2f);
    cueBall = world.CreateBody(&ball);
    cueBall->CreateFixture(&ballFixture);

    ball.position.Set(0.75f, 0.0f);
    redBall = world.CreateBody(&ball);
    redBall->CreateFixture(&ballFixture);

    ball.position.Set(-0.75f, 0.0f);
    yellowBall = world.CreateBody(&ball);
    yellowBall->CreateFixture(&ballFixture);
}

//puts the balls into position for level two
void World::levelTwo() {
    world.DestroyBody(cueBall);
    world.DestroyBody(redBall);
    world.DestroyBody(yellowBall);

    ball.position.Set(0.0f, -0.17f);
    cueBall = world.CreateBody(&ball);
    cueBall->CreateFixture(&ballFixture);

    ball.position.Set(0.6f, -0.5f);
    redBall = world.CreateBody(&ball);
    redBall->CreateFixture(&ballFixture);

    ball.position.Set(0.65f, -0.45f);
    yellowBall = world.CreateBody(&ball);
    yellowBall->CreateFixture(&ballFixture);
}

//puts the balls into position for level three
void World::levelThree() {
    world.DestroyBody(cueBall);
    world.DestroyBody(redBall);
    world.DestroyBody(yellowBall);

    ball.position.Set(-0.5f, 0.0f);
    cueBall = world.CreateBody(&ball);
    cueBall->CreateFixture(&ballFixture);

    ball.position.Set(0.0f, -0.005f);
    redBall = world.CreateBody(&ball);
    redBall->CreateFixture(&ballFixture);

    ball.position.Set(0.9f, 0.0f);
    yellowBall = world.CreateBody(&ball);
    yellowBall->CreateFixture(&ballFixture);
}

//puts the balls into position for level four
void World::levelFour() {
    world.DestroyBody(cueBall);
    world.DestroyBody(redBall);
    world.DestroyBody(yellowBall);

    ball.position.Set(0.63f, -0.43f);
    cueBall = world.CreateBody(&ball);
    cueBall->CreateFixture(&ballFixture);

    ball.position.Set(0.3f, 0.3f);
    redBall = world.CreateBody(&ball);
    redBall->CreateFixture(&ballFixture);

    ball.position.Set(0.6f, -0.5f);
    yellowBall = world.CreateBody(&ball);
    yellowBall->CreateFixture(&ballFixture);
}
