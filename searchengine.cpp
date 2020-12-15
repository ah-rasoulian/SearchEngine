#include "searchengine.h"
#include "ui_searchengine.h"

SearchEngine::SearchEngine(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchEngine)
{
    ui->setupUi(this);
    database = new DatabaseHandler();
    tokenizer = new Tokenizer();
    connect(tokenizer, &Tokenizer::show_message, this, &SearchEngine::show_message);
}

SearchEngine::~SearchEngine()
{
    delete ui;
}


void SearchEngine::on_search_button_clicked()
{

}

void SearchEngine::on_document_button_clicked()
{
    ui->result_list->clear();
    QString directory_path = ui->document_input->toPlainText();
    QDir directory(directory_path);
    if (!directory.exists()){
        ui->result_list->addItem("There is no such directory!");
        return;
    }
    tokenizer->tokenize(directory);
}

void SearchEngine::show_message(QString message){
    ui->result_list->addItem(message);
}
