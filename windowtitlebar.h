#ifndef WINDOWTITLEBAR_H
#define WINDOWTITLEBAR_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QGraphicsBlurEffect>
#include <QScreen>
#include "windowbutton.h"
#include "myprop.h"

#include <QDebug>
#include <QPropertyAnimation>

class WindowTitleBar : public QWidget
{
    Q_OBJECT

    QPixmap *m_Cache;
    QLabel m_Title;
    QLabel m_Blur;
    QPoint m_Diff;

    WindowButton m_Minimize;
    WindowButton m_Maximize;
    WindowButton m_Close;

    bool grab_titlebar;
    bool in_titlebar;

    bool maximize;
    bool minimize;
    bool show_title;

    QGraphicsBlurEffect blur_effect;

    QPropertyAnimation *ani_min,*ani_max,*ani_close,*ani_show;
    QRect mem_geo;

    QBrush WindowBrush;

    void draw_window();

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent (QPaintEvent  *event);
    void mousePressEvent  (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent   (QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void leaveEvent(QEvent*event);

public:
    explicit WindowTitleBar(QWidget *parent = 0);
    ~WindowTitleBar();

    void setWindowBrush(const QBrush &brush);
    void _hide();


signals:
    void ShowSizeGrip(const bool show);
    void hide_title();
    void titleReady(const bool&ready);

public slots:
    void UpdateWindowTitle();

    void Minimized();
    void Maximized();
    void Quit     ();

    void release_mouse();

    void ani_min_fin();
    void ani_max_fin();
    void ani_close_fin();

    void ShowTitle();


    void ani_show_vc(QVariant val);
    void ani_show_fin();
};

#endif // WINDOWTITLEBAR_H
