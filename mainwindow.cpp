#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QFileDialog>
#include<QFile>
#include<QTextStream>
#include<QtCharts>
#include<QChartView>
#include<QBarSet>
#include<QBarSeries>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Text tool");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
Funzione invocata in risposta all'evento clicked di pushButton
*/
void MainWindow::on_pushButton_clicked()
{
    pathFile = QFileDialog::getOpenFileName(this,
                                            tr("Open file"),
                                            QDir::homePath(),
                                            "Text File(*.txt)");    // salvo nella variabile pathFile il path da cui cercare il file.txt da caricare e visualizzare
    QFile txtFile(pathFile);
    if(!txtFile.open(QIODevice::ReadOnly)){     // controllo se il file selezionato è utilizzabile per la lettura
        QMessageBox::information(0,"errore", txtFile.errorString());
        return;
    }

    QTextStream in(&txtFile);
    ui->textEdit->setText(in.readAll());    // disposizione del testo letto dallo stream di input in nel widget textEdit
    ui->textEdit->setReadOnly(true);

    QString testo = ui->textEdit->toPlainText();        // conteggio caratteri
    int countChar=testo.length();
    for(int i=0; i<testo.length(); i++){
        if(testo[i]=="\n")
            countChar--;
    }
    nchar = countChar; // salvataggio del numero di caratteri contati

    int countCharns=testo.length(); // conteggio caratteri (no spazi)
    for(int i=0; i<testo.length(); i++){
        if(testo[i]=="\x020" || testo[i]=="\n")
            countCharns--;
    }
    ncharNoSpace=countCharns;       // salvataggio del numero di caratteri senza spazi contati

    int countParole=0;      // conteggio parole
    if(testo.length()>0){
        if(testo[0]!="\x020" && testo[0]!="\n")
            countParole++;
        for(int i=1; i<testo.length(); i++){
            if(testo[i]!="\x020" && testo[i]!="\n") {
                if(testo[i-1]=="\x020" || testo[i-1]=="\n")
                    countParole++;
            }
        }
     }
    nparole=countParole;        // salvataggio del numero di parole contate

    int countFrasi=0;       // conteggio frasi
    if(testo.length()>0){
        for(int i=1; i<testo.length(); i++){
            if((testo[i]=="." || testo[i]=="?" || testo[i]=="!") && (testo[i-1]!="\x020" && testo[i-1]!="." && testo[i-1]!="?" && testo[i-1]!="!"))
                countFrasi++;
        }
    }
    nFrasi=countFrasi;      // salvataggio del numero di frasi contate

    int countParagrafi=0;       // conteggio paragrafi
    if(testo.length()>0){
        for(int i=1; i<testo.length(); i++){
            if(testo[i]=="\n" && testo[i-1]!="\n")
                countParagrafi++;
        }

    if(testo[testo.length()-1]==".")
        countParagrafi++;
    }
    nParag=countParagrafi;      // salvataggio del numero di paragrafi contati


    ui->tableWidget->setRowCount(5);        // creazione tabella contenente le statistiche precedentemente calcolate
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList categorie;
    categorie << "caratteri" << "caratteri (no spaces)" << "parole" << "frasi" << "paragrafi";      // creazione QString contenente le categorie della tabella
    ui->tableWidget->setVerticalHeaderLabels(categorie);
    QStringList res;
    res << "risultati";
    ui->tableWidget->setHorizontalHeaderLabels(res);

    for(int i=0; i<ui->tableWidget->rowCount(); i++){       // inserimento dati nella tabella
        QTableWidgetItem *item=new QTableWidgetItem();

        if(i==0)
            item->setText(QString::number(nchar));
        if(i==1)
            item->setText(QString::number(ncharNoSpace));
        if(i==2)
            item->setText(QString::number(nparole));
        if(i==3)
            item->setText(QString::number(nFrasi));
        if(i==4)
            item->setText(QString::number(nParag));

        ui->tableWidget->setItem(i,0,item);
    }

    ui->graphic->setEnabled(true);      // enable del bottone grafico
    ui->save_info->setEnabled(true);    // enable del bottone salvataggio

}



/*
Funzione invocata in risposta all'evento clicked di graphic
*/
void MainWindow::on_graphic_clicked()
{
    QBarSeries *series = new QBarSeries();      // creazione serie di dati da inserire nel grafico

    QBarSet *setInfo = new QBarSet("");     // creazione insieme delle statistiche calcolate


    *setInfo << nchar << ncharNoSpace << nparole << nFrasi << nParag;


    series->append(setInfo);        // inserimento delle statistiche nella serie


    QChart *chart = new QChart();       // creazione chart
    chart->addSeries(series);       // aggiunta della serie di dati nel chart
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setMaximumWidth(621);        // impostazione dimensioni del grafico
    chart->setMaximumHeight(650);

    QStringList categorie;      //creazione categorie del grafico
    categorie << "caratteri" << "no spaces" << "parole" << "frasi" << "paragrafi";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categorie);
    chart->createDefaultAxes();     //creazione assi
    chart->setAxisX(axis, series);      //disposizione di categorie e dati nel grafo

    QChartView *view = new QChartView(chart);       //creazione della view per il grafo
    view->setMinimumSize(621, 650);     //impostazione dimensioni minime della view
    view->setWindowTitle("grafico");
    view->show();
}



/*
Funzione invocata in risposta all'evento clicked di save_info
*/
void MainWindow::on_save_info_clicked()
{
    QString fileName = QFileInfo(pathFile).fileName() + ".csv";     //QString contenente il nome del file csv che verrà generato
    QString path = QDir::homePath();        // path dove il file verrà salvato

    QStringList row1;       //StringList contenenti la coppia di stringhe categoria-dato
    QStringList row2;
    QStringList row3;
    QStringList row4;
    QStringList row5;

    row1 << "caratteri" << QString::number(nchar);
    row2 << "caratteri(no spazi)" << QString::number(ncharNoSpace);
    row3 << "parole" << QString::number(nparole);
    row4 << "frasi" << QString::number(nFrasi);
    row5 << "paragrafi" << QString::number(nParag);

    QFile file(path+"/"+fileName);      //creazione file
    QTextStream out(&file);     //creazione stream output
    if(file.open(QIODevice::WriteOnly))     //se il file esiste-> sovrascrivi; altrimenti -> crea nuovo file
    {

        out<<row1.at(0)<<","<<row1.at(1)<<"\n";     // disposizione dati nel file csv
        out<<row2.at(0)<<","<<row2.at(1)<<"\n";
        out<<row3.at(0)<<","<<row3.at(1)<<"\n";
        out<<row4.at(0)<<","<<row4.at(1)<<"\n";
        out<<row5.at(0)<<","<<row5.at(1)<<"\n";
        QMessageBox::information(this, "Save info", "salvataggio completato");
    }
    else
        QMessageBox::warning(0,"Errore", "Salvataggio non riuscito");


    file.close();
}

