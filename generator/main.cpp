#include <QApplication>
#include <QtWidgets>
#include <QImage>
#include <QPainter>

#include "stepinterface.h"
#include "wigglywidget.h"
#include "analogclock.h"

struct WidgetGenerator {

    void renderWidgetToFile(QString const& filename, std::function<QWidget*()> f) {
        std::unique_ptr<QWidget> widget(f());

        if (auto* stepInterface = qobject_cast<StepInterface*>(widget.get())) {
            for (int step = 0; step < stepInterface->stepCount(); ++step) {
                stepInterface->setStep(step);
                auto image = renderWidget(widget.get());
                image.save(QString("%1.%2.png").arg(filename).arg(step));
            }
        } else {
            auto image = renderWidget(widget.get());
            image.save(QString("%1.png").arg(filename));
        }
    }

    QImage renderWidget(QWidget *widget) const
    {
        qreal dpr = widget->devicePixelRatioF() * 2;
        QImage image(widget->minimumSizeHint() * dpr, QImage::Format_ARGB32_Premultiplied);
        image.setDevicePixelRatio(dpr);
        image.fill(0);
        QPainter p(&image);
        p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        widget->render(&p, QPoint(), QRegion(), QWidget::DrawChildren);
        return image;
    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    WidgetGenerator generator;
    std::map<QString, std::function<QWidget*()>> widgets {
        { "checkbox0", []() -> QWidget* { return new QCheckBox("Check Me"); } },
        { "checkbox1", []() -> QWidget* { auto* chk = new QCheckBox("Done");
            chk->setChecked(true);
            return chk;
        } },
        { "button0", []() -> QWidget* { return new QPushButton("Click Me"); } },
        { "button1", []() -> QWidget* { auto *btn = new QToolButton; btn->setText("Tool Time"); return btn; } },
        { "radiobutton0", []() -> QWidget* { auto* btn = new QRadioButton("Radio Gaga");
             btn->setChecked(true);
             return btn;
        } },
        { "radiobutton1", []() -> QWidget* { return new QRadioButton("Radio Gugu"); } },
        { "spinbox0", []() -> QWidget* { auto* box = new QSpinBox; box->setMaximum(100); box->setValue(42); return box; } },
        { "spinbox1", []() -> QWidget* { auto* box = new QDoubleSpinBox; box->setMaximum(10000); box->setValue(3.1459); box->setDecimals(4); box->setSingleStep(0.0001); return box; } },
        { "groupbox", []() -> QWidget* { auto* grp = new QGroupBox("Group");
             auto* layout = new QHBoxLayout;
            layout->addWidget(new QRadioButton("Option 1"));
            layout->addWidget(new QRadioButton("Option 2"));
            grp->setLayout(layout);
            return grp;
         } },
        { "wigglywidget", []() -> QWidget* { return new WigglyWidget; } },
        { "analogclock", []() -> QWidget* { return new AnalogClock; } },
    };

    for (auto& [name, ctor] : widgets) {
        generator.renderWidgetToFile(name, ctor);
    }

    return EXIT_SUCCESS;
}
