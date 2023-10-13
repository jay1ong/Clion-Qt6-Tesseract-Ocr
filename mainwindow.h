//
// Created by GXZC on 2023/10/9.
//
#include <QMainWindow>
#include <QRubberBand>
#include <allheaders.h> // leptonica main header for image io
#include <tesseract/baseapi.h> // tesseract main header
#include <opencv2/highgui.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QFileDialog;

class QDialog;

class QPdfDocument;

class QPdfView;

QT_END_NAMESPACE

#ifndef OCR_MAINWINDOW_H
#define OCR_MAINWINDOW_H


class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    PIX* qImage2PIX(const QImage &image);

public slots:

    void open(const QUrl &docLocation);

protected:
    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private slots:

    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;

    QFileDialog *m_fileDialog = nullptr;

    QDialog *m_imageDialog = nullptr;

    QPdfDocument *m_document;

    QRubberBand *rubberBand = nullptr;

    QPoint origin;

};


#endif //OCR_MAINWINDOW_H
