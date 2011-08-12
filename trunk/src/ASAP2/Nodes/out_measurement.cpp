#include "Nodes/out_measurement.h"
#include <QMessageBox>
#include <typeinfo>
#include <QHash>
#include "a2lgrammar.h"

//initialise static variables
Factory<Node,OUT_MEASUREMENT> OUT_MEASUREMENT::nodeFactory;

// Functions (Predicate)
bool nodeLessThan( const Node *a, const Node *b );
bool itemLessThan( const Item *a, const Item *b );

OUT_MEASUREMENT::OUT_MEASUREMENT(QTextStream &in, Node *parentNode)
    : Node(parentNode, parentNode->lex, parentNode->errorList)
{
    //get grammar
    A2lGrammar* gram = parentNode->lex->grammar;
    namePar = &gram->out_measurement.namePar;
    typePar = &gram->out_measurement.typePar;
    factoryOptNode = &gram->out_measurement.factoryOptNode;
    factoryOptItem = &gram->out_measurement.factoryOptItem;

    //Set the line where the Node starts in ASAP file
    a2lLine = lex->getLine();

    name = (char*)"OUT_MEASUREMENT";

    //Parse list Identifiers
    TokenTyp token = parseListChar(in);

    //fianlize parsing
    if (token == BlockEnd)
    {
        token = nextToken(in);
        if (token == Keyword && lex->getLexem() == "OUT_MEASUREMENT")
        {
            //Sort the childNodes
            //qSort(this->childNodes.begin(), this->childNodes.end(), nodeLessThan);

            //Sort The childItems
            //qSort(this->optItems.begin(), this->optItems.end(), itemLessThan);

            //sort the charList
            qSort(charList.begin(), charList.end());
        }
        else
        {
            QString s(lex->toString(token).c_str());
            this->showError("expected token : BlockEnd OUT_MEASUREMENT\nfind token : " + s);
        }
    }
    else
    {
        QString s1(lex->toString(token).c_str());
        QString s2(lex->getLexem().c_str());
        this->showError("expected end OUT_MEASUREMENT\nfind : " + s1 + " " + s2);
    }
}

OUT_MEASUREMENT::~OUT_MEASUREMENT()
{
    foreach (char* ptr, parameters)
    {
        delete[] ptr;
    }
}

QMap<std::string, std::string> *OUT_MEASUREMENT::getParameters()
{
    QMap<std::string, std::string> *par = new QMap<std::string, std::string>;
    for (int i = 0; i < namePar->count(); i++)
    {
        par->insert(namePar->at(i), parameters.at(i));
    }
    return par;
}

std::string  OUT_MEASUREMENT::pixmap()
{
    return ":/icones/CHAR.bmp";
}

TokenTyp OUT_MEASUREMENT::parseListChar(QTextStream &in)
{
    TokenTyp token = lex->getNextToken(in);

    while (token == Identifier)
    {
        charList.append(lex->getLexem());
        token = lex->getNextToken(in);
    }

    return token;
}

QStringList OUT_MEASUREMENT::getCharList()
{
    QStringList list;
    foreach(std::string str, charList)
    {
        list.append(str.c_str());
    }
    return list;
}

char* OUT_MEASUREMENT::getPar(std::string str)
{
    int i = namePar->indexOf(str);
    return parameters.at(i);
}
