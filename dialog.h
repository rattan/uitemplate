#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
//
#include <QGridLayout>
#include <QSizeGrip>
#include <QTimer>
#include "windowtitlebar.h"
//

#include<QDebug>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void changeEvent(QEvent*event);
    void mouseMoveEvent(QMouseEvent*event);



public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    //
    void setWindowTitle(const QString &title);
    //

private:
    Ui::Dialog *ui;

    //
    WindowTitleBar *m_Titlebar;
    QSizeGrip m_SizeGrip;
    QPixmap *m_Cache;

    QPropertyAnimation *ani_title;
    int top;
    bool title_ready;

    void draw_window();
private slots:
    void ShowSizeGrip(const bool show);
    void hide_title();
    void ani_title_vc(QVariant);
    void titleReady(const bool&ready);


    //
signals:
    void WindowTitleChanged();
    //

};

#endif // DIALOG_H
