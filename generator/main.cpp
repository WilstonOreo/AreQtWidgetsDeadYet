#include <QApplication>
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include <QFile>

#include "analogclock.h"
#include "stepinterface.h"
#include "widgets.h"
#include "wigglywidget.h"

struct WidgetGenerator {
    WidgetGenerator(QString const& dir) : dir(dir), qrcFile(QString("%1/widgets.qrc").arg(dir)), ts(&qrcFile) {
        if (!qrcFile.open(QFile::WriteOnly | QFile::Text)) {
            qFatal() << "Could not open file for writing:" << qrcFile.fileName();
        }

        ts << "<!DOCTYPE RCC><RCC version=\"1.0\">\n";
        ts << "<qresource prefix=\"/widgets\">\n";
    }

    ~WidgetGenerator() {
        ts << "</qresource>\n";
        ts << "</RCC>\n";
    }

    void renderWidgetToFile(QString const& filename, std::function<QWidget*()> f) {
        std::unique_ptr<QWidget> widget(f());

        if (auto* stepInterface = qobject_cast<StepInterface*>(widget.get())) {
            for (int step = 0; step < stepInterface->stepCount(); ++step) {
                stepInterface->setStep(step);
                renderWidgetToPng(widget.get(), QString("%1/%2.%3.png").arg(dir, filename).arg(step));
            }
        } else {
            renderWidgetToPng(widget.get(), QString("%1/%2.png").arg(dir, filename));
        }
    }

    void renderWidgetToPng(QWidget* widget, QString const& filename) {
        auto image = renderWidget(widget);
        image.save(filename);
        ts << QString("\t<file>%1</file>\n").arg(QFileInfo(filename).fileName());
    }

    QImage renderWidget(QWidget *widget) const
    {
        qreal dpr = widget->devicePixelRatioF() * 2;

        auto s = widget->minimumSizeHint();

        QImage image(s * dpr, QImage::Format_ARGB32_Premultiplied);
        image.setDevicePixelRatio(dpr);
        image.fill(0);
        QPainter p(&image);
        p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        widget->render(&p, QPoint(), QRegion(), QWidget::DrawChildren);
        return image;
    }

private:
    QString dir;
    QFile qrcFile;
    QTextStream ts;
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    WidgetGenerator generator(qEnvironmentVariable("GENERATOR_OUTPUT_DIR"));

    std::map<QString, std::function<QWidget*()>> widgets {
        { "checkbox0", []() -> QWidget* { return new QCheckBox("Check Me"); } },
        { "checkbox1", []() -> QWidget* { auto* chk = new QCheckBox("Done");
            chk->setChecked(true);
            return chk;
        } },
        { "checkbox2", []() -> QWidget* { return new QCheckBox("Off"); } },
        { "checkbox3", []() -> QWidget* { auto* chk = new QCheckBox("On");
             chk->setChecked(true);
             return chk;
        } },
        { "button0", []() -> QWidget* { return new QPushButton("Click Me"); } },
        { "button1", []() -> QWidget* { return new QPushButton("Ok"); } },
        { "button2", []() -> QWidget* { return new QPushButton("Cancel"); } },
        { "button3", []() -> QWidget* { return new QPushButton("Yes"); } },
        { "button4", []() -> QWidget* { return new QPushButton("No"); } },
        { "button5", []() -> QWidget* { return new QPushButton("Back"); } },
        { "button6", []() -> QWidget* { return new QPushButton("Next"); } },
        { "button7", []() -> QWidget* { auto *btn = new QToolButton; btn->setText("Tool Time"); return btn; } },
        { "radiobutton0", []() -> QWidget* { auto* btn = new QRadioButton("Radio Gaga");
             btn->setChecked(true);
             return btn;
        } },
        { "radiobutton1", []() -> QWidget* { return new QRadioButton("Radio Gugu"); } },
        { "radiobutton2", []() -> QWidget* { return new QRadioButton("Trolltech"); } },
        { "radiobutton3", []() -> QWidget* { return new QRadioButton("Digia"); } },
        { "radiobutton4", []() -> QWidget* { auto* btn = new QRadioButton("Qt Group");
             btn->setChecked(true);
             return btn;
         } },
        { "radiobutton5", []() -> QWidget* { return new QRadioButton("Nokia"); } },
        { "spinbox0", []() -> QWidget* { auto* box = new QSpinBox; box->setMaximum(100); box->setValue(42); return box; } },
        { "spinbox1", []() -> QWidget* { auto* box = new QDoubleSpinBox; box->setMaximum(10000); box->setValue(3.1459); box->setDecimals(4); box->setSingleStep(0.0001); return box; } },
        { "spinbox2", []() -> QWidget* { auto* box = new QSpinBox; box->setMaximum(100); box->setValue(23); return box; } },
        { "spinbox3", []() -> QWidget* { auto* box = new QDoubleSpinBox; box->setMaximum(10000); box->setValue(2.718); box->setDecimals(4); box->setSingleStep(0.0001); return box; } },
        { "groupbox", []() -> QWidget* { auto* grp = new QGroupBox("Group");
             auto* layout = new QHBoxLayout;
            layout->addWidget(new QRadioButton("Option 1"));
            layout->addWidget(new QRadioButton("Option 2"));
            grp->setLayout(layout);
            return grp;
         } },
        { "wigglywidget", []() -> QWidget* { return new WigglyWidget; } },
        { "analogclock", []() -> QWidget* { return new AnalogClock; } },
        { "lineedit", []() -> QWidget* { return new LineEdit; } },
        { "calendar", []() -> QWidget* { return new QCalendarWidget; } },
        { "slider", []() -> QWidget* { return new Slider; } },
        { "dial", []() -> QWidget* { return new Dial; } },
        { "progressbar", []() -> QWidget* { return new ProgressBar; } },
        { "treeview", []() -> QWidget* { return new TreeView; } },
        { "listview", []() -> QWidget* { return new ListView; } },
        { "tableview", []() -> QWidget* { return new TableView; } },
    };

    for (auto& [name, ctor] : widgets) {
        generator.renderWidgetToFile(name, ctor);
    }

    return EXIT_SUCCESS;
}
