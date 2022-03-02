#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_graphic_clicked();

    void on_save_info_clicked();

private:
    Ui::MainWindow *ui;

    QString pathFile;       // QString contenente il path del file txt caricato
    int nchar;      // numero di caratteri del testo
    int ncharNoSpace;       //numero di caratteri del testo (no spazi)
    int nparole;        // numero di parole del testo
    int nFrasi;     // numero di frasi del testo
    int nParag;     // numero di paragrafi del testo
};
#endif // MAINWINDOW_H
