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
    const double* lonBorderCoordinate = new double[4]{1,2.05,1.79,5.2}; // долгота С, Ю, З , В
    const double* latBorderCoordinate = new double[4]{3.57,1.4,3.33,1,}; // широта С, З, Ю , В
  const  double relocateLatConst=abs( 1./(latBorderCoordinate[0]*latBorderCoordinate[1]-latBorderCoordinate[2]*latBorderCoordinate[3]));
   const double relocateLonConst=abs( 1./(lonBorderCoordinate[1]*lonBorderCoordinate[2]-lonBorderCoordinate[3]*lonBorderCoordinate[0]));
    bool openDatabaseFile();
    bool createConnection();
    bool addOperators();
    bool changeForeginKey();
    bool changeTableData(const QString& colum, int mode);
    bool changeAdress();
    bool relocatePosition(const QString& colum);

    void addToErrorList(const QString& errorMassege);
    void printErrorList();

public:
    Obfuscator(const QString& path);
    ~Obfuscator();
    bool makeObfuscation();
};

#endif // OBFUSCATOR_H
