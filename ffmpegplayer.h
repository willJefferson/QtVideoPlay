#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QVideoWidget>
#include <QtMultimedia>
#include <QMessageBox>
#include "playerslider.h"
#include <QtDebug>
class ffmpegPlayer : public QWidget
{
    Q_OBJECT

public:
    ffmpegPlayer(QWidget *parent = 0);
    ~ffmpegPlayer();
public slots:
    void OpenPlayFile();
    void OnSlider(qint64);
    void OnDurationChanged(qint64);
    void OnStateChanged(QMediaPlayer::State);
    void BeginPlayFile();
    void StopPlayFile();
    void VolumChange(int vol);
    void BackPlay();
    void AdvancePlay();
    void setPositon(qint64 postion);
private:
    QFrame *videoFrame;
    PlayerSlider *processbar;
    QSlider *volumebar;
    QPushButton *playBtn,*stopBtn,*openBtn;
    QPushButton *backBtn,*advanceBtn;
    QVBoxLayout *mainLayout;
    QHBoxLayout *sliderLayout,*btnLayout;
    QLabel *volumeLabel;
    QLabel *currenttimeLabel,*finaltimeLabel,*lineLabel;

    QHBoxLayout *volumeLayout,*timeLayout;

    QString playfilename;
    bool IS_Playing;
    //调用系统播放器
    QVideoWidget *m_pPlayerWidget;
    QMediaPlayer *m_pPlayer;
};

#endif // FFMPEGPLAYER_H
