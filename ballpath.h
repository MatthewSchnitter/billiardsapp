#ifndef BALLPATH_H
#define BALLPATH_H

#include <QObject>
#include <Box2D/Box2D.h>


class ballPath : public QObject
{
    Q_OBJECT
public:
    explicit ballPath(QObject *parent = nullptr);
    void calculatePath(float angleRad, b2Vec2 cueBallPos);

signals:
    void drawPath();


private:
    void reflectPath(b2Vec2 startPoint, b2Vec2 endPoint);

};

#endif // BALLPATH_H
