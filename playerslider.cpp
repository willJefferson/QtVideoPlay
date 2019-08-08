#include "playerslider.h"

PlayerSlider::PlayerSlider(QWidget *parent):QSlider(parent)
{
    m_bPressed=false;
}
void PlayerSlider::mousePressEvent(QMouseEvent *e)
{
    m_bPressed=true;
    QSlider::mousePressEvent(e);
}
void PlayerSlider::mouseMoveEvent(QMouseEvent *e)
{
    QSlider::mouseMoveEvent(e);
}
void PlayerSlider::mouseReleaseEvent(QMouseEvent *e)
{
    m_bPressed=false;
    qint64 pos = value();
    emit sigProgress(pos);
}

void PlayerSlider::setProgress(qint64 progress_64)
{
    if(!m_bPressed)
        setValue(progress_64);
}
