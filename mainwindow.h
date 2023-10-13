//
// Created by GXZC on 2023/10/9.
//
#include <QMainWindow>
#include <QRubberBand>

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

    QPdfDocument *m_document;

    QRubberBand *rubberBand = nullptr;

    QPoint origin;

};


#endif //OCR_MAINWINDOW_H
