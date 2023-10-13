//
// Created by GXZC on 2023/10/9.
//

#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPdfDocument>
#include <QStandardPaths>
#include <QMouseEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

#include <iostream>
#include <memory>

#include <QBuffer>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          m_document(new QPdfDocument) {
    ui->setupUi(this);
    ui->toolBar->setMovable(false);
    QPdfView *pdfView = ui->pdfView;
    pdfView->setDocument(m_document);
    pdfView->setDocument(m_document);
    QPalette palette = pdfView->palette();
    palette.setBrush(QPalette::Dark, QColor("white"));
    pdfView->setPalette(palette);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::open(const QUrl &docLocation) {
    if (docLocation.isLocalFile()) {
        m_document->load(docLocation.toLocalFile());
        const auto documentTitle = m_document->metaData(QPdfDocument::MetaDataField::Title).toString();
        setWindowTitle(!documentTitle.isEmpty() ? docLocation.toLocalFile() : QStringLiteral("PDF Viewer"));
    } else {
        const QString message = tr("%1 is not a valid local file").arg(docLocation.toString());
        QMessageBox::critical(this, tr("Failed to open"), message);
    }
}

void MainWindow::on_actionOpen_triggered() {
    if (m_fileDialog == nullptr) {
        m_fileDialog = new QFileDialog(this, tr("选择一个pdf文件"),
                                       QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        m_fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
        m_fileDialog->setMimeTypeFilters({"application/pdf"});
    }

    if (m_fileDialog->exec() == QDialog::Accepted) {
        const QUrl toOpen = m_fileDialog->selectedUrls().constFirst();
        if (toOpen.isValid())
            open(toOpen);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    rubberBand->hide();

    auto imageLabel = new QLabel(this);
    auto pixmap = QWidget::grab(rubberBand->geometry());
    imageLabel->setPixmap(pixmap);

    char *outText;

    auto *api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "chi_sim")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    api->SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
    api->SetVariable("save_best_choices", "T");

    // Open input image with leptonica library
    Pix *image = qImage2PIX(pixmap.toImage());

    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();

    auto *imageDialog = new QDialog;
    imageDialog->setWindowTitle("识别结果及录入字段");
    auto *layout = new QVBoxLayout;
    auto label = new QLabel(QString::fromUtf8(outText));
    label->adjustSize();
    layout->addWidget(label);
    imageDialog->setLayout(layout);
    imageDialog->exec();

    // Destroy used object and release memory
    api->End();
    delete api;
    delete[] outText;
    pixDestroy(&image);

    // determine selection, for example using QRect::intersects()
    // and QRect::contains().
}

PIX* MainWindow::qImage2PIX(const QImage &qImage) {
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    qImage.save(&buf, "BMP");
    return pixReadMemBmp(reinterpret_cast<const l_uint8 *>(ba.constData()), ba.size());
}
