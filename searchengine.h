#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SearchEngine; }
QT_END_NAMESPACE

class SearchEngine : public QMainWindow
{
    Q_OBJECT

public:
    SearchEngine(QWidget *parent = nullptr);
    ~SearchEngine();

private:
    Ui::SearchEngine *ui;
};
#endif // SEARCHENGINE_H
