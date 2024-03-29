#ifndef PLAYERSLIDER_H
#define PLAYERSLIDER_H

#include <QSlider>
#include <QMouseEvent>
class PlayerSlider : public QSlider
{
    Q_OBJECT
public:
    PlayerSlider(QWidget *parent=0);
    void setProgress(qint64);
signals:
    void sigProgress(qint64);
private:
    bool m_bPressed;
protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
};

#endif // PLAYERSLIDER_H
