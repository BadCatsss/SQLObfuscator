#include "Obfuscator.h"

Obfuscator::Obfuscator(const QString& path):consoleOutput(stdout)
{
    QFileInfo info(path);
    this->filePath = info.absoluteFilePath();
}
Obfuscator::~Obfuscator()
{
}

void Obfuscator:: addToErrorList(const QString& errorMassege)
{
    obfusicationErrors.push_back(errorMassege);
}
void Obfuscator::printErrorList()
{
    for (auto errorMessage : obfusicationErrors) {
        consoleOutput << errorMessage << endl;
    }
}
bool Obfuscator::openDatabaseFile()
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

bool Obfuscator::createConnection()
{

    dbase = QSqlDatabase::addDatabase("QSQLITE", "mobileStandarts");
    dbase.setDatabaseName(databaseName + ".db");
    if (!dbase.open()) {
        addToErrorList("Cant connect to open database file");
        return false;
    }
    else {
        consoleOutput << "successfull connect to database" << endl;
        return true;
    }
}
bool Obfuscator::addOperators()
{
    bool returnVale = false;
    QSqlQuery mainCurrentQuery(dbase);
    QVector<QString> columnsName = {":OPERATORID", ":MCC", ":MNC", ":NAME", ":FULLNAME"};
    for (int operatorsCount = 0; operatorsCount < this->addingOperators.size(); ++operatorsCount) {
        if (mainCurrentQuery.prepare("INSERT INTO OPERATORS(OPERATORID,MCC,MNC,NAME,FULLNAME)  VALUES (:OPERATORID, :MCC, :MNC, :NAME,:FULLNAME)"))
        {
            for (int j = 0; j < columnsName.size(); ++j) {
                mainCurrentQuery.bindValue(columnsName[j], this->addingOperators[operatorsCount][j]);
            }
            mainCurrentQuery.exec();
            returnVale = true;
        }
        else {
            returnVale = false;
            addToErrorList("Cant add operators to table");
            break;
        }
    }
    if (returnVale) {
        consoleOutput << "operators was added" << endl;
    }
    return returnVale;
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
bool Obfuscator::changeTableData(const QString& column, int mode)
{
    QSqlQuery mainCurrentQuery(dbase);
    QSqlQuery sideConnection (dbase);
    QString querySelectExpression;
    QString queryUpdateExpression;
    QString currentValues;
    QString subExpression;
    int currentWorkIndex;
    bool updateSuccessFlag=false;
    QVector<QString> columnValues;
    QSqlRecord rec;

    if (mode == 0) { //change LAC ,CELLID
        querySelectExpression = "SELECT " + column + " FROM CELLS";
        mainCurrentQuery.exec(querySelectExpression);
        rec = mainCurrentQuery.record();
        currentWorkIndex = rec.indexOf(column); // index of the field
        while (mainCurrentQuery.next()) {
            columnValues.push_back(mainCurrentQuery.value(currentWorkIndex).toString());
        }

        for (int var = 0; var < columnValues.size(); ++var) {
            currentValues = columnValues[var];
            queryUpdateExpression =QString::fromStdString( "UPDATE   CELLS  SET " + column.toStdString() + "=" + column.toStdString() + "+ABS(RANDOM()) % (1000 - 10) + 10 WHERE " + column.toStdString() + "=");
            queryUpdateExpression.append(currentValues);
            if (sideConnection.prepare(queryUpdateExpression)) {
                sideConnection.exec();
                updateSuccessFlag = true;
            }
            else {
                updateSuccessFlag = false;
                break;
            }
        }

    }
    if (mode == 1) //change CELL_NAME
    {
        querySelectExpression="SELECT " + column + " FROM CELLS";
        mainCurrentQuery.exec(querySelectExpression);
        rec = mainCurrentQuery.record();
        currentWorkIndex=rec.indexOf(column);
        while (mainCurrentQuery.next()) {
            columnValues.push_back(mainCurrentQuery.value(currentWorkIndex).toString());
        }

        for (int var = 0; var < columnValues.size(); ++var) {
            currentValues = columnValues[var];
            subExpression = "CellName"+currentValues;

            queryUpdateExpression="UPDATE CELLS  SET CELL_NAME='" +subExpression + "' WHERE OPERATORID = "+currentValues;
            if (sideConnection.prepare(queryUpdateExpression)) {
                sideConnection.exec();
                updateSuccessFlag = true;
            }
            else {
                updateSuccessFlag = false;
                break;
            }
        }
    }

    if (updateSuccessFlag) {
        consoleOutput << "Data in " + column + " successfull changed" << endl;
        return true;
    }
    else {
        addToErrorList("Cant change" + column + "values");
        return false;
    }
}


bool Obfuscator::changeAdress()
{
    QSqlQuery mainCurrentQuery(dbase);
    if (mainCurrentQuery.prepare("UPDATE CELLS SET ADR = 'Some Adress'")) {
        mainCurrentQuery.exec();
        consoleOutput << "Adress was changed" << endl;
        return true;
    }
    else {
        addToErrorList("Adress was not changed");
        return false;
    }
}

bool Obfuscator::makeObfuscation()
{
    if ( this->openDatabaseFile() && this->createConnection()
         && this->addOperators() &&   this->changeForeginKey()
         &&  this->changeTableData("LAC",0) && this->changeTableData("CELLID",0)
         &&  this->changeTableData("OPERATORID",1) &&   this->changeAdress() ) {

        return true;
    }
    else {
        printErrorList();
        return false;
    }
}
