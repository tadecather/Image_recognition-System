#ifndef EXAMPLECOMMAND_H
#define EXAMPLECOMMAND_H

#include "imagecommand.h"
#include <QImage>
#include <QString>
#include "imagegray.h"
#include "mytabwidget.h"

//这是一个command样例类，实现见ExampleCommand.cpp
//下面的声明只能多不能少
class ExampleCommand : public ImageCommand
{
private:
    int index;
    MyTabWidget* mainTab;
public:
    ExampleCommand(QImage* imageLeft, QImage* imageRight, MyTabWidget* mainTab, int index);
    void undo();
    void redo();
};

#endif // EXAMPLECOMMAND_H