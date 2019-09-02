#ifndef OBFUSCATOR_H
#define OBFUSCATOR_H
#include <QtSql>
#include <QTextStream>

class Obfuscator
{
private:

    QString filePath = "";
    QString databaseName = "";
    QSqlDatabase dbase;
    QVector<QString> obfusicationErrors;
    QTextStream consoleOutput;
    QVector<QVector<QString>> addingOperators = {{"725","999","99","Aqua","Aqua"},{"726","999","98","Bro","Bro"},
                                               {"727","999","97","Bro2","Bro2"},{"728", "999", "96", "Bro3", "Bro3"}};
    bool openDatabaseFile();
    bool createConnection();
    bool addOperators();
    bool changeForeginKey();
    bool changeTableData(const QString& colum, int mode);
    bool changeAdress();
    bool relocatePosition();

    void addToErrorList(const QString& errorMassege);
    void printErrorList();

public:
    Obfuscator(const QString& path);
    ~Obfuscator();
    bool makeObfuscation();
};

#endif // OBFUSCATOR_H
