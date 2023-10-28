#include "widget.h"
#include "qcoreevent.h"

#include <QPushButton>
#include <QCheckBox>
#include <QPainter>
#include <QTransform>
#include <QRandomGenerator>

// Generate Random number 0.0 to 1.0
qreal rnd() {
    return QRandomGenerator::global()->bounded(0, 1000) / 1000.0;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_button(new QPushButton("Nope.", this))
{
    m_button->setFont(QFont("Arial" , 24));
    m_button->setFocus();
    m_button->setVisible(false);
    setGeometry(0,0, 1024, 768);

    startTimer(10, Qt::PreciseTimer);

    renderWidgets();

    for (int i = 0; i < 100; ++i) {
        m_positions.emplace_back(rnd() - 0.5, rnd() - 0.5, rnd());
    }
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    auto& image = m_widgetImages.at("CheckBox");

    for (auto& pos : m_positions) {
        QTransform t;
        t.translate(width() / 2, height() / 2);

     //   t.rotate(m_time * 5.0);
        auto f = -400.0 / (1.0 - pos.z());
        t.translate(pos.x() * f, pos.y() * f);

        auto scale = pos.z() * 2;
        t.scale(scale, scale);

        p.setTransform(t);
//        p.setOpacity(0.1);



        p.drawImage(QRect(QPoint(0,0), image.size() / 2), image);
        p.resetTransform();


    }

    QWidget::paintEvent(event);
}

void Widget::resizeEvent(QResizeEvent *)
{
    auto center = this->geometry().center();
    QPoint size(100, 50);
    m_button->setGeometry(QRect(center - size, center + size));
}

void Widget::timerEvent(QTimerEvent *event)
{
    m_time += 0.01;
    for (auto& pos : m_positions) {
        pos += QVector3D(0.0,0.0, 0.001);
        if (pos.z() > 1.0) {
            qreal i;
            pos.setZ(std::modf(pos.z(), &i));
        }
    }

    update();
}

void Widget::renderWidgets()
{
    auto checkBox = std::make_unique<QCheckBox>("CheckBox");
    checkBox->setGeometry(0,0,200,50);

    m_widgetImages["CheckBox"] = renderWidget(checkBox.get());
}

QImage Widget::renderWidget(QWidget *widget)
{
    qreal dpr = devicePixelRatioF() * 2;
    QImage image(widget->size() * dpr, QImage::Format_ARGB32_Premultiplied );
    image.setDevicePixelRatio(dpr);
    image.fill(0);
    QPainter p(&image);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    widget->render(&p, QPoint(), QRegion(), QWidget::DrawChildren);
    return image;
}

