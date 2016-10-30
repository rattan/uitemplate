#include "dialog.h"
#include "ui_dialog.h"

QVariant IntegerInterpolator(const int &start, const int &end, qreal progress)
{
    return start*(1-progress) + end*progress;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if(m_Cache != NULL)
    {
        QPainter painter(this);

        painter.drawPixmap(0, 0, *m_Cache);
    }
}

void Dialog::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    m_Titlebar->resize(event->size());

    draw_window();
}

void Dialog::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        if(this->windowState() == Qt::WindowNoState)
        {
            this->m_Titlebar->Minimized();
        }
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->pos().y()<33)
    {
        if(title_ready)
        {
        ani_title->setStartValue(0);
        ani_title->setEndValue(32);
        this->m_Titlebar->ShowTitle();
        ani_title->start();
        }
    }
    else
    {
        title_ready = true;
    }
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_SizeGrip(this),
    m_Cache(nullptr),
    top(0)
{
    ui->setupUi(this);

    //
    setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(200,200);
    m_Titlebar = new WindowTitleBar(this);

    connect(this, SIGNAL(WindowTitleChanged()),this->m_Titlebar, SLOT(UpdateWindowTitle()));
    connect(this->m_Titlebar,SIGNAL(ShowSizeGrip(const bool)),this,SLOT(ShowSizeGrip(const bool)));
    connect(this->m_Titlebar,SIGNAL(hide_title()),this,SLOT(hide_title()));
    connect(this->m_Titlebar,SIGNAL(titleReady(bool)),this,SLOT(titleReady(bool)));
    setWindowTitle(QString("uitemplate"));
    m_SizeGrip.resize(16, 16);

    setAttribute(Qt::WA_TranslucentBackground);

    qRegisterAnimationInterpolator<int>(IntegerInterpolator);
    ani_title = new QPropertyAnimation(this,nullptr);
    connect(ani_title,SIGNAL(valueChanged(QVariant)),this,SLOT(ani_title_vc(QVariant)));
    ani_title->setDuration(300);
    ani_title->setEasingCurve(QEasingCurve::OutExpo);

    title_ready = true;
    //
}

Dialog::~Dialog()
{
    if(m_Cache) delete m_Cache;
    delete ui;
}

void Dialog::setWindowTitle(const QString &title)
{
    QWidget::setWindowTitle(title);
    emit WindowTitleChanged();
}

void Dialog::draw_window()
{
    if(m_Cache) delete m_Cache;

    m_Cache = new QPixmap(size());

    m_Cache->fill(Qt::transparent);

    QPainter painter(m_Cache);

    QColor background_color(231,198,165,250);
    /********** Window's background **********/
    QPolygon background;

    background << QPoint(      0,       top)
               << QPoint(width(),       top)
               << QPoint(width(), height())
               << QPoint(      0, height());

    painter.setPen  (Qt::NoPen);
    painter.setBrush(QBrush(background_color));

    painter.drawPolygon(background);
    /*****************************************/

    m_SizeGrip.move  (width() - 16, height() - 16);
}

void Dialog::ShowSizeGrip(const bool show)
{
    if(show)
    {
        this->m_SizeGrip.show();
    }
    else
    {
        this->m_SizeGrip.hide();
    }
}

void Dialog::hide_title()
{
    ani_title->setStartValue(32);
    ani_title->setEndValue(0);
    ani_title->start();
}

void Dialog::ani_title_vc(QVariant val)
{
    this->top = val.value<int>();
    draw_window();
    update();
}

void Dialog::titleReady(const bool &ready)
{
    title_ready = ready;
}
