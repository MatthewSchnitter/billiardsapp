#include "contactlistener.h"

//listener class for collisions
ContactListener::ContactListener(World* world) : world(*world) {}

//calls method in the world when a collision is detected
void ContactListener::BeginContact(b2Contact* contact) {
    world.collisionDetected(contact);
}
