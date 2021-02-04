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

    // inserting clustered directories to database
    QDir directory1("/home/amirhossein/Data/University/Fall 2020/Information Retreival/clustered/بهداشت/");
    tokenizer->find_files(directory1, false, true);
    QDir directory2("/home/amirhossein/Data/University/Fall 2020/Information Retreival/clustered/تاریخ/");
    tokenizer->find_files(directory2, false, true);
    QDir directory3("/home/amirhossein/Data/University/Fall 2020/Information Retreival/clustered/ریاضیات/");
    tokenizer->find_files(directory3, false, true);
    QDir directory4("/home/amirhossein/Data/University/Fall 2020/Information Retreival/clustered/فناوری/");
    tokenizer->find_files(directory4, false, true);
    QDir directory5("/home/amirhossein/Data/University/Fall 2020/Information Retreival/clustered/فیزیک/");
    tokenizer->find_files(directory5, false, true);
}

SearchEngine::~SearchEngine()
{
    delete ui;
    delete tokenizer;
    delete query_processor;
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
    tokenizer->find_files(directory, false, false);
}

void SearchEngine::show_message(QString message){
    ui->result_list->addItem(message);
}

void SearchEngine::on_result_list_itemDoubleClicked(QListWidgetItem *item)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(item->text()));
}

void SearchEngine::on_ordered_search_button_clicked()
{
    ui->result_list->clear();
    QString querry = ui->search_input->toPlainText();
    if(!querry.isEmpty())
        query_processor->search(querry, "ordered");
}

void SearchEngine::on_ordered_champion_clicked()
{
    ui->result_list->clear();
    QString querry = ui->search_input->toPlainText();
    if(!querry.isEmpty())
        query_processor->search(querry, "ordered-champion");
}

void SearchEngine::on_clustered_search_button_clicked()
{
    ui->result_list->clear();
    QString querry = ui->search_input->toPlainText();
    if(!querry.isEmpty())
        query_processor->search(querry, "clustered");
}
