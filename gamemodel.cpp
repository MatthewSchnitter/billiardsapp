#include "gamemodel.h"

GameModel::GameModel(World* world) {
    connect(world,
            &World::collisionDetermined,
            this,
            &GameModel::processCollision);

    connect(world,
            &World::ballsStopped,
            this,
            &GameModel::turnOver);

    redBallHit = false;
    yellowBallHit = false;
    wallTouches = 0;
    stopScoring = false;
}

//each time the cue ball collides with another object in the world this method is called
//the method processes the collision and contains logic to decide if a point was scored
void GameModel::processCollision(World::PhysicsObj gameObject) {
    if (stopScoring)
        return;

    //switch on object hit by cue ball
    switch (gameObject) {
        case World::yellowBallEnum:
            yellowBallHit = true;
            //for a point to be scored the cue ball has to hit
            //the second ball after hitting the wall at least 3 times
            if (redBallHit) {
                if (wallTouches > 2) {
                    emit pointEarned();
                }
                stopScoring = true;
            }
            break;

        case World::redBallEnum:
            redBallHit = true;
            //same game logic as above
            if (yellowBallHit) {
                if (wallTouches > 2) {
                    emit pointEarned();
                }
                stopScoring = true;
            }
            break;

        case World::wallEnum:
            wallTouches++;
            break;
    }
}

//a player's shot has ended, "restart" the logic
void GameModel::turnOver() {
    redBallHit = false;
    yellowBallHit = false;
    wallTouches = 0;
    stopScoring = false;
}
