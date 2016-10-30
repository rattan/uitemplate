#include "windowtitlebar.h"

void WindowTitleBar::draw_window()
{
    if(m_Cache) delete m_Cache; // Remove old cache

    m_Cache = new QPixmap(size());  // Create a cache with same size as the widget

    m_Cache->fill(Qt::transparent);  // Create a the transparent background



    QPainter painter(m_Cache); // Start painting the cache

    QColor border_color    (102, 204, 204, 200);
    QColor gradientStart(  255,   255,   255,   200);
    QColor gradientEnd  (  0,   0,   0, 200);

    QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, height()));
    linearGrad.setColorAt(0, gradientStart);
    linearGrad.setColorAt(1, gradientEnd);

    /********** Title bar's frame **********/
    QPolygon frame;

    frame << QPoint(      0,  0)
          << QPoint(width(), 0)
          << QPoint(width(), 32)
          << QPoint(      0, 32);

    painter.setPen  (Qt::NoPen);
    painter.setBrush(WindowBrush);

    painter.drawPolygon(frame);
    /***************************************/

    /********** Title bar's buttons area **********/


    QLinearGradient btngrad(QPointF(0, 0), QPointF(120, 0));
    btngrad.setColorAt(0, QColor(255,255,255,WindowBrush.color().alpha()));
    btngrad.setColorAt(1, QColor(0,0,0,0));

    QPolygon buttons;

    buttons << QPoint(0, 0)
            << QPoint(0, 32)
            << QPoint(120, 32)
            << QPoint(120,0);

    painter.setPen  (Qt::NoPen);
    painter.setBrush(QBrush(btngrad));

    painter.drawPolygon(buttons);
    /**********************************************/

    QString str = QString("color: rgba(255, 255, 255, %1); font-family: Sans; font-weight: bold; font-size: 14px;");
    int alpha = WindowBrush.color().alpha()*1.7;

    m_Title.setStyleSheet(str.arg(alpha));
    m_Title.resize(width()-116,29);
    m_Title.move(width()/2,2);


}

void WindowTitleBar::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    draw_window();
}

void WindowTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(m_Cache != NULL)
    {
        QPainter painter(this);

        painter.drawPixmap(0, 0, *m_Cache);
    }
}

void WindowTitleBar::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_Diff = event->pos();
        grab_titlebar = true;
    }
    else
    {
        _hide();
        emit titleReady(false);
    }
}

void WindowTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    grab_titlebar = false;
}

void WindowTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(grab_titlebar)
    {
        if(maximize) emit Maximized();
        QPoint p = event->globalPos();

        window()->move(p - m_Diff);
    }
}

void WindowTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit Maximized();
    }
}

void WindowTitleBar::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    _hide();

}

WindowTitleBar::WindowTitleBar(QWidget *parent) :
    QWidget(parent),
    m_Cache(nullptr),
    m_Title(this),
    m_Minimize(WindowButton::BUTTON_MINIMIZE,this),
    m_Maximize(WindowButton::BUTTON_MAXIMIZE,this),
    m_Close(WindowButton::BUTTON_CLOSE,this),
    WindowBrush(QColor(50,50,50,150))
{
    setFixedHeight(33);


    connect(&m_Minimize,SIGNAL(clicked()),this,SLOT(Minimized()));
    connect(&m_Minimize,SIGNAL(title_mouse_release()),this,SLOT(release_mouse()));

    connect(&m_Maximize,SIGNAL(clicked()),this,SLOT(Maximized()));
    connect(&m_Maximize,SIGNAL(title_mouse_release()),this,SLOT(release_mouse()));

    connect(&m_Close,SIGNAL(clicked()),this,SLOT(Quit()));
    connect(&m_Close,SIGNAL(title_mouse_release()),this,SLOT(release_mouse()));

    m_Minimize.resize(33, 33);
    m_Minimize.move(0,  0);
    m_Minimize.setToolTip(QString("최소화"));

    m_Maximize.resize(33, 33);
    m_Maximize.move(33,  0);
    m_Maximize.setToolTip(QString("최대화"));

    m_Close.resize(33, 33);
    m_Close.move(66,  0);
    m_Close.setToolTip(QString("닫기"));


    grab_titlebar = in_titlebar = maximize = minimize = false;
    show_title = true;

    ani_min=new QPropertyAnimation(this->parentWidget(),"windowOpacity");
    ani_min->setDuration(300);
    ani_min->setEasingCurve(QEasingCurve::InOutExpo);
    connect(ani_min,SIGNAL(finished()),this,SLOT(ani_min_fin()));

    ani_max=new QPropertyAnimation(this->parentWidget(),"geometry");
    ani_max->setEasingCurve(QEasingCurve::InOutExpo);
    ani_max->setDuration(300);
    connect(ani_max,SIGNAL(finished()),this,SLOT(ani_max_fin()));

    ani_close=new QPropertyAnimation(this->parentWidget(),"geometry");
    ani_close->setDuration(300);
    ani_close->setEasingCurve(QEasingCurve::InOutExpo);
    connect(ani_close,SIGNAL(finished()),this,SLOT(ani_close_fin()));

    //    qRegisterAnimationInterpolator<QBrush>(myColorInterpolator);
    ani_show = new QPropertyAnimation(this,"geometry");
    ani_show->setDuration(300);
    ani_show->setEasingCurve(QEasingCurve::OutExpo);
    //    connect(ani_show,SIGNAL(valueChanged(QVariant)),this,SLOT(ani_show_vc(QVariant)));
    connect(ani_show,SIGNAL(finished()),this,SLOT(ani_show_fin()));

    _hide();
}

WindowTitleBar::~WindowTitleBar()
{
    if(m_Cache) delete m_Cache;
}

void WindowTitleBar::setWindowBrush(const QBrush &brush)
{
    this->WindowBrush = brush;
    draw_window();
}

void WindowTitleBar::_hide()
{
    emit hide_title();
    show_title = false;
    in_titlebar = false;
    ani_show->stop();
    ani_show->setStartValue(geometry());
    ani_show->setEndValue(QRect(0,-33,parentWidget()->width(),parentWidget()->height()));
    ani_show->start();
}

void WindowTitleBar::UpdateWindowTitle()
{
    m_Title.setText(window()->windowTitle());
}

void WindowTitleBar::Minimized()
{
    if(minimize)
    {
        ani_min->setStartValue(0);
        ani_min->setEndValue(1);
        minimize = false;
    }
    else
    {
        ani_min->setStartValue(1);
        ani_min->setEndValue(0);
        minimize = true;
    }

    ani_min->start();
}

void WindowTitleBar::Maximized()
{
    ani_max->setStartValue(parentWidget()->geometry());

    if(maximize)
    {
        ani_max->setEndValue(mem_geo);
        maximize = false;

    }
    else
    {
        mem_geo = parentWidget()->geometry();
        QRect full = QApplication::desktop()->screenGeometry(QApplication::desktop()->screenNumber(this->parentWidget()));
        full.setY(full.y()-1);
        ani_max->setEndValue(full);
        maximize = true;
    }
    ani_max->start();
}

void WindowTitleBar::Quit()
{
    mem_geo = this->parentWidget()->geometry();
    this->parentWidget()->setMinimumHeight(1);

    ani_close->setStartValue(mem_geo);
    ani_close->setEndValue(QRect(mem_geo.x(),mem_geo.y(),mem_geo.width(),1));
    ani_close->start();
}

void WindowTitleBar::release_mouse()
{
    mouseReleaseEvent(new QMouseEvent(QEvent::MouseButtonRelease,QCursor::pos(),Qt::LeftButton,Qt::NoButton,Qt::NoModifier));
}

void WindowTitleBar::ani_min_fin()
{
    if(minimize)
    {
        this->parentWidget()->setWindowOpacity(1);
        window()->showMinimized();
    }
}

void WindowTitleBar::ani_max_fin()
{
    emit ShowSizeGrip(!maximize);
}

void WindowTitleBar::ani_close_fin()
{
    qApp->quit();
}

void WindowTitleBar::ShowTitle()
{
    //    this->show();
    show_title = true;
    ani_show->stop();
    ani_show->setStartValue(geometry());
    ani_show->setEndValue(QRect(0,0,parentWidget()->width(),parentWidget()->height()));
    ani_show->start();
}

void WindowTitleBar::ani_show_vc(QVariant val)
{
    this->WindowBrush = val.value<QBrush>();
    draw_window();
    update();
}

void WindowTitleBar::ani_show_fin()
{
    if(show_title)
    {
        if(QCursor::pos().y()-parentWidget()->pos().y()<0 || QCursor::pos().y()-parentWidget()->pos().y()>33)
        {
            _hide();
        }
    }
    else
    {
    }
}
