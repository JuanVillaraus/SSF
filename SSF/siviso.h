#ifndef SIVISO_H
#define SIVISO_H

#include <QMainWindow>

#define DAY_STYLE 0
#define DUSK_STYLE 1
#define NIGHT_STYLE 2

namespace Ui {
class siviso;
}

class siviso : public QMainWindow
{
    Q_OBJECT

public:
    explicit siviso(QWidget *parent = 0);
    ~siviso();

    QApplication *m_pApplication;

    void changeStyleSheet(int iStyle);

private:
    Ui::siviso *ui;
};

#endif // SIVISO_H
