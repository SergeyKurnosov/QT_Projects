#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyle>
#include <QFileDialog>
#include <QTime>

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

    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
    connect(this->ui->horizontalSliderTime, &QSlider::sliderMoved, this, &MainWindow::on_horizontalSliderTime_sliderMoved);

    //          play list init
    m_playlist_model = new QStandardItemModel(this);
    this->ui->tableViewPlayList->setModel(m_playlist_model);

    m_playlist_model->setHorizontalHeaderLabels(QStringList()<< "Audio track" << "Files path" << "Duration");
    this->ui->tableViewPlayList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableViewPlayList->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->ui->tableViewPlayList->hideColumn(1);
    int duration_with = 64;
    this->ui->tableViewPlayList->setColumnWidth(2, duration_with);
    this->ui->tableViewPlayList->setColumnWidth(0,this->ui->tableViewPlayList->width() - duration_with*1.7);

    m_playlist = new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);

    connect(this->ui->pushButtonPrev, &QPushButton::clicked, this->m_playlist, &QMediaPlaylist::previous);
    connect(this->ui->pushButtonNext, &QPushButton::clicked, this->m_playlist, &QMediaPlaylist::next);
    connect(this->m_playlist, &QMediaPlaylist::currentIndexChanged, this->ui->tableViewPlayList, &QTableView::selectRow);
    connect(this->ui->tableViewPlayList, &QTableView::doubleClicked,
            [this](const QModelIndex& index){m_playlist->setCurrentIndex(index.row()); this->m_player->play();}
            );
    connect(this->m_player, &QMediaPlayer::currentMediaChanged,
            [this](const QMediaContent& media)
    {
        this->ui->labelFileName->setText(media.canonicalUrl().toString());
        this->setWindowTitle(this->ui->labelFileName->text().split('/').last());
    }

            );
}

MainWindow::~MainWindow()
{
    delete m_playlist_model;
    delete m_playlist;
    delete m_player;
    delete ui;
}

void MainWindow::loadFileToPlaylist(const QString &filename)
{
    m_playlist->addMedia(QUrl(filename));
    QList<QStandardItem*> items;
    items.append(new QStandardItem(QDir(filename).dirName()));
    items.append(new QStandardItem(filename));
  //  QMediaPlayer player;
  //  player.setMedia(QUrl(filename));
    m_duration_player.setMedia(QUrl(filename));
 //   m_duration_player.play();
   // player.play();
   // player.pause();
    //items.append(new QStandardItem(QTime::fromMSecsSinceStartOfDay(player.duration()).toString("mm:ss")));
    items.append(new QStandardItem(QString::number(m_duration_player.duration())));
    m_duration_player.pause();
    m_playlist_model->appendRow(items);


}


void MainWindow::on_pushButtonAdd_clicked()
{
   /* QString file = QFileDialog::getOpenFileName
    (
      this,
      "Open file",
      "D:\\ProjectHW\\Audio_Player\\Audio_Player\\Tracks",
      "Audio files (*.mp3 *.flac);; MP-3 (*.mp3);; Flac (*.flac)"
    );

    ui->labelFileName->setText(QString("File: ").append(file));
    this->m_player->setMedia(QUrl(file));*/

    QStringList files = QFileDialog::getOpenFileNames
             (
                this,
                "Open file",
                "D:\\ProjectHW\\Audio_Player\\Audio_Player\\Tracks",
                "Audio files(*.mp3 *.flac *.flacc);; mp3 (*.mp3);; Flac (*flac *.flac)"
             );
    for(QString file:files)
    {
        loadFileToPlaylist(file);
    }

}


void MainWindow::on_pushButtonPlay_clicked()
{
    this->m_player->play();
}


void MainWindow::on_horizontalSliderVolume_valueChanged(int value)
{
    m_player->setVolume(value);
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(value)));
}



void MainWindow::on_pushButtonPause_clicked()
{
    m_player->state() == QMediaPlayer::State::PausedState ? m_player->play() : this->m_player->pause();
}


void MainWindow::on_pushButtonStop_clicked()
{
    this->m_player->stop();
}


void MainWindow::on_pushButtonMute_clicked()
{
    m_player->setMuted(!m_player->isMuted());
    ui->pushButtonMute->setIcon(style()->standardIcon(m_player->isMuted() ? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
}

void MainWindow::on_duration_changed(qint64 duration)
{
    this->ui->horizontalSliderTime->setRange(0,duration);
    this->ui->labelDuration->setText(QTime::fromMSecsSinceStartOfDay(duration).toString("hh:mm:ss"));
}

void MainWindow::on_position_changed(qint64 position)
{
    this->ui->labelPosition->setText(QString(QTime::fromMSecsSinceStartOfDay(position).toString("hh:mm:ss")));
    this->ui->horizontalSliderTime->setValue(position);
}




void MainWindow::on_horizontalSliderTime_sliderMoved(qint64 position)
{
    this->m_player->setPosition(position);
}

