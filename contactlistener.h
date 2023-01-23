#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include <Box2D/Box2D.h>
#include "world.h"

class ContactListener : public b2ContactListener
{
public:
    ContactListener(World*);
    void BeginContact(b2Contact*);

private:
    World& world;
};

#endif // CONTACTLISTENER_H
