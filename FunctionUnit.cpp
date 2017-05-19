#include "FunctionUnit.h"

namespace func {

Polynominal::Polynominal (
        const std::vector<double> &coefficient, const FunctionUnit &funit) {

    init(coefficient, funit);
}

Polynominal::Polynominal (const Polynominal &obj) {

    coef = obj.coef;
    nest = obj.nest->clone();
}

void Polynominal::init(
        const std::vector<double> &coefficient, const FunctionUnit &funit) {

    coef = coefficient;
    nest = funit.clone();
}

double Polynominal::call (double x) const {

    double base = nest->call(x);
    double pow = 1.0;
    double value = 0.0;

    for (auto itr = coef.begin(); itr != coef.end(); ++itr) {
        value += pow * (*itr);
        pow *= base;
    }

    return value;
}

std::shared_ptr<FunctionUnit> Polynominal::differential () const {

    std::vector<double> dcoef;
    int size = static_cast<int>(coef.size());
    std::shared_ptr<FunctionUnit> dnest = nest->differential();

    // TODO implement synthetic derivative
    for (int i = 1; i < size; ++i) {
        dcoef.push_back(coef[i] * i);
    }

    return std::make_shared<Polynominal>(dcoef, *nest);
}

std::shared_ptr<FunctionUnit> Polynominal::clone () const {
    return std::make_shared<Polynominal>(coef, *nest);
}

std::string Polynominal::toString () const {

    std::string base = nest->toString();
    int size = static_cast<int>(coef.size());
    std::string str = "";

    for (int i = size - 1; i > 0; --i) {
        if (coef[i] != 0.0) {
            str += std::to_string(coef[i]) +
                "{(" + base + ")} ^ {" + std::to_string(i) + "}";
        }

        if (coef[i - 1] != 0.0) {
            str += " + ";
        }
    }

    if (coef[0] != 0) {
        str += std::to_string(coef[0]);
    }

    return str;
}

}
