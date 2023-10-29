#pragma once

#include <QWidget>
#include <QVector3D>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

struct MovingWidget {
    MovingWidget(QString const& prefix);

    std::vector<QImage> frames;
    double animSpeed = 1.0;
    double anim = 0.0;
    double speed = 1.0;
    QVector3D position;

    QImage const& currentFrame() const;
    void advance();

private:
    bool tryLoadFrame(QString const&);
};


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
    void makeWidgets();

    QPushButton* m_button;

    double m_time = 0;
    std::vector<MovingWidget> m_movingWidgets;

};
