#pragma once

#include <QWidget>

namespace Ui {
    class Widget;
}

class GraphicsView;

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    GraphicsView* view()const;

private slots:
    void on_shuffleButton_clicked();
    void on_forcesCheckBox_toggled(bool _checked);

private:
    Ui::Widget *ui;
};
