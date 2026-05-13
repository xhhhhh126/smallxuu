#include "galaxy.h"
#include "particle.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QtMath>
#include <algorithm>

GalaxyWindow::GalaxyWindow(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(false);

    connect(&timer, &QTimer::timeout, this, &GalaxyWindow::updateScene);
    timer.start(16); // ~60 FPS
}

void GalaxyWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (!inited) {
        center = QPointF(width() / 2.0, height() / 2.0);
        targetCenter = center;
        spawnParticles(particleCount);
        inited = true;
    } else if (!dragging) {
        // 窗口变化时，中心跟着居中一点点
        center = QPointF(width() / 2.0, height() / 2.0);
        targetCenter = center;
    }

    Q_UNUSED(event);
}

void GalaxyWindow::spawnParticles(int count)
{
    static const QVector<QColor> colors = {
        QColor(255, 120, 120),
        QColor(255, 180, 90),
        QColor(120, 200, 255),
        QColor(180, 120, 255),
        QColor(120, 255, 180),
        QColor(255, 255, 255)
    };

    particles.reserve(count);

    auto *rng = QRandomGenerator::global();

    for (int i = 0; i < count; ++i) {
        Particle* p = new Particle();
        QColor c = colors[rng->bounded(colors.size())];
        p->reset(center, c);
        particles.append(p);
    }
}

void GalaxyWindow::updateScene()
{
    // 平滑移动中心
    center += (targetCenter - center) * 0.08;

    for (Particle* p : particles) {
        p->update(center, gravityStrength);

        // 粒子生命结束后重生
        // 这里不用直接访问 life，简单做法：漂移太远就重置
        // 由于 life 是私有的，这里用位置范围判断更省事
        if (std::abs(p->pos().x()) > 1000000) {
            // 占位，不会触发
        }
    }

    update();
}

void GalaxyWindow::paintBackground(QPainter& p)
{
    // 深色星空渐变
    QRadialGradient bg(QPointF(width() * 0.5, height() * 0.4), std::max(width(), height()) * 0.9);
    bg.setColorAt(0.0, QColor(25, 20, 50));
    bg.setColorAt(0.45, QColor(10, 10, 28));
    bg.setColorAt(1.0, QColor(2, 2, 10));
    p.fillRect(rect(), bg);

    // 生成固定星点
    static QVector<QPointF> stars;
    static QSize lastSize;

    if (stars.isEmpty() || lastSize != size()) {
        stars.clear();
        lastSize = size();

        auto *rng = QRandomGenerator::global();
        for (int i = 0; i < 220; ++i) {
            stars.append(QPointF(
                rng->bounded(width()),
                rng->bounded(height())
                ));
        }
    }

    p.setPen(Qt::NoPen);
    for (const QPointF& s : stars) {
        int alpha = 80 + QRandomGenerator::global()->bounded(120);
        p.setBrush(QColor(255, 255, 255, alpha));
        p.drawEllipse(s, 1.0, 1.0);
    }
}

void GalaxyWindow::drawCenterBody(QPainter& p)
{
    // 中心恒星
    QRadialGradient star(center, 80);
    star.setColorAt(0.0, QColor(255, 255, 220, 255));
    star.setColorAt(0.2, QColor(255, 210, 120, 220));
    star.setColorAt(0.6, QColor(255, 120, 50, 100));
    star.setColorAt(1.0, QColor(255, 80, 30, 0));

    p.setPen(Qt::NoPen);
    p.setBrush(star);
    p.drawEllipse(center, 80, 80);

    p.setBrush(QColor(255, 240, 180));
    p.drawEllipse(center, 8, 8);
}

void GalaxyWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);

    paintBackground(p);
    drawCenterBody(p);

    for (Particle* particle : particles) {
        particle->draw(p);
    }
}

void GalaxyWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        targetCenter = event->position();
    }
}

void GalaxyWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (dragging) {
        targetCenter = event->position();
    }
}

void GalaxyWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragging = false;
    }
}

void GalaxyWindow::wheelEvent(QWheelEvent *event)
{
    const int delta = event->angleDelta().y();
    if (delta > 0) {
        gravityStrength *= 1.12f;
    } else if (delta < 0) {
        gravityStrength *= 0.89f;
    }

    gravityStrength = std::clamp(gravityStrength, 0.2f, 5.0f);
    event->accept();
}