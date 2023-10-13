//
// Created by GXZC on 2023/10/12.
//

#include <QBuffer>
#include <QLabel>
#include <QVBoxLayout>
#include "ocr.h"
#include <QDialog>

Ocr::Ocr(QWidget *parent, const QPixmap& pix) : QDialog(parent)
{
    if(m_api == nullptr){
        m_api = new tesseract::TessBaseAPI();
        // Initialize tesseract-ocr with English, without specifying tessdata path
        if (m_api->Init(nullptr, "chi_sim")) {
            fprintf(stderr, "Could not initialize tesseract.\n");
            exit(1);
        }
        m_api->SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
        m_api->SetVariable("save_best_choices", "T");
    }

    // Open input image with leptonica library
    m_api->SetImage(qImage2PIX(pix.toImage()));
    // Get OCR result
    setWindowTitle("识别结果及录入字段");
    if(m_label == nullptr){
        m_label = new QLabel(QString::fromUtf8(m_api->GetUTF8Text()));
        m_label->adjustSize();
    }else{
        m_label->setText(QString::fromUtf8(m_api->GetUTF8Text()));
    }
    if (m_layout == nullptr){
        m_layout = new QVBoxLayout;
        m_layout->addWidget(m_label);
        setLayout(m_layout);
    }
}

Ocr::~Ocr()
{
    // Destroy used object and release memory
    m_api->End();
    delete m_label;
    delete m_layout;
    delete m_api;
}

PIX* Ocr::qImage2PIX(const QImage &qImage) {
    QByteArray ba;
    QBuffer buf(&ba);
    buf.open(QIODevice::WriteOnly);
    qImage.save(&buf, "BMP");
    return pixReadMemBmp(reinterpret_cast<const l_uint8 *>(ba.constData()), ba.size());
}