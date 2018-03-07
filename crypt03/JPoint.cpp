//
//  JPoint.cpp
//  crypt03
//
//  Created by Андрей Зорькин on 15.12.17.
//  Copyright © 2017 Андрей Зорькин. All rights reserved.
//

#include "JPoint.hpp"

void initArithmetic(){ ZZ_p::init(conv<ZZ>(module_str)); }

void loadECP(){
    ECP.p = conv<ZZ_p>(p_str);
    ECP.a = conv<ZZ_p>(a_str);
    ECP.x = conv<ZZ_p>(x_base);
    ECP.y = conv<ZZ_p>(y_base);
    ECP.e = (-(3*sqr(ECP.p) + 4*ECP.a))/16;
    ECP.d = 3*ECP.p/4;
    ECP.q = conv<ZZ_p>(q_str);
    const ZZ_p& x = ECP.x;
    const ZZ_p& y = ECP.y;
    const ZZ_p& p = ECP.p;
    ECP.X_base = 2*(x-p);
    ECP.Y_base = (2*x+p)*sqr(x-p)-sqr(y);
    ECP.Z_base = y;
}

JPoint getBasePoint(){ return JPoint(ECP.X_base, ECP.Y_base, ECP.Z_base); }

JPoint::JPoint(): X(0), Y(1), Z(1){ }

JPoint::JPoint(ZZ_p X_init, ZZ_p Y_init, ZZ_p Z_init): X(X_init), Y(Y_init), Z(Z_init){ }

JPoint JPoint::operator+(const JPoint& other){
    const ZZ_p& X1 = this->X;
    const ZZ_p& Y1 = this->Y;
    const ZZ_p& Z1 = this->Z;
    const ZZ_p& X2 = other.X;
    const ZZ_p& Y2 = other.Y;
    const ZZ_p& Z2 = other.Z;
    const ZZ_p& e = ECP.e;
    const ZZ_p& d = ECP.d;
    ZZ_p X3 = X1*Z1*Y2 + Y1*X2*Z2;
    ZZ_p Y3 = (sqr(Z1)*sqr(Z2) + e*sqr(X1)*sqr(X2)) * (Y1*Y2 - 2*d*X1*X2*Z1*Z2) + 2*e*X1*X2*Z1*Z2*(sqr(X1)*sqr(Z2)+sqr(Z1)*sqr(X2));
    ZZ_p Z3 = sqr(Z1)*sqr(Z2)-e*sqr(X1)*sqr(X2);
    
    return JPoint(X3, Y3, Z3);
}

JPoint JPoint::operator-(){ return JPoint(-(this->X), this->Y, this->Z);}

JPoint JPoint::operator-(JPoint& other){ return *this + (-other); }

bool JPoint::operator==(const JPoint& other){
    const ZZ_p& X1 = this->X;
    const ZZ_p& Y1 = this->Y;
    const ZZ_p& Z1 = this->Z;
    const ZZ_p& X2 = other.X;
    const ZZ_p& Y2 = other.Y;
    const ZZ_p& Z2 = other.Z;
    if (X1 != ZZ_p(0)){
        ZZ_p t = X2/X1;
        return ( sqr(t)*Y1 == Y2 && t*Z1 == Z2) ? true : false;
    }else if (Z1 != ZZ_p(0)){
        ZZ_p t = Z2/Z1;
        return (sqr(t)*Y1 == Y2 && t*X1 == X2) ? true : false;
    }else if (X2 != ZZ_p(0)){
        ZZ_p t = X1/X2;
        return (sqr(t)*Y2 == Y1 && t*Z2 == Z1) ? true : false;
    }else if (Z2 != ZZ_p(0)){
        ZZ_p t = Z1/Z2;
        return ( sqr(t)*Y2 == Y1 && t*X2 == X1) ? true : false;
    }else
        return (Y1 == Y2) ? true : false;
}

JPoint JPoint::doubled(){
    JPoint T = JPoint();
    const ZZ_p& e = ECP.e;
    const ZZ_p& d = ECP.d;
    const ZZ_p A = power(Z, ZZ(4));
    const ZZ_p B = X*Z;
    const ZZ_p C = power(X, ZZ(4));
    T.X = 2*Y*B;
    T.Y = (A + e*C)*(sqr(Y)-2*d*sqr(B)) + 4*e*C*A;
    T.Z = A - e*C;
    return T;
}

JPoint JPoint::pow(const ZZ n){
    JPoint Pn = *this;
    JPoint S = JPoint(ZZ_p(0), ZZ_p(1), ZZ_p(1));
    for (int i = 0; i < NumBits(n);  ++i){
        if (bit(n, i))
            S = S + Pn;
        Pn = Pn.doubled();
    }
    return S;
}

JPoint JPoint::powByInput(){
    std::string buf{};
    std::cout << " Введите степень > ";
start:
    std::cin >> buf;
    std::cout << '\n';
    int i = 0;
    while (buf[i] != '\0' && i < buf.length()){
        if (!isdigit(buf[i])){
            std::cout<< " Неверный ввод, повторите > ";
            goto start;
        }
        ++i;
    }
    ZZ n = conv<ZZ>(buf.c_str());
    return this->pow(n);
}

JPoint JPoint::inWeierstrass(){
    const ZZ_p& X = this->X;
    const ZZ_p& Y = this->Y;
    const ZZ_p& Z = this->Z;
    const ZZ_p& e = ECP.e;
    const ZZ_p x = 2*(Y + sqr(Z))/sqr(X) - e/2;
    const ZZ_p y = (4*(Y+sqr(Z))-3*e*sqr(X))*Z/power(X,3);
    std::cout << " Координаты точки в форме Вейерштрасса" << '\n';
    std::cout << " x: " << x << "\n y: " << y << "\n\n";
    return *this;
}

JPoint JPoint::printCoordinates(){
    std::cout << " X: "<< X << "\n Y: " << Y << "\n Z: " << Z << "\n\n";
    return *this;
}

JPoint JPoint::isOnCurve(){
    const ZZ_p& e = ECP.e;
    const ZZ_p& d = ECP.d;
    if (sqr(Y)== e*sqr(sqr(X))-2*d*sqr(X)*sqr(Z)+sqr(sqr(Z)))
        std::cout << " Да, точка лежит на кривой\n\n";
    else
        std::cout << " Нет, точка не лежит на кривой\n\n";
    return *this;
}

JPoint JPoint::isEqualTo(const JPoint Point){
    if (*this == Point)
        std::cout << " Да, эти точки равны\n\n";
    else
        std::cout << " Нет, эти точки разные\n\n";
    return *this;
}
