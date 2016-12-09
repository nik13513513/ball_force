#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setWindowTitle(tr("Ball test"));
}

Widget::~Widget()
{
    delete ui;
}

GraphicsView *Widget::view() const
{
    return ui->frame;
}

void Widget::on_shuffleButton_clicked()
{
    ui->frame->shuffle();
}

void Widget::on_forcesCheckBox_toggled(bool _checked)
{
    ui->frame->showForces(_checked);
}

