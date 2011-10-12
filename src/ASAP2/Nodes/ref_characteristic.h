// HEXplorer is an Asap and HEX file editor
// Copyright (C) 2011  <Christophe Hoel>
//
// This file is part of HEXplorer.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// please contact the author at : christophe.hoel@gmail.com

#ifndef REF_CHARACTERISTIC_H
#define REF_CHARACTERISTIC_H

#include "node.h"
#include "item.h"
#include "lexer.h"
#include "basefactory.h"


class REF_CHARACTERISTIC : public Node
{
    public:
        REF_CHARACTERISTIC(Node *parentNode);
        ~REF_CHARACTERISTIC();

        QMap<std::string, std::string> *getParameters();
        static Factory<Node,REF_CHARACTERISTIC> nodeFactory;
        std::string pixmap();
        QStringList getCharList();
        char* getPar(std::string str);

    private:
        // Fix parameters
        QList<TokenTyp> *typePar;
        QList<std::string> *namePar;
        QList<char*> parameters;
        void parseFixPar(QList<TokenTyp> *typePar);

        // Opt parameters
        QMap<std::string, FactoryPlant<Node> *>  *factoryOptNode;
        QMap<std::string, FactoryPlant<Item> *>  *factoryOptItem;

        // CHARACTERISTICS list
        QList<std::string> charList;
        TokenTyp parseListChar();
};

#endif // REF_CHARACTERISTIC_H
