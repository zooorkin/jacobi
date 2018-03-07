//
//  main.cpp
//  crypt03
//
//  Created by Андрей Зорькин on 15.12.17.
//  Copyright © 2017 Андрей Зорькин. All rights reserved.
//

#include <iostream>
#include "JPoint.hpp"

void printHeader(std::string s){
    std::cout << "---------------------------------------------------------------------------------\n";
    std::cout << "   " << s << '\n';
    std::cout << "---------------------------------------------------------------------------------\n";
}

void check(){
    JPoint E = JPoint();
    JPoint B = getBasePoint();
    JPoint P = JPoint(ZZ_p(0), ZZ_p(-1), ZZ_p(1));
    JPoint X = JPoint(ZZ_p(6),ZZ_p(2),ZZ_p(6));
    ZZ q = rep(ECP.q);
    
    printHeader("ТОЧКИ");
    std::cout << " E - нейтральный элемент" << '\n';
    std::cout << " B - базовая точка эллиптической кривой" << '\n';
    std::cout << " P - точка порядка 2" << '\n';
    std::cout << " X - некоторая точка, возможно, не лежащая на кривой" << '\n';
    std::cout << '\n';
    printHeader("ВСПОМОГАТЕЛЬНАЯ ИНФОМРАЦИЯ");
    std::cout << " q - порядок циклической подгруппы группы точек эллиптической кривой" << '\n';
    std::cout << '\n';
    
    printHeader("КООРДИНАТЫ ТОЧЕК");
    std::cout << " Координаты точки E:\n";
    E.printCoordinates();
    std::cout << " Координаты точки B:\n";
    B.printCoordinates();
    std::cout << " Координаты точки P:\n";
    P.printCoordinates();
    std::cout << " Координаты точки X:\n";
    X.printCoordinates();
    
    printHeader("ПРОВЕРКА ПРИНАДЛЕЖНОСТИ ТОЧЕК КРИВОЙ");
    std::cout << " E - точка эллиптической кривой?\n";
    E.isOnCurve();
    std::cout << " B - точка эллиптической кривой?\n";
    B.isOnCurve();
    std::cout << " P - точка эллиптической кривой?\n";
    P.isOnCurve();
    std::cout << " X - точка эллиптической кривой?\n";
    X.isOnCurve();
    
    printHeader("ПРОВЕРКА СПРАВЕДЛИВОСТИ РАВЕНСТВ");
    std::cout << " Верно ли, что E + E == E ?\n";
    (E+E).isEqualTo(E);
    std::cout << " Верно ли, что E + B == B ?\n";
    (E+B).isEqualTo(B);
    std::cout << " Верно ли, что B + E == B ?\n";
    (B+E).isEqualTo(B);
    std::cout << " Верно ли, что E + B == E ?\n";
    (E+B).isEqualTo(E);
    std::cout << " Верно ли, что B + B == B ?\n";
    (B+B).isEqualTo(B);
    std::cout << " Верно ли, что B == 2*B ?\n";
    B.isEqualTo(B.doubled());
    std::cout << " Верно ли, что B + 2*B == 2*B + B ?\n";
    (B + B.doubled()).isEqualTo(B.doubled() + B);
    std::cout << " Верно ли, что P + P == 2*P ?\n";
    (P+P).isEqualTo(P.doubled());
    std::cout << " Верно ли, что 2*P == E ?\n";
    P.doubled().isEqualTo(E);
    std::cout << " Верно ли, что B + B + B == 3*B ?\n";
    (B + B + B).isEqualTo(B.pow(ZZ(3)));
    std::cout << " Верно ли, что B + B + B + B + B + B + B == 7*B ?\n";
    (B + B + B + B + B + B + B).isEqualTo(B.pow(ZZ(7)));
    std::cout << " Верно ли, что q*B == E ?\n";
    B.pow(q).isEqualTo(E);
    printHeader(" ВОЗВЕДЕНИЕ ТОЧКИ B СТЕПЕНЬ ТОЧКИ B");
    //while (1)
    B.powByInput().printCoordinates();
}

int main(){
    initArithmetic();
    loadECP();
    check();
    return 0;
}
