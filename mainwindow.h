#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_clearSrc_clicked();

    void on_toolButton_clearOut_clicked();

    void on_pushButton_Encrypt_clicked();

    void on_pushButton_Decrypt_clicked();

private:
    Ui::MainWindow *ui;

    //-------------------

    int q,p; // Два случайных простых числа заданного размера (например 1024 бита каждое)

    int N; // модуль чисел q и p равный q*p

    int M; // значение функции эйлера от N равное (M = (p-1)*(q-1) )

    int e; // целое число ( 1<e<M ), взаимно простое со значением функции Эйлера M,
           // Обычно в качестве e берут простые числа содержащие небольшое количество бит
           // в двоичной записи, например простые числа Ферма 17, 257 , 65537.
           // Число e называется открытой экспонентой.
           // Время необходимое для шифрования с использованием быстрого возведения в степень, пропорционально
           // числу единичных бит в e.
           // Слишком малые значения e, например 3, потенциально могут ослабить безопасность схемы RSA.

    int d; // число d, мультипликативно обратное числу e по модулю M, то есть число, удовлетворяющее сравнению,
            // d*e = 1(mod M)
            // число d называется секретной экспонентой. Обычно оно вычисляется
            // при помощи расширенного алгоритма Евклида.

    void init_start_values();

    QString get_encrypted_S(QString plain_message);
    QString get_decrypted_S(QString chiped_message);
    
    int fmod(uint a, uint b, uint N);

    QVector<int> *FermaNum;

    QVector<bool> *SimplyNumbers; // указатель на массив простых целых чисел
    QVector<bool> * FindAllSimplyNumbersTo_n(int n); //возвращает указатель на массив простых целых чисел
    // расчитаных от 2 до n
    int ucmod(uchar a, int b, int N);
};

#endif // MAINWINDOW_H
