#include "widget.h"

#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_button(new QPushButton("Nope.", this))
{
    m_button->setFont(QFont("Arial" , 24));
    m_button->setFocus();
    setGeometry(0,0, 1024, 768);
}

Widget::~Widget()
{
}

void Widget::resizeEvent(QResizeEvent *)
{
    auto center = this->geometry().center();
    QPoint size(100, 50);
    m_button->setGeometry(QRect(center - size, center + size));
}

