#include "searchengine.h"
#include "ui_searchengine.h"

SearchEngine::SearchEngine(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchEngine)
{
    ui->setupUi(this);

    database = new DatabaseHandler();
    connect(database, &DatabaseHandler::show_message, this, &SearchEngine::show_message);

    linguistic_modules = new LinguisticModules();

    tokenizer = new Tokenizer(database, linguistic_modules);
    connect(tokenizer, &Tokenizer::show_message, this, &SearchEngine::show_message);

    query_processor = new QueryProcessor(database, linguistic_modules);
    connect(query_processor, &QueryProcessor::show_message, this, &SearchEngine::show_message);
}

SearchEngine::~SearchEngine()
{
    delete ui;
}


void SearchEngine::on_search_button_clicked()
{
    ui->result_list->clear();
    QString querry = ui->search_input->toPlainText();
    if(!querry.isEmpty())
        query_processor->search(querry, "simple_binary");
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
    tokenizer->find_files(directory);
}

void SearchEngine::show_message(QString message){
    ui->result_list->addItem(message);
}
