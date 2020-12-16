#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QMainWindow>
#include <tokenizer.h>
#include <databasehandler.h>
#include <queryprocessor.h>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SearchEngine; }
QT_END_NAMESPACE

class SearchEngine : public QMainWindow
{
    Q_OBJECT

public:
    SearchEngine(QWidget *parent = nullptr);
    ~SearchEngine();

public slots:
    void show_message(QString message);

private slots:
    void on_search_button_clicked();

    void on_document_button_clicked();

private:
    Ui::SearchEngine *ui;
    DatabaseHandler *database;
    Tokenizer *tokenizer;
    QueryProcessor *query_processor;

};
#endif // SEARCHENGINE_H
