#include "Nodes/a2lfile.h"
#include "workproject.h"
#include <QMessageBox>
#include "a2lgrammar.h"

// Functions (Predicate)
bool nodeLessThan( const Node *a, const Node *b );
bool itemLessThan( const Item *a, const Item *b );

A2LFILE::A2LFILE(QTextStream &in, Node *parentNode, A2lLexer *lexer, QStringList *errorList, QString fullFileName)
    : Node(parentNode, lexer, errorList)
{
    //initialize
    project = NULL;
    optParameters = new QMap<std::string, std::string>;
    fullA2lName = fullFileName;

    //call the parser
    parser(in);
}

A2LFILE::~A2LFILE()
{
    delete optParameters;
    delete[] name;
    delete errorList;
    delete lex;
}

void A2LFILE::parser(QTextStream &in)
{
    TokenTyp token = nextToken(in);

    while (token == Keyword)
    {
        if (lex->getLexem() == "ASAP2_VERSION")
            getAsap2Version(in);
        else if (lex->getLexem() == "A2ML_VERSION")
            getA2mlVersion(in);
        else
        {
            QString s1(lex->toString(token).c_str());
            QString s2(lex->getLexem().c_str());

            showError("ASAP2 parser : wrong ASAP2 file format at line ???\n"
                                    "expected token : Keyword (ASAP2_VERSION or A2ML_VERSION)\n"
                                    "find token : " + s1 + " (" + s2 + ")");
            return;
        }
        token = nextToken(in);
    }

    if (token == BlockBegin)
    {
        TokenTyp token1 = nextToken(in);

        if (token1 == Keyword && lex->getLexem() == "PROJECT")
        {
            PROJECT *child = new PROJECT(in, this, lex);
            addChildNode(child);
            project = child;

            token = nextToken(in);
        }
        else
        {
            QString s1(lex->toString(token1).c_str());
            QString s2(lex->getLexem().c_str());

            showError("ASAP2 parser : wrong ASAP2 file format at line ???\n"
                                    "expected token : Identifier (PROJECT)\n"
                                    "find token : " + s1 + " (" + s2 + ")");
        }
    }
    else
    {
        QString s(lex->toString(token).c_str());

        this->showError("ASAP2 parser : wrong ASAP2 file format at line ???\n"
                                "expected token : BlockBegin or Keyword\n"
                                 "find token : " + s);
    }
}

void A2LFILE::getAsap2Version(QTextStream &in)
{        
    if (optParameters->contains("ASAP2_VERSION"))
    {
        showError("ASAP2 parser ASAP2_VERSION already defined");
        return;
    }

    std::string str;
    TokenTyp token1 = nextToken(in);

    if (token1 == Integer)
    {
        str += lex->getLexem();
        TokenTyp token2 =nextToken(in);
        if (token2 == Integer)
        {
            str += " ";
            str += lex->getLexem();
            optParameters->insert("ASAP2_VERSION", str);
        }
        else
        {
            this->showError("ASAP2 parser : wrong ASAP2 file format at line ???");
        }

    }
    else
    {
        this->showError("ASAP2 parser : wrong ASAP2 file format at line ???");
    }
}

void A2LFILE::getA2mlVersion(QTextStream &in)
{
    if (this->optParameters->contains("A2ML_VERSION"))
    {
        this->showError("ASAP2 parser : A2ML_VERSION already defined");
        return;
    }

    std::string str;
    TokenTyp token1 = nextToken(in);

    if (token1 == Integer)
    {
        str += lex->getLexem();
        TokenTyp token2 = nextToken(in);
        if (token2 == Integer)
        {
            str += " ";
            str += lex->getLexem();
            this->optParameters->insert("A2ML_VERSION", str);
        }
        else
        {
            this->showError("ASAP2 parser : wrong ASAP2 file format at line ???");
        }
    }
    else
    {
        this->showError("ASAP2 parser : wrong ASAP2 file format at line ???");
    }

}

bool A2LFILE::isConform()
{
    if (project)
    {
        QList<MODULE*> list = project->listModule();
        if (list.isEmpty())
        {
            return false;
        }
        else
            return true;
    }
    else
    {
        return false;
    }
}

std::string A2LFILE::pixmap()
{
    return ":/icones/milky_cartable.png";
}

QString A2LFILE::fullName()
{
    return fullA2lName;
}

PROJECT *A2LFILE::getProject()
{
    return project;
}
