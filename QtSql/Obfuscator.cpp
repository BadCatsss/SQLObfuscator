#include "Obfuscator.h"

Obfuscator::Obfuscator(const QString& path):consoleOutput(stdout)
{
    QFileInfo info(path);
    this->filePath = info.absoluteFilePath();
    this->addingOperators = {{"725", "999", "99", "Aqua", "Aqua"}, {"726", "999", "98", "Bro", "Bro"},
                             {"727", "999", "97", "Bro2", "Bro2"}, {"728", "999", "96", "Bro3", "Bro3"}};

    this->relocateLatConst = abs( 1./(latBorderCoordinate[0] * latBorderCoordinate[1] - latBorderCoordinate[2] * latBorderCoordinate[3]));
    this->relocateLonConst = abs( 1./(lonBorderCoordinate[1] * lonBorderCoordinate[2] - lonBorderCoordinate[3] * lonBorderCoordinate[0]));
}
void Obfuscator:: addToErrorList(const QString& errorMassege)
{
    obfusicationError=errorMassege;
}
void Obfuscator::printErrorList()
{
        consoleOutput << obfusicationError << endl;
}
bool Obfuscator::openDatabaseFile()
{
    if (QFile::exists(filePath))
    {
        int startSearchPosition = filePath.lastIndexOf(".");
        if (filePath.indexOf("db",startSearchPosition) != -1)
        {
            filePath.remove(filePath.lastIndexOf("."), filePath.length() - filePath.lastIndexOf("."));
            this->databaseName = filePath;
            consoleOutput << "file successfull open" << endl;
            return true;
        }
        else {
            addToErrorList("Cant open file. Incorrect format");
            return false;
        }
    }
    else {
        addToErrorList("The specified file does not exist on this path.");
        return false;
    }
}
bool Obfuscator::createConnection(const QString& testColumnRequest)
{
    dbase = QSqlDatabase::addDatabase("QSQLITE", "mobileStandarts");
    dbase.setDatabaseName(databaseName + ".db");
    if (!dbase.open()) {
        addToErrorList("Cant connect to database.Cant open datbase file.");
        return false;
    }
    else {
        QSqlQuery testConnectionQuery(dbase);
        if (testConnectionQuery.prepare(QString("SELECT %1 FROM CELLS").arg(testColumnRequest))) {
            consoleOutput << "successfull connect to database" << endl;
            return true;
        }
        else {
            addToErrorList("Cant connect to database.Test request not sucessfull.");
            return false;
        }
    }
}
bool Obfuscator::addOperators()
{
    QSqlQuery mainCurrentQuery(dbase);
    QVector<QString> columnsName = {":OPERATORID", ":MCC", ":MNC", ":NAME", ":FULLNAME"};
    for (int operatorsCount = 0; operatorsCount < this->addingOperators.size(); ++operatorsCount) {
        if (mainCurrentQuery.prepare("INSERT INTO OPERATORS(OPERATORID,MCC,MNC,NAME,FULLNAME)  VALUES (:OPERATORID, :MCC, :MNC, :NAME,:FULLNAME)"))
        {
            for (int j = 0; j < columnsName.size(); ++j) {
                mainCurrentQuery.bindValue(columnsName[j], this->addingOperators[operatorsCount][j]);
            }
            mainCurrentQuery.exec();
        }
        else {
            addToErrorList("Cant add operators to table");
            return false;
        }
    }

    consoleOutput << "operators was added" << endl;
    return true;
}
bool Obfuscator::changeForeginKey()
{
    QSqlQuery mainCurrentQuery(dbase);
    if (mainCurrentQuery.prepare("UPDATE CELLS SET OPERATORID=ABS(RANDOM()) % (729 - 725) + 725")) {
        mainCurrentQuery.exec();
        consoleOutput << "Foregin Key successfull changed" << endl;
        return true;
    }
    else {
        addToErrorList("Cant change foregin key in CELL table");
        return false;
    }
}
bool Obfuscator::changeLacCid(const QString& column)
{
    QSqlQuery mainCurrentQuery(dbase);
    if ( mainCurrentQuery.prepare(QString("UPDATE CELLS SET %1=%1+ABS(RANDOM()) % (1000 - 10) * 10 WHERE %1=%1").arg(column)) ) {
        mainCurrentQuery.exec();
        consoleOutput << column + " was changed" << endl;
        return true;
    }
    else {
        addToErrorList(column + " was not changed.Query not prepare.");
        return false;
    }
}
bool Obfuscator::changeCellName()
{
    QSqlQuery sideConnection(dbase);
    QSqlQuery mainCurrentQuery(dbase);
    if ( mainCurrentQuery.prepare(QString("UPDATE CELLS SET CELL_NAME = 'CellName_' || CELLID"))) {
        mainCurrentQuery.exec();
        consoleOutput << "Cell name was changed" << endl;
        return true;
    }
    else {
        addToErrorList("Name was not changed.Query not prepare");
        return false;
    }
}
bool Obfuscator::changeAdress()
{
    QSqlQuery mainCurrentQuery(dbase);
    if (mainCurrentQuery.prepare("UPDATE CELLS SET ADR = 'Some Adress'")) {
        mainCurrentQuery.exec();
        consoleOutput << "Adress was changed." << endl;
        return true;
    }
    else {
        addToErrorList("Adress was not changed.");
        return false;
    }
}
bool Obfuscator::relocatePosition(const QString& column,const double coefficient)
{
    QSqlQuery mainCurrentQuery(dbase);
    mainCurrentQuery.prepare(QString("UPDATE CELLS  SET  %1 = %1 * :coeficient WHERE  %1 = %1").arg(column));
    mainCurrentQuery.bindValue(":coeficient", coefficient);
    if (mainCurrentQuery.exec()) {
        consoleOutput << column + " was changed" << endl;
        return true;
    }
    else {
        addToErrorList("Position was not relocate");
        return false;
    }
}
bool Obfuscator::makeObfuscation()
{
    if ( this->openDatabaseFile() && this->createConnection("LAC")
         && this->addOperators() && this->changeForeginKey()
         &&  this->changeLacCid("LAC") && this->changeLacCid("CELLID")
         &&  this->changeCellName() && this->changeAdress()
         &&  this->relocatePosition("LAT",this->relocateLatConst) && this->relocatePosition("LON",this->relocateLonConst) ) {
        return true;
    }
    else {
        printErrorList();
        return false;
    }
}

