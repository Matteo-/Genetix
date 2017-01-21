#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void start_clicked();
    void stop_clicked();
    void delay_changed(int);

public slots:
    void changedGen(int gen);
    void on_start_clicked();
    void on_stop_clicked();
    void on_delay_changed(int);

private:
    QLCDNumber *lcdGen;
};

#endif // MAINWINDOW_H
