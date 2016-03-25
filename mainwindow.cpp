#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->doubleSpinBox->setMinimum(3.2);
    ui->listWidget->hide();
    ui->lcdNumber->setFont(QFont("Times", 10, QFont::Bold));
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Filled);
    setWindowTitle("Sender");
    setWindowIcon(QIcon(":image/images/main.png"));
    msg.setWindowIcon(QIcon(":image/images/main.png"));
    msg.setWindowTitle("Оповещение");
    ui->actionPlay_mess->setEnabled(false);
    ui->actionPause_mess->setEnabled(false);
    ui->actionStop_mess->setEnabled(false);
    ui->lineEdit->setText("===========---У Славика лучшие вещи--=========");
    ui->mainToolBar->setMovable(false);
    movie = new QMovie(":/image/images/load5.gif");
    ui->label_movie->setMovie(movie);
    movie->start();
    ui->label_movie->hide();
    mobile_album.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_webView_urlChanged(const QUrl &arg1)
{
    if ("/blank.html" == arg1.path()) {
        QRegExp regexp("access_token=([^,]+)&expires_in=([^,]+)&user_id=([^,]+)");
        QString str=arg1.fragment();
        if( -1 != regexp.indexIn(str) ) {
            m_access_token = regexp.cap(1);
            m_expires_in = regexp.cap(2);
            m_user_id = regexp.cap(3);
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Авторизирован. Можно начинать рассылку",10000);
            can_do_message=true;
        }
    }
    else if("/api/login_failure.html" == arg1.path()){
        msg.setText("Errrror. Не получил token");
        msg.exec();
        can_do_message=false;
    }

}



void MainWindow::send_mess(){

    ////////////////////////////////////Часть один №1
    ///
    ///
    if(!can_do_message){
        msg.setText("Вы не вошли");
        msg.exec();
        return;
    }
    ui->label_movie->show();
    ui->listWidget->clear();
    play_mess=true;
    pause_mess=false;
    stop_mess=false;
    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Начало рассылки",0);

    timer_to_send=ui->doubleSpinBox->value();
    ui->webView->hide();
    ui->listWidget->show();


    QUrlQuery request;
    if(m_user_id.toInt()==338897086){
        request.addQueryItem("chat_id","1");
        request.addQueryItem("message","БОТ: Рассылка запущена на климентии");
    }
    if(m_user_id.toInt()==324486924){
        request.addQueryItem("chat_id","5");
        request.addQueryItem("message","БОТ: Рассылка запущена почему то с главного славика");
    }
    if(m_user_id.toInt()==336536313){
        request.addQueryItem("chat_id","2");
        request.addQueryItem("message","БОТ: Рассылка запущена на молодом Славике");
    }
    request.addQueryItem("v","5.42");
    request.addQueryItem("access_token",m_access_token);
    QUrl sad=(QString("https://api.vk.com/method/messages.send?"));
    sad.setQuery(request);

    QNetworkAccessManager *manager=new QNetworkAccessManager(this);
    QNetworkReply *reply=manager->get(QNetworkRequest(sad));
    wait;
    connect(manager, SIGNAL(finished(QNetworkReply*)), &wait, SLOT(quit()));
    wait.exec();

    QByteArray answer=reply->readAll();
    qDebug() << answer;

    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Получение списка групп",0);

    request.clear();
    sad.clear();


    ////////////////////////////////////Часть два  №2

    request.addQueryItem("user_id", "336536313");
    request.addQueryItem("v","5.42");
    request.addQueryItem("access_token",m_access_token);
    sad=(QString("https://api.vk.com/method/groups.get?"));
    sad.setQuery(request);

    manager=new QNetworkAccessManager(this);
    reply=manager->get(QNetworkRequest(sad));
    connect(manager, SIGNAL(finished(QNetworkReply*)), &wait, SLOT(quit()));
    wait.exec();

    answer=reply->readAll();
    qDebug() << answer;

    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Начало рассылки",5000);



    ////////////////////////////////////Часть три  №3



    QList<QByteArray> baList = answer.split(',');//ba.split(' '); // делим строку на кусочки... в качестве разделителя используем пробел
    QList<int> intList; // сюда запишем результат
    qDebug() << "start";
    for (int i=0;i<baList.size();i++) // перебираем все кусочки строки
    {
        intList.append(baList[i].toInt()); // переводим их int и записываем в новый массив
        qDebug() << intList[i];
    }
    count=intList.count()-3;
    qDebug() << "end";
    qDebug() << count;
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(count);


    request.clear();
    sad.clear();
    what_albums=ui->comboBox->currentIndex();
    QString temp;
    QString ansver_is_error;
    new_message=ui->lineEdit->text();
    if(new_message.isEmpty()){
        has_text=false;
    }else {has_text=true;}
    f_piece=count*timer_to_send/60;
    s_piece=count*timer_to_send-(f_piece*60);
    ui->comboBox->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->doubleSpinBox->setEnabled(false);
    ui->actionLogin->setEnabled(false);
    ui->actionStart_messages->setEnabled(false);
    ui->actionPlay_mess->setEnabled(false);
    ui->actionPause_mess->setEnabled(true);
    ui->actionStop_mess->setEnabled(true);
    ui->label_movie->hide();
    for(int i=0;i<count;i++){
        if(i%5==0 && i!=0 && s_piece!=0)s_piece--;
        if(i%10==0){
            f_piece=(count-i)*timer_to_send/60;
            s_piece=(count-i)*timer_to_send-(f_piece*60);
        }
        temp.clear();
        if(pause_mess==true){
            pauseMessanger();
        }
        if(stop_mess==true){
            stopMessage();
            return;
        }
        temp="-"+QString::number(intList[i+2]);
        request.addQueryItem("owner_id", temp);
        if(has_text)request.addQueryItem("message",new_message);
        if(what_albums==0){request.addQueryItem("attachments","album336536313_225104626,album336536313_225104597,album336536313_225104636,album336536313_225104603,album336536313_225104662"+mobile_album);}
        if(what_albums==1){request.addQueryItem("attachments","album324486924_222555197,album324486924_222555174,album324486924_223636335,album324486924_222555165,album324486924_223636302"+mobile_album);}
        request.addQueryItem("v","5.42");
        request.addQueryItem("access_token",m_access_token);
        sad=(QString("https://api.vk.com/method/wall.post?"));
        sad.setQuery(request);

        manager=new QNetworkAccessManager(this);
        reply=manager->get(QNetworkRequest(sad));
        connect(manager, SIGNAL(finished(QNetworkReply*)), &wait, SLOT(quit()));
        wait.exec();
        answer.clear();
        answer=reply->readAll();
        qDebug() << answer;


        ansver_is_error=takeErrorInAnswer(answer);

        ui->listWidget->addItem(answer);
        ui->listWidget->addItem(ansver_is_error);
        ui->listWidget->scrollToBottom();

        sleeping();
        ui->progressBar->setValue(ui->progressBar->value()+1);

        request.clear();
        sad.clear();
        delete manager;
    }
    ui->lcdNumber->display("00:00");
    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Рассылка закончена",10000);
    msg.setText("Рассылка закончена");
    msg.exec();
    ui->comboBox->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->doubleSpinBox->setEnabled(true);
    ui->actionLogin->setEnabled(true);
    ui->actionStart_messages->setEnabled(true);
    ui->actionPlay_mess->setEnabled(false);
    ui->actionPause_mess->setEnabled(false);
    ui->actionStop_mess->setEnabled(false);
    music();
}

void MainWindow::on_actionLogin_triggered()
{
    ui->label_movie->show();
    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Загрузка web узла",0);
    ui->webView->load(QUrl("https://oauth.vk.com/authorize?client_id=5197660&display=page&redirect_uri=http://oauth.vk.com/blank.html&scope=wall,groups,messages&response_type=token&v=5.42"));

}

void MainWindow::on_actionStart_messages_triggered()
{
    send_mess();
}

void MainWindow::on_webView_loadStarted()
{
        ui->label_movie->show();
        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Загрузка web узла...",0);
}

void MainWindow::on_webView_loadFinished(bool arg1)
{
        ui->statusBar->clearMessage();
        ui->statusBar->showMessage("Web узел загружен",0);
        ui->label_movie->hide();
}



void MainWindow::sleeping(){
    wait;
    for(int i=0;i<timer_to_send;i++){
        QTimer::singleShot(1000, &wait, SLOT(quit()));//вариант 2 в закладках
        wait.exec();
        lcd.clear();
        lcd=QString::number(f_piece)+":"+QString::number(s_piece);
        ui->lcdNumber->display(lcd);
        if(s_piece==0){
            f_piece--;
            s_piece=59;
            if(f_piece<0){
                f_piece=0;
                s_piece=30;
            }

        }
        else s_piece--;
        if(f_piece==0 && s_piece==0 && ui->progressBar->value()+5<ui->progressBar->maximum()){
            s_piece=30;
        }
    }

}


QString MainWindow::takeErrorInAnswer(QByteArray answer){
    char mass_ansver[2000]={0};

    strncpy(mass_ansver, answer,100);
    QString numb_error;

    for(int i=0;i<answer.length();i++){
        if(mass_ansver[i]=='e' && mass_ansver[i+1]=='r' && mass_ansver[i+2]=='r' && mass_ansver[i+3]=='o' && mass_ansver[i+4]=='r' &&
                mass_ansver[i+5]=='_' && mass_ansver[i+6]=='c' && mass_ansver[i+7]=='o' && mass_ansver[i+8]=='d'){
            if(mass_ansver[i+12]>='0' && mass_ansver[i+12]<='9' && mass_ansver[i+13]>='0' && mass_ansver[i+13]<='9' && mass_ansver[i+14]>='0' && mass_ansver[i+14]<='9'){
                numb_error.append(mass_ansver[i+12]);
                numb_error.append(mass_ansver[i+13]);
                numb_error.append(mass_ansver[i+14]);
            }else if(mass_ansver[i+12]>='0' && mass_ansver[i+12]<='9' && mass_ansver[i+13]>='0' && mass_ansver[i+13]<='9'){
                numb_error.append(mass_ansver[i+12]);
                numb_error.append(mass_ansver[i+13]);
            }else if(mass_ansver[i+12]<='0' && mass_ansver[i+12]<='9'){
                numb_error.append(mass_ansver[i+12]);

            }

        }
    }
    if(numb_error.toInt()==14){
        return "Ошибка, требуется капча";
    }
    if(numb_error.toInt()==214){
         return "Ошибка, Публикация запрещена. Превышен лимит на число публикаций в сутки, либо на указанное время уже запланирована другая запись. ";
    }
    if(numb_error.toInt()==219){
         return "Ошибка, Рекламный пост уже недавно публиковался. ";
    }
    if(numb_error.toInt()==220){
         return "Ошибка, Слишком много получателей ";
    }
    if(numb_error.toInt()==222){
         return "Ошибка, Запрещено размещать ссылки. ";
    }
    return "Ok";
}





void MainWindow::on_actionPlay_mess_triggered()
{
    play_mess=true;
    pause_mess=false;
    stop_mess=false;
}

void MainWindow::on_actionPause_mess_triggered()
{
    play_mess=false;
    pause_mess=true;
    stop_mess=false;
}

void MainWindow::on_actionStop_mess_triggered()
{
    play_mess=false;
    pause_mess=false;
    stop_mess=true;
}

void MainWindow::pauseMessanger(){
    play_mess=false;
    stop_mess=false;
    ui->actionPause_mess->setEnabled(false);
    ui->actionPlay_mess->setEnabled(true);
    ui->actionStop_mess->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->doubleSpinBox->setEnabled(true);
    while(true){
        if(play_mess==true){
            break;
        }
        if(stop_mess==true)
        {
            ui->lcdNumber->display("00:00");
            ui->statusBar->clearMessage();
            ui->statusBar->showMessage("Рассылка закончена",10000);
            msg.setText("Рассылка закончена");
            msg.exec();
            ui->comboBox->setEnabled(true);
            ui->lineEdit->setEnabled(true);
            ui->doubleSpinBox->setEnabled(true);
            ui->actionLogin->setEnabled(true);
            ui->actionStart_messages->setEnabled(true);
            ui->actionPause_mess->setEnabled(false);
            ui->actionPlay_mess->setEnabled(false);
            ui->actionStop_mess->setEnabled(false);
            pause_mess=false;
            play_mess=false;
            stop_mess=false;
            return;
        }
        QTimer::singleShot(100, &wait, SLOT(quit()));//вариант 2 в закладках
        wait.exec();
    }
    pause_mess=false;
    if(new_message=="Введите текс сообщения" || new_message.isEmpty()){
        has_text=false;
    }else {has_text=true;}
    new_message=ui->lineEdit->text();
    timer_to_send=ui->doubleSpinBox->value();
    what_albums=ui->comboBox->currentIndex();
    f_piece=count*timer_to_send/60;
    s_piece=count*timer_to_send-(f_piece*60);
    ui->actionPlay_mess->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->doubleSpinBox->setEnabled(false);
    ui->actionPause_mess->setEnabled(true);
}


void MainWindow::stopMessage(){
    ui->lcdNumber->display("00:00");
    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("Рассылка закончена",10000);
    msg.setText("Рассылка закончена");
    msg.exec();
    ui->comboBox->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    ui->doubleSpinBox->setEnabled(true);
    ui->actionLogin->setEnabled(true);
    ui->actionStart_messages->setEnabled(true);
    pause_mess=false;
    stop_mess=false;
    play_mess=false;
    music();
}


void MainWindow::music(){
    Beep (659.26,200);
              Beep (659.26,200);
              Sleep (200);
              Beep (659.26,200);
              Sleep (100);
              Beep (523.26,200);
              Beep (659.26,200);
              Sleep(200);
              Beep (783.98,200);
              Sleep(400);
              Beep (391.99,200);
}



void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        mobile_album=",album336536313_226119188";
    }
    else{
        mobile_album.clear();
    }
}
