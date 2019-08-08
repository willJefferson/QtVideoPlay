#include "ffmpegplayer.h"

ffmpegPlayer::ffmpegPlayer(QWidget *parent)
    : QWidget(parent)
{
    //this->setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    //videoFrame = new QFrame(this);

    playBtn = new QPushButton(this);
    playBtn->setText(tr("播放"));
    stopBtn = new QPushButton(this);
    stopBtn->setText(tr("停止"));
    openBtn = new QPushButton(this);
    openBtn->setText(tr("打开"));
    backBtn = new QPushButton(this);
    backBtn->setText(tr("快退"));
    advanceBtn = new QPushButton(this);
    advanceBtn->setText(tr("快进"));

    currenttimeLabel = new QLabel(this);
    currenttimeLabel->setText(tr("00:00:00"));
    currenttimeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    finaltimeLabel = new QLabel(this);
    finaltimeLabel->setText(tr("00:00:00"));
    finaltimeLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    lineLabel = new QLabel(this);
    lineLabel->setText(tr("/"));
    lineLabel->setAlignment(Qt::AlignCenter);
    timeLayout = new QHBoxLayout();
    timeLayout->addWidget(currenttimeLabel);
    timeLayout->addWidget(lineLabel);
    timeLayout->addWidget(finaltimeLabel);
    timeLayout->setSpacing(10);
    timeLayout->setMargin(0);
    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(playBtn);
    btnLayout->addWidget(stopBtn);
    btnLayout->addWidget(openBtn);
    btnLayout->addWidget(backBtn);
    btnLayout->addWidget(advanceBtn);
    btnLayout->addLayout(timeLayout);

    sliderLayout = new QHBoxLayout();
    processbar = new PlayerSlider(this);
    processbar->setOrientation(Qt::Horizontal);
    volumebar = new QSlider(this);
    volumebar->setOrientation(Qt::Horizontal);

    volumebar->setRange(0,100);
    volumebar->setValue(20);
    volumeLabel = new QLabel(this);
    volumeLabel->setText(tr("音量"));
    volumeLabel->setAlignment(Qt::AlignRight);
    volumeLayout = new QHBoxLayout();
    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(volumebar);

    sliderLayout->addWidget(processbar);
    sliderLayout->addLayout(volumeLayout);

    sliderLayout->setStretchFactor(processbar,7);

    mainLayout = new QVBoxLayout();
    resize(1000,600);

    m_pPlayer = new QMediaPlayer;
    m_pPlayerWidget = new QVideoWidget;
    m_pPlayer->setVideoOutput(m_pPlayerWidget);

    mainLayout->addWidget(m_pPlayerWidget);
    mainLayout->addLayout(sliderLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->setStretchFactor(m_pPlayerWidget,8);
    mainLayout->setStretchFactor(sliderLayout,1);
    mainLayout->setStretchFactor(btnLayout,1);
    setLayout(mainLayout);

    m_pPlayerWidget->setAutoFillBackground(true);
    QPalette qplte;
    qplte.setColor(QPalette::Window,QColor(0,0,0));
    m_pPlayerWidget->setPalette(qplte);

    connect(openBtn,SIGNAL(clicked()),this,SLOT(OpenPlayFile()));
    connect(playBtn,SIGNAL(clicked()),this,SLOT(BeginPlayFile()));
    connect(stopBtn,SIGNAL(clicked()),this,SLOT(StopPlayFile()));
    connect(backBtn,SIGNAL(clicked()),this,SLOT(BackPlay()));
    connect(advanceBtn,SIGNAL(clicked()),this,SLOT(AdvancePlay()));
    connect(m_pPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(OnStateChanged(QMediaPlayer::State)));

    processbar->setEnabled(false);
    IS_Playing=false;

    connect(m_pPlayer,SIGNAL(positionChanged(qint64)),this,SLOT(OnSlider(qint64)));
    connect(m_pPlayer,SIGNAL(durationChanged(qint64)),this,SLOT(OnDurationChanged(qint64)));
    connect(processbar,SIGNAL(sigProgress(qint64)),this,SLOT(setPositon(qint64)));
    connect(volumebar,SIGNAL(valueChanged(int)),this,SLOT(VolumChange(int)));


}
void ffmpegPlayer::setPositon(qint64 postion)
{
    //m_pPlayer->setPosition(postion*1000);
    m_pPlayer->setPosition(postion);
}

ffmpegPlayer::~ffmpegPlayer()
{
    delete m_pPlayer;
    delete m_pPlayerWidget;
}
void ffmpegPlayer::OpenPlayFile()
{
    QString s = QFileDialog::getOpenFileName(this,"select outputfile","E:/video/","mp4 files(*.mp4)::avi files(*.avi)::mkv files(*.mkv)::All files(*.*)");
    playfilename=s;

    if(s.length()>0)
    {
       m_pPlayer->setMedia(QUrl::fromLocalFile(playfilename));
       //IS_Playing=true;
       processbar->setValue(0);

       m_pPlayer->play();//选择后播放
    }
}
void ffmpegPlayer::OnSlider(qint64 pos)
{
    //processbar->setProgress(pos/1000);
    processbar->setProgress(pos);

    int totalSec =  pos/1000;
    int min = totalSec/60;
    int sec = totalSec%60;
    int hour = min/60;
    min%=60;
    char buf[256]={0};
    sprintf(buf,"%02d:%02d:%02d",hour,min,sec);
    currenttimeLabel->setText(buf);
}
void ffmpegPlayer::OnDurationChanged(qint64 duration)
{
    if(duration>0&&playfilename.length()>0)
    {
        //processbar->setRange(0,duration/1000);
        processbar->setRange(0,duration);
        char buf[256]={0};
        int totalSec =  duration/1000;
        int min = totalSec/60;
        int sec = totalSec%60;
        int hour = min/60;
        min%=60;
        sprintf(buf,"%02d:%02d:%02d",hour,min,sec);

        finaltimeLabel->setText(buf);
    }
}
void ffmpegPlayer::OnStateChanged(QMediaPlayer::State enumState)
{
    if(QMediaPlayer::StoppedState==enumState)
    {
        //playBtn->setEnabled(true);
        playBtn->setText(tr("播放"));
        //stopBtn->setEnabled(false);
        processbar->setValue(0);
        processbar->setEnabled(false);
        IS_Playing=false;
        currenttimeLabel->setText(tr("00:00:00"));
        finaltimeLabel->setText(tr("00:00:00"));
    }
    else if (QMediaPlayer::PlayingState==enumState)
    {
        //playBtn->setEnabled(false);
        playBtn->setText(tr("暂停"));
        //stopBtn->setEnabled(true);
        processbar->setEnabled(true);
        IS_Playing=true;
    }
    else if(QMediaPlayer::PausedState==enumState)
    {
        playBtn->setText(tr("播放"));
        IS_Playing=false;
    }
}
void ffmpegPlayer::BeginPlayFile()
{
    if(playfilename.isEmpty())
    {
        QMessageBox::information(this,"warning","Please select playfile!");
        openBtn->setFocus();
        return;
    }
    else
    {
        if(IS_Playing)
        {
            m_pPlayer->pause();
            //IS_Playing=false;
        }
        else
        {
            m_pPlayer->play();//选择后播放
            //IS_Playing=true;
        }
    }
}

void ffmpegPlayer::StopPlayFile()
{
    if(IS_Playing||processbar->value()>0)
    {
        m_pPlayer->stop();
        return;
    }
}
void ffmpegPlayer::VolumChange(int vol)
{
    m_pPlayer->setVolume(vol);
}
void ffmpegPlayer::BackPlay()
{
    //后退6秒
    qint64 pos = processbar->value();
    if(pos==0)
        return;
    pos = pos>6000?(pos-6000):0;
    m_pPlayer->setPosition(pos);
    processbar->setValue(pos);
}
void ffmpegPlayer::AdvancePlay()
{
    //后退6秒
    qint64 pos = processbar->value();

    qint64 max= m_pPlayer->duration();
    pos = (pos+6000)<max?(pos+6000):max;
    m_pPlayer->setPosition(pos);
    processbar->setValue(pos);
}
