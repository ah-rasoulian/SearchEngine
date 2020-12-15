#include "searchengine.h"
#include "ui_searchengine.h"

SearchEngine::SearchEngine(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchEngine)
{
    ui->setupUi(this);
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

}
