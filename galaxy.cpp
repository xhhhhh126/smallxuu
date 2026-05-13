#include "galaxy.h"
#include <QtMath>

GalaxyWindow::GalaxyWindow(QWidget *parent)
    : QMainWindow(parent), dragging(false), gravityStrength(50), particleCount(500)
{
    setWindowFlags(Qt::FramelessWindowHint);

    // 背景透明，窗口背景通过paintBackground绘制
    setAttribute(Qt::WA_TranslucentBackground);

    center = QPointF(width()/2, height()/2);
    targetCenter = center;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GalaxyWindow::updateScene);
    timer->start(16); // ~60fps

    spawnParticles(particleCount);
}

GalaxyWindow::~GalaxyWindow() {
    for(auto p : particles) delete p;
}

void GalaxyWindow::spawnParticles(int count) {
    // 彩虹色系
    QList<QColor> colors = {
        QColor(255, 100, 150),  // 粉红
        QColor(255, 180, 100),  // 橙色
        QColor(255, 255, 100),  // 黄色
        QColor(100, 255, 150),  // 绿色
        QColor(100, 200, 255),  // 青色
        QColor(150, 100, 255),  // 紫色
        QColor(255, 100, 255),  // 洋红
    };

    for(int i = 0; i < count; i++) {
        Particle* p = new Particle();
        QColor c = colors[qrand() % colors.size()];
        p->reset(QPointF(width()/2, height()/2), c);
        particles.append(p);
    }
}

void GalaxyWindow::paintBackground(QPainter& p) {
    // 渐变背景
    QRadialGradient bg(center, qMax(width(), height()));
    bg.setColorAt(0, QColor(20, 20, 50));
    bg.setColorAt(0.5, QColor(10, 10, 30));
    bg.setColorAt(1, QColor(5, 5, 15));
    p.fillRect(rect(), bg);

    // 星星背景
    static QVector<QPointF> stars;
    if(stars.isEmpty() || stars.size() < 200) {
        qsrand(time(nullptr));
        for(int i = 0; i < 200; i++) {
            stars.append(QPointF(qrand() % 1000, qrand() % 800));
        }
    }

    p.setPen(QPen(QColor(255,255,255,80)));
    for(const auto& s : stars) {
        p.drawPoint(s);
    }
}

void GalaxyWindow::drawCenterBody(QPainter& p) {
    // 中心黑洞效果
    QRadialGradient glow(center, 80);
    glow.setColorAt(0, QColor(255, 200, 100, 150));
    glow.setColorAt(0.3, QColor(255, 100, 50, 80));
    glow.setColorAt(0.7, QColor(100, 50, 150, 30));
    glow.setColorAt(1, QColor(0, 0, 0, 0));

    p.setPen(Qt::NoPen);
    p.setBrush(glow);
    p.drawEllipse(center, 80, 80);

    // 核心亮点
    QRadialGradient core(center, 15);
    core.setColorAt(0, QColor(255, 255, 255, 255));
    core.setColorAt(0.5, QColor(255, 200, 100, 200));
    core.setColorAt(1, QColor(255, 100, 50, 0));
    p.setBrush(core);
    p.drawEllipse(center, 15, 15);
}

void GalaxyWindow::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    paintBackground(p);
    drawCenterBody(p);

    for(auto particle : particles) {
        particle->draw(p);
    }
}

void GalaxyWindow::updateScene() {
    // 鼠标拖拽中心
    if(dragging) {
        center = targetCenter;
    } else {
        center = center + (targetCenter - center) * 0.05f;
    }

    // 更新粒子
    for(auto particle : particles) {
        if(particle->life >= particle->maxLife) {
            particle->reset(center, particle->color);
        }
        particle->update(center, gravityStrength);
    }

    update();
}

void GalaxyWindow::mousePressEvent(QMouseEvent* e) {
    if(e->button() == Qt::LeftButton) {
        dragging = true;
        targetCenter = e->pos();
    }
}

void GalaxyWindow::mouseMoveEvent(QMouseEvent* e) {
    if(dragging) {
        targetCenter = e->pos();
    }
}

void GalaxyWindow::mouseReleaseEvent(QMouseEvent*) {
    dragging = false;
    targetCenter = center;
}

void GalaxyWindow::wheelEvent(QWheelEvent* e) {
    gravityStrength += e->angleDelta().y() * 0.1f;
    gravityStrength = qBound(10.0f, gravityStrength, 200.0f);
}