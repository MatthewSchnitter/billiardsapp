#include "billiardsview.h"
#include "billiardsmodel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BilliardsModel model;
    BilliardsView w(model);
    w.show();
    return a.exec();
}
