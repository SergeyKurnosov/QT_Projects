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
    this->m_player->play();
}


void MainWindow::on_pushButtonPlay_clicked()
{
    this->m_player->play();
}

