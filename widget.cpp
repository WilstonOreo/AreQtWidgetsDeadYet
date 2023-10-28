#include "widget.h"

#include <QPushButton>
#include <QCheckBox>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_button(new QPushButton("Nope.", this))
{
    m_button->setFont(QFont("Arial" , 24));
    m_button->setFocus();
    setGeometry(0,0, 1024, 768);

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
    p.drawImage(QRect(QPoint(0,0), image.size() / 2), image);

    QWidget::paintEvent(event);
}

void Widget::resizeEvent(QResizeEvent *)
{
    auto center = this->geometry().center();
    QPoint size(100, 50);
    m_button->setGeometry(QRect(center - size, center + size));
}

void Widget::renderWidgets()
{
        qreal dpr = devicePixelRatioF() * 2;
    auto checkBox = std::make_unique<QCheckBox>("CheckBox");
    checkBox->setGeometry(0,0,200,50);

    QImage image(checkBox->size() * dpr, QImage::Format_ARGB32);
    image.setDevicePixelRatio(dpr);
    QPainter p(&image);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    checkBox->render(&p);

    m_widgetImages["CheckBox"] = image;
}

