#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyle>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //          Buttons style
    ui->pushButtonAdd->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));
    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButtonPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    //          Player init
    m_player = new QMediaPlayer();
    m_player->setVolume(70);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
    ui->horizontalSliderVolume->setValue(m_player->volume());

    //           Class work
}

MainWindow::~MainWindow()
{
    delete m_player;
    delete ui;
}


void MainWindow::on_pushButtonAdd_clicked()
{
    QString file = QFileDialog::getOpenFileName
    (
      this,
      "Open file",
      "",
      "Audio files (*.mp3 *.flac);; MP-3 (*.mp3);; Flac (*.flac)"
    );

    ui->labelFileName->setText(QString("File: ").append(file));
    this->m_player->setMedia(QUrl(file));

    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::updateSlider);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::setSliderRange);
    ui->horizontalSliderTime->setMaximum(static_cast<int>(m_player->duration()));


}


void MainWindow::on_pushButtonPlay_clicked()
{
    this->m_player->play();
}


void MainWindow::on_horizontalSliderVolume_valueChanged(int value)
{
    m_player->setVolume(value);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));
}

void MainWindow::updateSlider(qint64 position)
{
    ui->horizontalSliderTime->setValue(static_cast<int>(position));
    ui->labelTime->setText(QString("Time: ").append(QString("%1:%2")
                                                    .arg(m_player->position() / 60000, 2, 10, QChar('0'))
                                                    .arg((m_player->position() / 1000) % 60, 2, 10, QChar('0'))));
}

void MainWindow::setSliderRange(qint64 duration)
{
    ui->horizontalSliderTime->setMaximum(static_cast<int>(duration));
    ui->labelDuration->setText(QString("Duration: ").append(QString("%1:%2")
                                                            .arg(m_player->duration() / 60000, 2, 10, QChar('0'))
                                                            .arg((m_player->duration() / 1000) % 60, 2, 10, QChar('0'))));
}

