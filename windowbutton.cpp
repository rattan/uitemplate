#include "windowbutton.h"

#include <QPainter>

WindowButton::WindowButton(ButtonType type, QWidget *parent) : QAbstractButton(parent),
  m_Type   (type        ),
  m_State  (STATE_NORMAL),
  m_Normal (NULL        ),
  m_Hovered(NULL        ),
  m_Clicked(NULL        ),
  c_hover(QColor(50,50,50,200)),
  c_click(QColor(0,178,238,200)),
  c_click_x(QColor(255,48,48))
{
}

WindowButton::~WindowButton()
{
  delete m_Normal ;
  delete m_Hovered;
  delete m_Clicked;
}

void WindowButton::resizeEvent(QResizeEvent *event)
{
  Q_UNUSED(event);

  InitPixmaps();
}

void WindowButton::paintEvent(QPaintEvent  *event)
{
  Q_UNUSED(event);

  QPainter painter(this);

  if(isEnabled())
  {
    switch(m_State)
    {
    case STATE_NORMAL:
      if(m_Normal  != NULL) painter.drawPixmap(0, 0, *m_Normal );
      break;
    case STATE_HOVERED:
      if(m_Hovered != NULL) painter.drawPixmap(0, 0, *m_Hovered);
      break;
    case STATE_CLICKED:
      if(m_Clicked != NULL) painter.drawPixmap(0, 0, *m_Clicked);
      break;
    }
  }
  else
  {
    if(m_Normal != NULL) painter.drawPixmap(0, 0, *m_Normal);
  }
}

void WindowButton::InitPixmaps()
{
  // Delete previous button
  InitPixmap(&m_Normal );
  InitPixmap(&m_Hovered);
  InitPixmap(&m_Clicked);

  switch(m_Type)
  {
  case BUTTON_MINIMIZE:
    InitMinimize();
    break;
  case BUTTON_MAXIMIZE:
    InitMaximize();
    break;
  case BUTTON_CLOSE:
    InitClose();
    break;
  }
}

void WindowButton::InitPixmap(QPixmap **pixmap)
{
  delete *pixmap;

  *pixmap = new QPixmap(size());

  (*pixmap)->fill(Qt::transparent);
}

void WindowButton::InitMinimize()
{
    QBrush symbol_brush;
    symbol_brush.setColor(c_hover);
    symbol_brush.setStyle(Qt::SolidPattern);

  /********** Button's border **********/
  QPolygon border;

  border << QPoint(          0,            0)
         << QPoint(width() - 1,            0)
         << QPoint(width() - 1, height() - 1)
         << QPoint(          0, height() - 1);
  /*************************************/

  /********** Button's symbol **********/
  QPolygon symbol;

  symbol << QPoint(width()*0.35, height()*0.65)
         << QPoint(width()*0.35, height()*0.55)
         << QPoint(width()*0.65, height()*0.55)
         << QPoint(width()*0.65, height()*0.65);
  /*************************************/

  QPainter painter;

  /********** Normal **********/
  painter.begin(m_Normal);

  painter.setPen  (Qt::NoPen        );
  painter.setBrush(symbol_brush);

  painter.drawPolygon(symbol);

  painter.end();
  /****************************/

  symbol_brush.setColor(Qt::white);
  /********** Hovered **********/
  painter.begin(m_Hovered);

  painter.setPen  (Qt::NoPen);
  painter.setBrush(QBrush(c_hover));

  painter.drawPolygon(border);

  painter.setPen  (Qt::NoPen        );
  painter.setBrush(symbol_brush);

  painter.drawPolygon(symbol);

  painter.end();
  /*****************************/

  /********** Clicked **********/
  painter.begin(m_Clicked);

  painter.setPen  (Qt::NoPen);
  painter.setBrush(QBrush(c_click));

  painter.drawPolygon(border);

  painter.setPen  (Qt::NoPen        );
  painter.setBrush(symbol_brush);

  painter.drawPolygon(symbol);

  painter.end();
  /*****************************/
}

void WindowButton::InitMaximize()
{
  /********** Button's border **********/
  QPolygon border;
  QPen symbol_pen;
  symbol_pen.setColor(c_hover);
  symbol_pen.setJoinStyle(Qt::MiterJoin);
  symbol_pen.setWidth(height()/15);

  border << QPoint(          0,            0)
         << QPoint(width() - 1,            0)
         << QPoint(width() - 1, height() - 1)
         << QPoint(          0, height() - 1);
  /*************************************/

  /********** Button's symbol **********/
  QPolygon symbol;

  symbol << QPoint(width()*0.35, height()*0.35)
         << QPoint(width()*0.35, height()*0.65)
         << QPoint(width()*0.65, height()*0.65)
         << QPoint(width()*0.65, height()*0.35);

  /*************************************/

  QPainter painter;

  /********** Normal **********/
  painter.begin(m_Normal);

  painter.setPen  (symbol_pen);
  painter.setBrush(Qt::NoBrush    );

  painter.drawPolygon(symbol);

  painter.end();
  /****************************/

  /********** Hovered **********/
  painter.begin(m_Hovered);

  painter.setPen  (Qt::NoPen);
  painter.setBrush(QBrush(c_hover));

  painter.drawPolygon(border);

  symbol_pen.setColor(Qt::white);
  painter.setPen  (symbol_pen);
  painter.setBrush(Qt::NoBrush);

  painter.drawPolygon(symbol);

  painter.end();
  /*****************************/

  /********** Clicked **********/
  painter.begin(m_Clicked);

  painter.setPen  (Qt::NoPen);
  painter.setBrush(QBrush(c_click));

  painter.drawPolygon(border);

  painter.setPen  (symbol_pen);
  painter.setBrush(Qt::NoBrush);

  painter.drawPolygon(symbol);

  painter.end();
  /*****************************/
}

void WindowButton::InitClose()
{
  /********** Button's border **********/
  QPolygon border;

  border << QPoint(          0,            0)
         << QPoint(width() - 1,            0)
         << QPoint(width() - 1, height() - 1)
         << QPoint(          0, height() - 1);
  /*************************************/

  /********** Button's symbol **********/
  QLine symbol1(QPoint(width()*0.35, height()*0.35), QPoint(width()*0.65, height()*0.65));
  QLine symbol2(QPoint(width()*0.35, height()*0.65), QPoint(width()*0.65, height()*0.35));
  /*************************************/

  QPainter painter;

  /********** Normal **********/
  painter.begin(m_Normal);

  painter.setPen(QPen(QBrush(c_hover), height()/15));

  painter.drawLine(symbol1);
  painter.drawLine(symbol2);

  painter.end();
  /****************************/

  /********** Hovered **********/
  painter.begin(m_Hovered);

  painter.setPen  (Qt::NoPen);
  painter.setBrush(QBrush(c_hover));

  painter.drawPolygon(border);

  painter.setPen(QPen(QBrush(Qt::white), height()/15));

  painter.drawLine(symbol1);
  painter.drawLine(symbol2);

  painter.end();
  /*****************************/

  /********** Clicked **********/
  painter.begin(m_Clicked);

  painter.setPen  (Qt::NoPen);
  painter.setBrush(QBrush(c_click_x));

  painter.drawPolygon(border);

  painter.setPen(QPen(QBrush(Qt::white), height()/15));

  painter.drawLine(symbol1);
  painter.drawLine(symbol2);

  painter.end();
  /*****************************/
}

void WindowButton::enterEvent(QEvent *event)
{
  Q_UNUSED(event);

  m_State = STATE_HOVERED;

  update();
}

void WindowButton::leaveEvent(QEvent *event)
{
  Q_UNUSED(event);

  m_State = STATE_NORMAL;

  update();
}

void WindowButton::mousePressEvent(QMouseEvent *event)
{
  QAbstractButton::mousePressEvent(event);

  m_State = STATE_CLICKED;

  update();
  emit title_mouse_release();
}

void WindowButton::mouseReleaseEvent(QMouseEvent *event)
{
  QAbstractButton::mouseReleaseEvent(event);

  if(underMouse()) m_State = STATE_HOVERED;
  else             m_State = STATE_NORMAL;

  update();
}
