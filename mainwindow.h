#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebView/QtWebView>

#include <QUrl>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <windows.h>

#include <strsafe.h>

#include <stdio.h>
#include <QCoreApplication>

#include <string.h>
#include <QtMultimedia/QSound>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
        QString m_access_token;
        QString m_expires_in;
        QString m_user_id;
        QString lcd;
        QString new_message;
        QString mobile_album;
        int f_piece=0;
        int s_piece=0;
        int countStartLoad=0;
        int what_albums;
        int count;
        QMessageBox msg;
        bool can_do_message=false;
        bool play_mess=false;
        bool stop_mess=false;
        bool pause_mess=false;
        bool has_text=false;
        QEventLoop wait;

        double timer_to_send=3.2;
        QNetworkReply* m_http;
        QMovie *movie;




        void send_mess();
        void sleeping();
        QString takeErrorInAnswer(QByteArray answer);
        void pauseMessanger();
        void stopMessage();
        void music();




private slots:


    void on_webView_urlChanged(const QUrl &arg1);

    void on_actionLogin_triggered();

    void on_actionStart_messages_triggered();

    void on_webView_loadStarted();

    void on_webView_loadFinished(bool arg1);



    void on_actionPlay_mess_triggered();

    void on_actionPause_mess_triggered();

    void on_actionStop_mess_triggered();

    void on_checkBox_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
