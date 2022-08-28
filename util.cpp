#ifndef HEADER_DATA
#define HEADER_DATA
#include <QString>
#include <QDateTime>
#include <QDebug>

struct Data{
    QString str;
    QString sender;
    QDateTime timestamp;

    QString toString(){
        return timestamp.toString() + "   " + sender + "\n" + str;
    }

    QString fline(){
        return timestamp.toString() + "   " + sender;
    }

    QString sline(){
        return str;
    }

    QString convert(QString append = ""){
        return str+"</>"+sender+"</>"+timestamp.toString() + append;
    }
};

static Data FromString(QString str){
    QStringList arr = str.split("</>");
    return {arr.at(0),arr.at(1),QDateTime::fromString(arr.at(2))};
}

#endif
