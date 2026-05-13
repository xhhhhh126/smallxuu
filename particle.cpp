#include "particle.h"
#include <QtMath>
#include <QPainter>

Particle::Particle() {
    mass = 1.0f;
    life = 0;
    maxLife = 300 + rand() % 200;
    trailIndex = 0;
    for(int i = 0; i < TRAIL_LENGTH; i++) trail[i] = QPointF(0, 0);
}

void Particle::reset(const QPointF& p, const QColor& c) {
    float angle = (float)qrand() / RAND_MAX * M_PI * 2;
    float speed = 1.0f + (float)qrand() / RAND_MAX * 2.0f;

    pos = p + QPointF(cos(angle) * 50, sin(angle) * 50);
    vel = QPointF(cos(angle + M_PI/2) * speed, sin(angle + M_PI/2) * speed);
    acc = QPointF(0, 0);
    color = c;
    life = 0;
    maxLife = 300 + rand() % 200;
    trailIndex = 0;
}

void Particle::update(const QPointF& center, float strength) {
    // 向心力（模拟引力）
    QPointF r = center - pos;
    float dist = qSqrt(r.x()*r.x() + r.y()*r.y());

    if(dist > 5) {
        float force = strength * mass / (dist * 0.1 + 1);
        acc = QPointF(r.x() / dist * force, r.y() / dist * force);
    }

    // 切向速度（保持旋转）
    vel += acc;

    // 阻尼
    vel *= 0.998f;

    // 更新位置
    pos += vel;

    // 轨迹
    trail[trailIndex % TRAIL_LENGTH] = pos;
    trailIndex++;

    life++;
}

void Particle::draw(QPainter& p) {
    float alpha = 1.0f - (float)life / maxLife;
    if(alpha <= 0) return;

    // 绘制轨迹
    for(int i = 1; i < TRAIL_LENGTH; i++) {
        int idx = (trailIndex - i + TRAIL_LENGTH) % TRAIL_LENGTH;
        int nextIdx = (trailIndex - i + 1 + TRAIL_LENGTH) % TRAIL_LENGTH;

        QColor c = color;
        c.setAlphaF(alpha * (1.0f - (float)i / TRAIL_LENGTH) * 0.5f);

        p.setPen(QPen(c, 2.5));
        p.drawLine(trail[idx], trail[nextIdx]);
    }

    // 绘制核心
    QColor core = color;
    core.setAlphaF(alpha);
    p.setPen(Qt::NoPen);
    p.setBrush(core);
    p.drawEllipse(pos, 4, 4);

    // 发光效果
    QRadialGradient glow(pos, 15);
    glow.setColorAt(0, QColor(color.red(), color.green(), color.blue(), 100 * alpha));
    glow.setColorAt(1, QColor(0, 0, 0, 0));
    p.setBrush(glow);
    p.drawEllipse(pos, 15, 15);
}