//
// Created by GXZC on 2023/10/12.
//

#include <QDialog>

#include <QRubberBand>

#include <QBuffer>
#include <QLabel>
#include <QVBoxLayout>

#include <allheaders.h> // leptonica main header for image io
#include <tesseract/baseapi.h> // tesseract main header
#include <opencv2/highgui.hpp>

#ifndef OCR_OCR_H
#define OCR_OCR_H


class Ocr : public QDialog {
    Q_OBJECT
public:
    explicit Ocr(QWidget *parent, const QPixmap& pix);

    ~Ocr() override;

private:
    static PIX* qImage2PIX(const QImage &image);

    tesseract::TessBaseAPI *m_api = nullptr;

    QLabel *m_label = nullptr;

    QVBoxLayout *m_layout = nullptr;

};


#endif //OCR_OCR_H
