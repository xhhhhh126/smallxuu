#ifndef GALAXY_H
#define GALAXY_H
#include <QWidget>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPaintEvent>
#include <QResizeEvent>
class Particle;
class GalaxyWindow : public QWidget
{
public:
    explicit GalaxyWindow(QWidget *parent = nullptr);
    ~GalaxyWindow() override = default;
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    void updateScene();
    void spawnParticles(int count);
    void paintBackground(QPainter& p);
    void drawCenterBody(QPainter& p);
private:
    QVector<Particle*> particles;
    QTimer timer;
    QPointF center;
    QPointF targetCenter;
    bool dragging = false;
    float gravityStrength = 1.0f;
    int particleCount = 400;
    bool inited = false;
};
#endif // GALAXY_H