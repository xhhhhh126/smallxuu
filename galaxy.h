#ifndef GALAXY_H
#define GALAXY_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include <QPainter>
#include <QPointF>
#include <QVector>
#include <QColor>
#include <ctime>
#include "particle.h"

class GalaxyWindow : public QMainWindow {
    Q_OBJECT

public:
    GalaxyWindow(QWidget *parent = nullptr);
    ~GalaxyWindow();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent* e) override;

private slots:
    void updateScene();

private:
    QVector<Particle*> particles;
    QTimer* timer;
    QPointF center;          // 星系中心
    QPointF targetCenter;    // 目标中心
    bool dragging;
    float gravityStrength;
    int particleCount;

    void spawnParticles(int count);
    void paintBackground(QPainter& p);
    void drawCenterBody(QPainter& p);
};

#endif