#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QFileDialog>

QImage blurImage(QImage source, int blurRadius) {
    if(source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));
    auto *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurRadius);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0, 0, source.width(), source.height()));
    return result;
}

QImage setImage(QImage source) {
    if(source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0, 0, source.width(), source.height()));
    return result;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    auto *window = new QWidget;
    window->resize(650,500);
    auto *layOut = new QVBoxLayout(window);
    auto *pic = new QLabel;
    auto *slider = new QSlider(Qt::Horizontal);
    auto *button = new QPushButton("open");
    QString filePath;
    QImage sourceImage;

    slider->setMinimum(0);
    slider->setMaximum(16);

    layOut->addWidget(pic);
    layOut->addWidget(slider);
    layOut->addWidget(button);

    QObject::connect(button,&QPushButton::clicked,[&filePath, &sourceImage, &pic](){
        filePath = QFileDialog::getOpenFileName(nullptr, "Open", "../myBlureWidget/images/", "jpg files (*.jpg)");
        sourceImage = setImage(QImage(filePath));
        pic->setPixmap(QPixmap::fromImage(sourceImage).scaled(pic->width(),pic->height(),Qt::KeepAspectRatio));
    });

    QObject::connect(slider,&QSlider::valueChanged,[&sourceImage, &pic](int val){
        QImage buf(sourceImage);
        buf = blurImage(buf,val);
        pic->setPixmap(QPixmap::fromImage(buf).scaled(pic->width(),pic->height(),Qt::KeepAspectRatio));
    });

    window->show();
    return a.exec();
}
