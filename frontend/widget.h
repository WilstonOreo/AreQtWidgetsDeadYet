#pragma once

#include <QWidget>
#include <QVector3D>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void timerEvent(QTimerEvent*) override;
private:
    void renderWidgets();
    QImage renderWidget(QWidget* widget);

    QPushButton* m_button;

    std::map<QString, QImage> m_widgetImages;
    double m_time = 0;

    std::vector<QVector3D> m_positions;

};
