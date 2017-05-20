#include "FunctionUnit.h"

namespace func {

Summation::Summation (const Summation &obj) {
    terms = obj.terms;
}

Summation::Summation (
        const std::vector<std::shared_ptr<FunctionUnit> > &funits) {
    terms = funits;
}

void Summation::init (
        const std::vector<std::shared_ptr<FunctionUnit> > &funits) {
    terms = funits;
}

double Summation::call (double x) const {

    double value = 0;

    for (auto itr = terms.begin(); itr != terms.end(); ++itr) {
        value += (*itr)->call(x);
    }

    return value;
}

std::shared_ptr<FunctionUnit> Summation::differential () const {

    std::vector<std::shared_ptr<FunctionUnit> > dterms;
    std::shared_ptr<FunctionUnit> term;
    Zero zero;

    for (auto itr = terms.begin(); itr != terms.end(); ++itr) {
        term = (*itr)->differential();
        if (typeid(*term) != typeid(zero)) {
            dterms.push_back((*itr)->differential());
        }
    }

    return std::make_shared<Summation>(dterms);
}

std::shared_ptr<FunctionUnit> Summation::clone () const {
    return std::make_shared<Summation>(terms);
}

std::string Summation::toString () const {

    std::string str = terms[0]->toString();

    for (auto itr = terms.begin() + 1; itr != terms.end(); ++itr) {
        str += " + ";
        str += (*itr)->toString();
    }

    return str;
}

Product::Product (const Product &obj) {
    terms = obj.terms;
}

Product::Product (const std::vector<std::shared_ptr<FunctionUnit> > &obj) {
    init(obj);
}

void Product::init (const std::vector<std::shared_ptr<FunctionUnit> > &obj) {

    terms.clear();

    for (auto itr = obj.begin(); itr != obj.end(); ++itr) {
        if (Zero* f = dynamic_cast<Zero*>(&(**itr))) {
            terms.clear();
            terms.push_back(f->clone());
            break;

        } else if (Base* f = dynamic_cast<Base*>(&(**itr))) {
            (void)f;
            continue;

        } else if (Product* f = dynamic_cast<Product*>(&(**itr))) {
            for (auto ttr = f->terms.begin(); ttr != f->terms.end(); ++ttr) {
                terms.push_back((*ttr)->clone());
            }

        } else {
            terms.push_back((*itr)->clone());
        }
    }
}

double Product::call (double x) const {

    double value = 1.0;
    for (auto itr = terms.begin(); itr != terms.end(); ++itr) {
        value *= (*itr)->call(x);
    }

    return value;
}

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

std::shared_ptr<FunctionUnit> Product::differential () const {
    int size = static_cast<int>(terms.size());
    std::vector<std::shared_ptr<FunctionUnit> > buffer;
    std::vector<std::shared_ptr<FunctionUnit> > dterms;

    for (int i = 0; i < size; ++i) {
        buffer = terms;
        buffer.erase(buffer.begin() + i);
        buffer.push_back(terms[i]->differential()->clone());
        dterms.push_back(std::make_shared<Product>(buffer));
    }

    return std::make_shared<Summation>(dterms);
}

std::shared_ptr<FunctionUnit> Product::clone () const {
    return std::make_shared<Product>(terms);
}

std::string Product::toString () const {
    std::string str = "";
    for (auto itr = terms.begin(); itr != terms.end(); ++itr) {
        str += "(" + (*itr)->toString() + ")";
    }
    return str;
}

std::shared_ptr<FunctionUnit> Polynominal::differential () const {

    std::vector<double> dcoef;
    int size = static_cast<int>(coef.size());
    std::shared_ptr<FunctionUnit> dnest = nest->differential();

    for (int i = 1; i < size; ++i) {
        dcoef.push_back(coef[i] * i);
    }

    std::vector<std::shared_ptr<FunctionUnit> > terms;
    terms.push_back(std::make_shared<Polynominal>(dcoef, *nest));
    terms.push_back(nest->differential());

    return std::make_shared<Product>(terms);
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
