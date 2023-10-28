#include <QApplication>
#include <QtWidgets>
#include <QImage>
#include <QPainter>

struct WidgetGenerator {

    void renderWidgetToFile(QString const& filename, std::function<QWidget*()> f, int steps = 1) {
        std::unique_ptr<QWidget> widget(f());

        auto image = renderWidget(widget.get());
        image.save(QString("%1.png").arg(filename));
    }

    QImage renderWidget(QWidget *widget) const
    {
        qreal dpr = widget->devicePixelRatioF() * 2;
        QImage image(widget->size() * dpr, QImage::Format_ARGB32_Premultiplied);
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
    WidgetGenerator generator;
    std::map<QString, std::function<QWidget*()>> widgets {
        { "checkbox0", []() -> QWidget* { return new QCheckBox("Checked"); } }
    };

    for (auto& [name, ctor] : widgets) {
        generator.renderWidgetToFile(name, ctor);
    }

    return EXIT_SUCCESS;
}
