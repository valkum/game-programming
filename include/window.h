#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    MainWindow *mainWindow;
};

#endif
