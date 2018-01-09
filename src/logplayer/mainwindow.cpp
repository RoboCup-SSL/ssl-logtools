/***************************************************************************
 *   Copyright (c) 2013 Robotics Erlangen e.V.                             *
 *   http://www.robotics-erlangen.de/                                      *
 *   info@robotics-erlangen.de                                             *
 *                                                                         *
 *   This file may be licensed under the terms of the GNU General Public   *
 *   License Version 3, as published by the Free Software Foundation.      *
 *   You can find it here: http://www.gnu.org/licenses/gpl.html            *
 *                                                                         *
 ***************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_stopped(true),
    m_currentFrame(0)
{
    m_ui->setupUi(this);

    m_ui->btnOpen->setIcon(QIcon::fromTheme("document-open"));
    m_ui->btnPlay->setIcon(QIcon::fromTheme("media-playback-start"));

    connect(m_ui->btnOpen, SIGNAL(clicked()), SLOT(openFile()));
    connect(m_ui->actionOpen, SIGNAL(triggered()), SLOT(openFile()));
    connect(m_ui->btnPlay, SIGNAL(clicked()), SLOT(toggleStopped()));
    connect(m_ui->actionPlay, SIGNAL(triggered()), SLOT(toggleStopped()));
    connect(m_ui->horizontalSlider, SIGNAL(sliderReleased()), SLOT(userSliderChange()));

    m_statusLabel = new QLabel;
    statusBar()->addPermanentWidget(m_statusLabel);

    QSettings s;
    s.beginGroup("MainWindow");
    restoreGeometry(s.value("Geometry").toByteArray());
    restoreState(s.value("State").toByteArray());
    s.endGroup();

    connect(&m_player, SIGNAL(positionChanged(int,double)), SLOT(updatePosition(int,double)));

    setStopped(true);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    QSettings s;

    s.beginGroup("MainWindow");
    s.setValue("Geometry", saveGeometry());
    s.setValue("State", saveState());
    s.endGroup();

    QMainWindow::closeEvent(e);
}

void MainWindow::openFile()
{
    setStopped(true);

    QString filename = QFileDialog::getOpenFileName(this, "Select log file", "", "Log files (*.log *.log.gz)");

    if (!filename.isEmpty()) {
        loadFile(&filename);
    }
}

void MainWindow::loadFile(const QString *filename)
{
    m_currentFrame = 0;

    int maxFrame;
    double duration;

    if (m_player.load(*filename, maxFrame, duration)) {
        m_ui->horizontalSlider->setValue(0);
        m_ui->horizontalSlider->setMaximum(maxFrame);

        m_ui->lblPacketMax->setText(QString::number(maxFrame));
        m_ui->lblTimeMax->setText(QString("%1:%2.%3")
                                          .arg((int) (duration / 1E9) / 60)
                                          .arg((int) (duration / 1E9) % 60, 2, 10, QChar('0'))
                                          .arg((int) (duration / 1E6) % 1000, 3, 10, QChar('0')));

        QFileInfo fileInfo(*filename);
        m_statusLabel->setText(fileInfo.fileName());
    }
}

void MainWindow::userSliderChange()
{
    int value = m_ui->horizontalSlider->value();
    seekFrame(value);
}

void MainWindow::seekFrame(int frame)
{
    m_currentFrame = frame;

    if (!m_stopped) {
        m_player.stop();
        m_player.start(m_currentFrame);
    }
}

void MainWindow::toggleStopped()
{
    setStopped(!m_stopped);
}

void MainWindow::setStopped(bool p)
{
    m_stopped = p | !m_player.good();

    if (m_stopped) {
        m_ui->btnPlay->setIcon(QIcon::fromTheme("media-playback-start"));
        m_ui->actionPlay->setText("Play");
        m_player.stop();
    } else {
        m_ui->btnPlay->setIcon(QIcon::fromTheme("media-playback-stop"));
        m_ui->actionPlay->setText("Stop");
        m_player.start(m_currentFrame);
    }
}

void MainWindow::updatePosition(int frame, double time)
{
    m_currentFrame = frame;

    if (!m_ui->horizontalSlider->isSliderDown()) {
        m_ui->horizontalSlider->setValue(frame);
    }

    m_ui->lblPacketCurrent->setText(QString::number(frame));
    m_ui->lblTimeCurrent->setText(QString("%1:%2.%3")
        .arg((int) (time / 1E9) / 60)
        .arg((int) (time / 1E9) % 60, 2, 10, QChar('0'))
        .arg((int) (time / 1E6) % 1000, 3, 10, QChar('0')));
}
