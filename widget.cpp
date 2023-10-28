#include "widget.h"
#include "qcoreevent.h"

#include <QPushButton>
#include <QCheckBox>
#include <QPainter>
#include <QTransform>

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_button(new QPushButton("Nope.", this))
{
    m_button->setFont(QFont("Arial" , 24));
    m_button->setFocus();
    setGeometry(0,0, 1024, 768);

    startTimer(10, Qt::PreciseTimer);

    renderWidgets();
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    auto& image = m_widgetImages.at("CheckBox");

    for (int i = 0; i < 100; ++i) {
        QTransform t;
        t.rotate(m_time * 5.0);
        p.setTransform(t);
        p.drawImage(QRect(QPoint(0,0), image.size() / 2), image);
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
    QImage image(widget->size() * dpr, QImage::Format_ARGB32);
    image.setDevicePixelRatio(dpr);
    QPainter p(&image);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    widget->render(&p);
    return image;
}

