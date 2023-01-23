#include "billiardsmodel.h"

//constructor for the billiards model, sets a world and a contact listener
BilliardsModel::BilliardsModel(QObject *parent)
    : QObject{parent}, gameModel(&world), currentLevel(0), listener(&world)
{
    connect(&world,
            &World::updateCueBallPosition,
            this,
            &BilliardsModel::updateCueBall);
    connect(&world,
            &World::updateRedBallPosition,
            this,
            &BilliardsModel::updateRedBall);
    connect(&world,
            &World::updateYellowBallPosition,
            this,
            &BilliardsModel::updateYellowBall);
    connect(&gameModel,
            &GameModel::pointEarned,
            this,
            &BilliardsModel::pointEarned);
    connect(&world,
            &World::ballsStopped,
            this,
            &BilliardsModel::ballsStopped);

    direction = 0.0f;
    magnitude = 0.5f;

    world.world.SetContactListener(&listener);
}

//steps the simulation in the world
void BilliardsModel::stepSimulation() {
    world.stepSimulation();
}

//relay signal from world to view
void BilliardsModel::updateCueBall(float x, float y) {
    emit updateCueBallPosition(x, y);
    if (shotAvailable) {
        calculatePath(x,y);
    }
}

//relay signal from world to view
void BilliardsModel::updateRedBall(float x, float y) {
    emit updateRedBallPosition(x, y);
}

//relay signal from world to view
void BilliardsModel::updateYellowBall(float x, float y) {
    emit updateYellowBallPosition(x, y);
}

//calculates the path of the ball
void BilliardsModel::calculatePath(float x, float y) {
    // Points of the ray
    float pathLength = magnitude;
    b2Vec2 startPoint(x, y);

    //calculates endpoint according to the magnitude and direction
    b2Vec2 endPoint = startPoint + pathLength * b2Vec2(sinf(M_PI / 2 - direction), cosf(M_PI / 2 - direction));

    //vector for recursive method
    QVector<QPoint> ballPath ;
    reflectPath(startPoint, endPoint, ballPath);
}

//finds all the reflections or "bounces" using raycasting and tells view to draw it
void BilliardsModel::reflectPath(b2Vec2 startPoint, b2Vec2 endPoint, QVector<QPoint> ballPath) {
    // Offset Values taken from view
    int worldToViewRatio = 262;
    int tableOffSetX = 393;
    int tableOffSetY = 196;

    // Create rayCast input
    b2RayCastInput input;
    input.p1 = startPoint;
    input.p2 = endPoint;
    input.maxFraction = 1;

    // Find the closest object inside of the box2D world
    float otherFraction = 1;
    b2Vec2 normalIntersection(0, 0);

    for (b2Body *body = world.world.GetBodyList(); body; body = body->GetNext()){
        for(b2Fixture *fixtrue = body->GetFixtureList(); fixtrue; fixtrue = fixtrue->GetNext()){
            b2RayCastOutput output;

            // RayCast returns true if a ray hits a fixtrue
            if(! fixtrue->RayCast(&output, input, 0))
                continue;
            if(output.fraction < otherFraction) {
                otherFraction = output.fraction;
                normalIntersection = output.normal;
            }
        }
    }

    b2Vec2 intersect = startPoint + otherFraction * (endPoint - startPoint);

    // +25 for the cushions at the ends of the table
    float x1 = (input.p1.x * worldToViewRatio + tableOffSetX) + 25;
    float y1 = (input.p1.y * worldToViewRatio + tableOffSetY) + 25;
    float x2 = (intersect.x * worldToViewRatio + tableOffSetX) + 25;
    float y2 = (intersect.y * worldToViewRatio + tableOffSetY) + 25;

    ballPath << QPoint(x1, y1);
    ballPath << QPoint(x2, y2);

    if (otherFraction == 1 || otherFraction == 0){
        emit drawLine(ballPath);
        return;
    }

    b2Vec2 remainingRay = (endPoint - intersect);
    b2Vec2 incidenceRay = b2Dot(remainingRay, normalIntersection) * normalIntersection;
    b2Vec2 nextP2 = endPoint - 2 * incidenceRay;
    reflectPath(intersect, nextP2, ballPath);
}

//called when user takes a shot
void BilliardsModel::onShot() {
    if (shotAvailable) {
        world.giveForceToCueBall(direction, magnitude);
        shotAvailable = false;
        //this gets rid of the path of the cue ball
        QVector<QPoint> vector {QPoint(0, 0)};
        emit drawLine(vector);
    }
}

//when user changes direction to the left
void BilliardsModel::onChangeDirectionLeft() {
    direction -= 0.01f;
}

//when user changes direction to the right
void BilliardsModel::onChangeDirectionRight() {
    direction += 0.01f;
}

//when the user increases the power
void BilliardsModel::onIncreasePower() {
    if (magnitude < 5.0f)
        magnitude += 0.05f;

    emit updatePower(magnitude / 5.0f * 100);
}

//when the user decreases the power
void BilliardsModel::onDecreasePower() {
    if (magnitude > 0.1f)
        magnitude -= 0.05f;

    emit updatePower(magnitude / 5.0f * 100);
}

//relay signal that a point was scored
void BilliardsModel::pointEarned() {
    emit addOneScore();
}

//when the user switches levels, the balls have to be placed in the correct positions for that level
void BilliardsModel::switchLevel(int level) {
    switch(level) {
    case 0:
        //freeplay, same setup as level one
    case 1:
        world.levelOne();
        break;

    case 2:
        world.levelTwo();
        break;

    case 3:
        world.levelThree();
        break;

    case 4:
        world.levelFour();
        break;
    }

    currentLevel = level;
}

//resets the current level by putting the balls back in the starting position
void BilliardsModel::resetLevel() {
    switch(currentLevel) {
    case 0:
        //freeplay, same setup as level one
    case 1:
        world.levelOne();
        break;

    case 2:
        world.levelTwo();
        break;

    case 3:
        world.levelThree();
        break;

    case 4:
        world.levelFour();
        break;
    }
}

//when the balls stop moving, allow the player to shoot again
void BilliardsModel::ballsStopped(){
    shotAvailable = true;
}
