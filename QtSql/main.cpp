#include <QCoreApplication>
#include "Obfuscator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString pathToFile= "";
    QTextStream in(stdin);
    QTextStream out(stdout);
    out<<"input path to database file:"<<endl;
    out<<"example: D:\\QtSQL\\QtSql\\stations.db"<<endl;
    in>>pathToFile;
    Obfuscator obf(pathToFile);
    if (obf.makeObfuscation()) {
        out << "Obfuscation correct complate" << endl;
    }
    else {
        out << "Obfuscation not complate" << endl;
    }

    return a.exec();
}
