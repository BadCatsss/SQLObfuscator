#ifndef OBFUSCATOR_H
#define OBFUSCATOR_H
#include <QtSql>
#include <QTextStream>
#include <QFileInfo>

class Obfuscator
{
private:
    QString filePath = "";
    QString databaseName = "";
    QSqlDatabase dbase;
    QString obfusicationError;
    QTextStream consoleOutput;
    QVector<QVector<QString>> addingOperators;
    const double lonBorderCoordinate[4]{1, 2.05, 1.79, 5.2}; // долгота С, В, Ю , 3
    const double latBorderCoordinate[4]{3.57, 1.4, 3.33, 1}; // широта С, З, Ю , В
    double relocateLatConst=1;
    double relocateLonConst=1;
    bool openDatabaseFile();
    bool createConnection(const QString& testColumnRequest);
    bool addOperators();
    bool changeForeginKey();
    bool changeLacCid(const QString& column);
    bool changeCellName();
    bool changeAdress();
    bool relocatePosition(const QString& colum, const double coefficient);
    void addToErrorList(const QString& errorMassege);
    void printErrorList();
public:
    Obfuscator(const QString& path);
    bool makeObfuscation();
};
#endif // OBFUSCATOR_H
