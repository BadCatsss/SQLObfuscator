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
    QVector<QVector<QString>> addingOperators;
    const double lonBorderCoordinate[4]{1, 2.05, 1.79, 5.2}; // долгота С, Ю, З , В
    const double latBorderCoordinate[4]{3.57, 1.4, 3.33, 1}; // широта С, З, Ю , В
    double relocateLatConst=1;
    double relocateLonConst=1;
    bool openDatabaseFile();
    bool createConnection(const QString& testColumnRequest);
    bool addOperators();
    bool changeForeginKey();
    bool changeLacCid(const QString& column);
    bool changeCellName(const QString& column, const QString& values);
    bool changeAdress();
    bool relocatePosition(const QString& colum, const double coefficient);
    void addToErrorList(const QString& errorMassege);
    void printErrorList();
public:
    Obfuscator(const QString& path);
    bool makeObfuscation();
};
#endif // OBFUSCATOR_H
