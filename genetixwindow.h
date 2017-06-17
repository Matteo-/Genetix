#ifndef GENETIXWINDOW_H
#define GENETIXWINDOW_H

#include <QMainWindow>

namespace Ui {
class GenetixWindow;
}

class GenetixWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GenetixWindow(QWidget *parent = 0);
    ~GenetixWindow();

private:
    Ui::GenetixWindow *ui;
};

#endif // GENETIXWINDOW_H
