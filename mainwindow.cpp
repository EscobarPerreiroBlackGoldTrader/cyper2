#include "mainwindow.h"
#include "ui_mainwindow.h"

const int MAXSIMPLE = 2000000; // не должно быть менше чем два миллиона
//===============================================================================================
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    SimplyNumbers(NULL),// зануление указателя перед началом работы
    FermaNum(NULL)
{
    ui->setupUi(this);

    init_start_values();
}
//===============================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//===============================================================================================
void MainWindow::on_toolButton_clearSrc_clicked()
{
    ui->plainTextEdit_src->clear();
}
//===============================================================================================
void MainWindow::on_toolButton_clearOut_clicked()
{
    ui->plainTextEdit_out->clear();
}
//===============================================================================================
void MainWindow::init_start_values() //получение начальных значений
{
    SimplyNumbers = FindAllSimplyNumbersTo_n(MAXSIMPLE);


    q = 211;//7;
    p = 223;//23;
    N = q*p;
    M = (q-1)*(p-1); // нужен только чтобы найти секретную экспоненту d

    //e = 16813;
    e = 257; // открытая экспонента

    for(int i=0; i<M; i++){// алгоритм Евклида для вычисления секретной экспоненты d
        if ((i*e-1)%M==0){
            d=i;
            break;
        }
    }

    ui->plainTextEdit_src->setPlainText( "N = " + QString::number(N) +
                                         ",M = " + QString::number(M) +
                                         ",e = " + QString::number(e) +
                                         ",d = " + QString::number(d));


    FermaNum = new QVector<int>(3); // Заполнение простыми числами для e
    FermaNum->replace(0,17);
    FermaNum->replace(1,257);
    FermaNum->replace(2,65537);
}
//===============================================================================================
QString MainWindow::get_encrypted_S(QString plain_message)
{
/*
    QByteArray arr = plain_message.toUtf8();
    QByteArray larr = plain_message.toLocal8Bit();

    QString a;

    ui->plainTextEdit_out->appendPlainText(a.fromLocal8Bit(larr));
  */



    //************************************************************
//    for(int g=0;g<arr.length();++g){
//        ui->plainTextEdit_out->appendPlainText(arr[g]);
//    }

    QString chiped_message;

    int lng = plain_message.length();
    QChar literal, chiplit;
    uchar byteH,byteL;
    char cr;
    uint chiped_char,unicode_num;
    for(int i=0;i<lng; ++i){
        literal = plain_message.at(i);

        byteL = literal.cell();// младший байт
        byteH = literal.row(); // старший байт
        unicode_num = byteH;
        unicode_num <<= 8; //сдвиг влево на восемь бит с присваиванием
        unicode_num += byteL;


        //cr = literal.toLatin1();
        //ui->plainTextEdit_out->appendPlainText(literal.row());
        //literal.unicode();

        chiped_char = fmod(unicode_num,e,N);



        //ui->plainTextEdit_out->setPlainText();
        //ui->plainTextEdit_out->appendPlainText(QString::number(chr));
        ui->plainTextEdit_out->appendPlainText("row " + QString::number(literal.row()));
        ui->plainTextEdit_out->appendPlainText("cell " + QString::number(literal.cell()));
        ui->plainTextEdit_out->appendPlainText("unicode " + QString::number(literal.unicode()));
        ui->plainTextEdit_out->appendPlainText("chipped_char " + QString::number(chiped_char));
        chiped_message.append( QChar(chiped_char) );
        chiplit = chiped_message.at(i);
        ui->plainTextEdit_out->appendPlainText("extracted_char unicode " + QString::number(chiplit.unicode()));


    }

    return chiped_message;
}
//===============================================================================================
QString MainWindow::get_decrypted_S(QString chiped_message)
{

    QString unchiped_message;
    int lng = chiped_message.length();
    QChar literal;
    uchar byteH,byteL;
    uint unchiped_char,unicode_num;
    ui->plainTextEdit_out->appendPlainText("=============================");
    ui->plainTextEdit_out->appendPlainText("income to unchipe: " + chiped_message);
    for(int i=0;i<lng; ++i){
        literal = chiped_message.at(i);

        byteH = literal.row();
        byteL = literal.cell();
        unicode_num = byteH;
        unicode_num <<= 8;
        unicode_num += byteL;
        //literal.unicode();

        unchiped_char = fmod(unicode_num,d,N);



        //ui->plainTextEdit_out->setPlainText();
        ui->plainTextEdit_out->appendPlainText("row " + QString::number(literal.row()));
        ui->plainTextEdit_out->appendPlainText("cell " + QString::number(literal.cell()));
        ui->plainTextEdit_out->appendPlainText("unicode num " + QString::number(unicode_num));
        ui->plainTextEdit_out->appendPlainText("unchipped_char " + QString::number(unchiped_char));
        unchiped_message.append(unchiped_char);
    }
    return unchiped_message;


}
//===============================================================================================
int MainWindow::fmod(uint a,uint b,uint N) // функция возведения в степень по модулю a^b(mod N)
{

    uint at = a;
    uint S = a;
    for(int i=1;i<b;++i){
        for(int j=1;j<a;++j){
            S += at;
            if(S >= N) S -= N;
        }
        at = S;
    }
    return S;
}
//===============================================================================================
int MainWindow::ucmod(uchar a,int b,int N) // функция возведения в степень по модулю a^b(mod N)
{
    int at = a;
    int S = a;
    for(int i=1;i<b;++i){
        for(int j=1;j<a;++j){
            S += at;
            if(S >= N) S -= N;
        }
        at = S;
    }
    return S;
}

//===============================================================================================
QVector<bool> *MainWindow::FindAllSimplyNumbersTo_n(int n)
{
    if(n > 2147395600)return NULL; // если n слишком велико - возвращает пустой указатель

    QVector<bool> * prime = new QVector<bool>(n+1,true);

    prime->replace(0,false);
    prime->replace(1,false);
    //prime[0] = prime[1] = false;
    for (int i=2; i*i<=n; ++i)   // работает для n < 46340^2 = 2147395600
        if (prime->value(i))
            for (int j=i*i; j<=n; j+=i)
                prime->replace(j,false);

    return prime;
}
//===============================================================================================
void MainWindow::on_pushButton_Encrypt_clicked()
{
   /*
    QTime now;
    now.start();
    int nw = now.second();
    int r = rand()* nw;
    ui->plainTextEdit_src->setPlainText(QString::number(r) + " " + QString::number(nw) );
    */


    QString S_enc = get_encrypted_S(ui->plainTextEdit_src->toPlainText());
    ui->plainTextEdit_out->appendPlainText(S_enc);

    int l = S_enc.length();
    for(int i=0;i<l;++i){
        ui->plainTextEdit_out->appendPlainText(QString::number(S_enc.at(i).unicode()) );
    }


/*
    int plain_text = 112;
    ui->plainTextEdit_out->appendPlainText("plain number " + QString::number(plain_text));
    int chipped_text = fmod(plain_text,e,N);
    ui->plainTextEdit_out->appendPlainText("chipped number " + QString::number(chipped_text));
    int unchipped_text = fmod(chipped_text,d,N);
    ui->plainTextEdit_out->appendPlainText("unchipped number " + QString::number(unchipped_text));
*/
    //ui->plainTextEdit_out->appendPlainText("2^3 = " + QString::number(fmod(2,3,20)));
}
//===============================================================================================
void MainWindow::on_pushButton_Decrypt_clicked()
{
    QString S_decr = get_decrypted_S(ui->plainTextEdit_src->toPlainText());
    ui->plainTextEdit_out->appendPlainText(S_decr);
    /*
    int l = S_decr.length();
    for(int i=0;i<l;++i){
        ui->plainTextEdit_out->appendPlainText(QString::number(S_decr.at(i).unicode()) );
    }
    */

}
//===============================================================================================
