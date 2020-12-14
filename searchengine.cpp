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

