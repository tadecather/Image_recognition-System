#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <QImage>
#include <QVector>

class ImageSegmentation
{
public:
    static QImage * ostu(QImage &image);
};

#endif // SEGMENTATION_H