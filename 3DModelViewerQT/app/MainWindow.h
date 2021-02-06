#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../engine/SceneManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MaterialPtr createTexturedMaterial() const;
    MaterialPtr createColoredMaterial(const QColor& color) const;

private:
    Ui::MainWindow *ui;

    int m_counter = 0;
};

#endif // MAINWINDOW_H
