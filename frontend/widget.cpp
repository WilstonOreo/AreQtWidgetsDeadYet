#include "widget.h"
#include "qcoreevent.h"

#include <QFile>
#include <QPushButton>
#include <QCheckBox>
#include <QPainter>
#include <QTransform>
#include <QString>
#include <QRandomGenerator>
#include <QDirIterator>

#include <set>

static QString WIDGET_DIR(":/widgets/");

// Generate Random number 0.0 to 1.0
qreal rnd() {
    return QRandomGenerator::global()->bounded(0, 1000) / 1000.0;
}


MovingWidget::MovingWidget(const QString &prefix) :
    position(rnd() - 0.5, rnd() - 0.5, rnd())
{
    if (!tryLoadFrame(WIDGET_DIR + prefix + ".png")) {
        for (int i = 0; i < 100; ++i) {
            if (!tryLoadFrame(QString("%1%2.%3.png").arg(WIDGET_DIR, prefix).arg(i)))
                break;
        }
    }
    anim = rnd() * frames.size();
    animSpeed = frames.size() / 25.0;
    speed = 1.0 + rnd();
}

QImage const& MovingWidget::currentFrame() const
{
    auto idx = static_cast<size_t>(floor(anim)) % frames.size();
    return frames.at(idx);
}

void MovingWidget::advance() {
    position += QVector3D(0.0,0.0, 0.001 * speed);
    if (position.z() > 1.0) {
        qreal i;
        position.setZ(std::modf(position.z(), &i));
    }
    anim += animSpeed;
}


bool MovingWidget::tryLoadFrame(const QString &filename)
{
    if (QFile::exists(filename)) {
        QImage frame;
        frame.load(filename);
        frames.push_back(frame);
        return true;
    }

    return false;
}



Widget::Widget(QWidget *parent)
    : QWidget(parent), m_button(new QPushButton("Nope.", this))
{
    m_buttonHits = m_maxButtonHits;

    m_button->setFont(QFont("Arial" , 24));
    m_button->setFocus();
    connect(m_button, &QPushButton::clicked, this, &Widget::onButtonClick);

    setGeometry(0,0, 1024, 768);

    for (int i = 0; i < 3; ++i)
        makeWidgets();


    startTimer(40, Qt::PreciseTimer);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    if (m_button->isVisible()) {
        // Paint score
        if (m_clickedOnce) {
            int clicks = int(m_buttonHits + 1.0);
            QFont f;
            f.setPixelSize(20);
            p.setFont(f);

            p.drawText(QPoint(16,36), QString("Just %1 more click%2!").arg(clicks).arg(clicks > 1 ? "s" : ""));
        }
    } else {
        // Paint widget starfield
        static std::vector<MovingWidget*> widgets;
        widgets.clear();
        widgets.reserve(m_movingWidgets.size());
        for (auto& widget : m_movingWidgets) {
            widgets.push_back(&widget);
        }
        std::sort(widgets.begin(), widgets.end(), [&](auto& lhs, auto& rhs) {
            return lhs->position.z() < rhs->position.z();
        });

        auto sizeFactor = width() / 400.0 * (qreal(height()) / width());

        for (auto& movingWidget : widgets) {
            QTransform t;
            auto& frame = movingWidget->currentFrame();
            auto pos = movingWidget->position;

            t.translate(width() / 2 - frame.width() /  2 , height() / 2 - frame.height() / 2);

            auto f = -200.0 * sizeFactor / (1.0 - pos.z());
            t.translate(pos.x() * f, pos.y() * f);

            auto scale = pos.z() * sizeFactor;
            t.scale(scale, scale);

            p.setTransform(t);

            p.drawImage(QRect(QPoint(0,0), frame.size() / 2), frame);
            p.resetTransform();
        }
    }

    QWidget::paintEvent(event);
}

void Widget::resizeEvent(QResizeEvent *)
{
    auto center = this->geometry().center();
    QPoint size(100, 50);
    if (!m_clickedOnce) {
        m_button->setGeometry(QRect(center - size, center + size));
    }
}

void Widget::timerEvent(QTimerEvent *event)
{
    m_time += 0.04;
    for (auto& movingWidget : m_movingWidgets) {
        movingWidget.advance();
    }

    m_buttonHits += 0.04;
    m_buttonHits = std::min(m_buttonHits, m_maxButtonHits);

    update();
}

void Widget::makeWidgets()
{
    std::set<QString> prefixes;
    QDirIterator it(":/widgets");
    while (it.hasNext()) {
        QString dir = it.next();

        dir.remove(WIDGET_DIR);
        if (dir.isEmpty())
            continue;

        auto prefix = dir.first(dir.indexOf("."));
        if (prefixes.count(prefix) == 0) {
            m_movingWidgets.emplace_back(prefix);
        }

        prefixes.insert(prefix);
    }
}

void Widget::onButtonClick()
{
    m_buttonHits -= 1.0;
    auto s = m_button->size();

    m_clickedOnce = true;
    m_button->setGeometry(QRect(QPoint(rnd() * (width() - s.width()),rnd() * (height() - s.height())), s));

    if (m_buttonHits < 0.0)
        m_button->setVisible(false);

    update();
}
