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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadFile(const QString *filename);
    void setStopped(bool p);

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void openFile();
    void toggleStopped();
    void userSliderChange();
    void seekFrame(int frame);
    void updatePosition(int frame, double time);

private:
    Ui::MainWindow* m_ui;
    QLabel* m_statusLabel;
    Player m_player;
    int m_currentFrame;
    bool m_stopped;
};

#endif // MAINWINDOW_H
