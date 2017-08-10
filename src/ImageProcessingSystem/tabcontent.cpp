#include "tabcontent.h"

TabContent::TabContent(QWidget *parent, QImage *image) : QWidget(parent)
{
    this->imageDisplayL = new ImageDisplay(this, image);
    this->imageDisplayR = new ImageDisplay(this);

    //当前聚焦的imagedispy，0左1右
    focusOn = 0;

    connect(imageDisplayL, &ImageDisplay::focusOnSignal, this, &TabContent::changeFocusImageDisplaySlot);
    connect(imageDisplayR, &ImageDisplay::focusOnSignal, this, &TabContent::changeFocusImageDisplaySlot);

    QGridLayout *layout = new QGridLayout();

    layout->addWidget(imageDisplayL, 0, 0, 1, 1);
    layout->addWidget(imageDisplayR, 0, 1, 1, 1);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);

    this->setLayout(layout);
}

TabContent::~TabContent()
{
    if(this->imageDisplayL != NULL)
        imageDisplayL->deleteLater();
    if(this->imageDisplayR != NULL)
        imageDisplayR->deleteLater();
}

ImageDisplay* TabContent::getImageDisplayL()
{
    return this->imageDisplayL;
}

ImageDisplay* TabContent::getImageDisplayR()
{
    return this->imageDisplayR;
}

//获取当前聚焦的ImageDisplay指针
ImageDisplay* TabContent::getFocusedImageDisplay()
{
    if(focusOn == 0)
        return imageDisplayL;
    else
        return imageDisplayR;
}

//槽函数，改变当前聚焦的imagedisplay对象
void TabContent::changeFocusImageDisplaySlot()
{
    ImageDisplay* sender = (ImageDisplay*)QObject::sender();
    //左边imagedisplay聚焦
    if(sender == imageDisplayL)
    {
        this->focusOn = 0;
        imageDisplayL->setFocusBorder(1);
        imageDisplayR->setFocusBorder(0);
    }
    //右边imagedisplay聚焦
    else
    {
        this->focusOn = 1;
        imageDisplayL->setFocusBorder(0);
        imageDisplayR->setFocusBorder(1);
    }
}

int TabContent::getFocus()
{
    return this->focusOn;
}
