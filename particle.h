#ifndef PARTICLE_H
#define PARTICLE_H
#include <QPointF>
#include <QColor>
class QPainter;
class Particle
{
public:
    static constexpr int TRAIL_LENGTH = 40;
    Particle();
    void reset(const QPointF& center, const QColor& c);
    void update(const QPointF& center, float gravityStrength);
    void draw(QPainter& p) const;
    bool dead() const { return life <= 0; }
private:
    QPointF pos;
    QPointF vel;
    QPointF acc;
    QPointF trail[TRAIL_LENGTH];
    int trailIndex = 0;
    float mass = 1.0f;
    QColor color;
    int life = 0;
    int maxLife = 0;
};
#endif // PARTICLE_H