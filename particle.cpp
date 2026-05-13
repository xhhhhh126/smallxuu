#include "particle.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QtMath>
#include <algorithm>

Particle::Particle()
{
    for (int i = 0; i < TRAIL_LENGTH; ++i) {
        trail[i] = QPointF(0, 0);
    }
}

void Particle::reset(const QPointF& center, const QColor& c)
{
    auto *rng = QRandomGenerator::global();

    // 随机角度、距离
    const double angle = rng->generateDouble() * 2.0 * M_PI;
    const double distance = 40.0 + rng->generateDouble() * 260.0;

    pos = center + QPointF(std::cos(angle) * distance, std::sin(angle) * distance);

    // 切向速度，让粒子更像“绕中心旋转”
    QPointF radius = pos - center;
    const double len = std::hypot(radius.x(), radius.y());
    QPointF tangent(-radius.y() / (len + 0.0001), radius.x() / (len + 0.0001));

    const double speed = 0.6 + rng->generateDouble() * 2.4;
    vel = tangent * speed;

    acc = QPointF(0, 0);
    mass = 0.7f + float(rng->generateDouble()) * 1.8f;
    color = c;

    maxLife = 260 + rng->bounded(180);
    life = maxLife;

    trailIndex = 0;
    for (int i = 0; i < TRAIL_LENGTH; ++i) {
        trail[i] = pos;
    }
}

void Particle::update(const QPointF& center, float gravityStrength)
{
    QPointF dir = center - pos;
    const double distSq = dir.x() * dir.x() + dir.y() * dir.y() + 120.0;

    const double dist = std::sqrt(distSq);
    QPointF norm = dir / dist;

    // 一个简单的中心吸引力
    const double force = gravityStrength * 2200.0 / distSq;
    acc = norm * force;

    vel += acc;
    vel *= 0.992;   // 阻尼，避免越转越飞
    pos += vel;

    trail[trailIndex] = pos;
    trailIndex = (trailIndex + 1) % TRAIL_LENGTH;

    --life;
}

void Particle::draw(QPainter& p) const
{
    // 画尾迹
    for (int i = 0; i < TRAIL_LENGTH - 1; ++i) {
        int idx1 = (trailIndex - 1 - i + TRAIL_LENGTH) % TRAIL_LENGTH;
        int idx2 = (trailIndex - 2 - i + TRAIL_LENGTH) % TRAIL_LENGTH;

        double alpha = 1.0 - double(i) / double(TRAIL_LENGTH);
        QColor c = color;
        c.setAlphaF(alpha * 0.35);

        QPen pen(c);
        pen.setWidthF(std::max(0.5, alpha * 2.2));
        p.setPen(pen);
        p.drawLine(trail[idx1], trail[idx2]);
    }

    // 粒子本体
    QColor body = color;
    body.setAlpha(220);

    p.setPen(Qt::NoPen);
    p.setBrush(body);

    const qreal r = 2.0 + (qreal(life) / qreal(maxLife)) * 2.5;
    p.drawEllipse(pos, r, r);
}