#ifndef PARTICLE_H
#define PARTICLE_H

#include <QPointF>
#include <QColor>

class Particle {
public:
    QPointF pos;      // 当前位置
    QPointF vel;      // 速度
    QPointF acc;      // 加速度
    float mass;       // 质量
    QColor color;     // 颜色
    int life;         // 寿命
    int maxLife;      // 最大寿命

    // 轨迹历史
    static const int TRAIL_LENGTH = 40;
    QPointF trail[TRAIL_LENGTH];
    int trailIndex;

    Particle();
    void update(const QPointF& center, float strength);
    void draw(QPainter& p);
    void reset(const QPointF& pos, const QColor& color);
};

#endif