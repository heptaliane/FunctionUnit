#ifndef FUNCTION_UNIT_H_
#define FUNCTION_UNIT_H_

#include <cstddef>
#include <vector>
#include <string>
#include <memory>

namespace func {


class FunctionUnit {
public:

    virtual double call (double x) const = 0;

    virtual std::shared_ptr<FunctionUnit> differential () const = 0;

    virtual std::shared_ptr<FunctionUnit> clone() const = 0;

    virtual std::string toString () const = 0;
};

class Zero : public FunctionUnit {
public:

    Zero () {}

    double call (double x) const {
        (void)x;
        return 0;
    }

    std::shared_ptr<FunctionUnit> differential () const {
        return std::make_shared<Zero>();
    }

    std::shared_ptr<FunctionUnit> clone() const {
        return std::make_shared<Zero>();
    }

    std::string toString () const { return ""; }

    bool equal (const FunctionUnit &obj) const {
        (void)obj;
        return false;
    }

    bool equal (const Zero &obj) const {
        (void)obj;
        return true;
    }
};

class Base : public FunctionUnit {
public:

    Base () {}

    double call (double x) const {
        (void)x;
        return 1;
    }

    std::shared_ptr<FunctionUnit> differential () const {
        return std::make_shared<Zero>();
    }

    std::shared_ptr<FunctionUnit> clone() const {
        return std::make_shared<Base>();
    }

    std::string toString () const { return "1"; }

    bool equal (const FunctionUnit &obj) const {
        (void)obj;
        return false;
    }

    bool equal (const Base &obj) const {
        (void)obj;
        return true;
    }
};

class Variable : public FunctionUnit {
public:

    Variable () {}

    double call (double x) const { return x; }

    std::shared_ptr<FunctionUnit> differential () const {
        return std::make_shared<Base>();
    }

    std::shared_ptr<FunctionUnit> clone() const {
        return std::make_shared<Variable>();
    }

    std::string toString () const { return "x"; }

    bool equal (const Variable &obj) const {
        (void)obj;
        return true;
    }

    bool equal (const FunctionUnit &obj) const {
        (void)obj;
        return false;
    }
};

class Summation : public FunctionUnit {
public:

    Summation () {};

    Summation (const Summation &obj);

    explicit Summation (
            const std::vector<std::shared_ptr<FunctionUnit> > &funits);

    void init (const std::vector<std::shared_ptr<FunctionUnit> > &funits);

    double call (double x) const;

    std::shared_ptr<FunctionUnit> differential () const;

    std::shared_ptr<FunctionUnit> clone () const;

    std::string toString () const;

private:

    std::vector<std::shared_ptr<FunctionUnit> > terms;
};

class Product : public FunctionUnit {
public:

    Product () {};

    Product (const Product &obj);

    explicit Product (const std::vector<std::shared_ptr<FunctionUnit> > &obj);

    void init (const std::vector<std::shared_ptr<FunctionUnit> > &obj);

    double call (double x) const;

    std::shared_ptr<FunctionUnit> differential () const;

    std::shared_ptr<FunctionUnit> clone () const;

    std::string toString () const;

private:

    bool isZero = false;

    std::vector<std::shared_ptr<FunctionUnit> > terms;

    void concat (const FunctionUnit &obj);

    void concat (const Zero &obj);

    void concat (const Base &obj);

    void concat (const Product &obj);
};

class Polynominal : public FunctionUnit {
public:

    Polynominal () {};

    explicit Polynominal (
            const std::vector<double> &coefficient,
            const FunctionUnit &funit);

    Polynominal (const Polynominal &obj);

    void init (
            const std::vector<double> &coefficient,
            const FunctionUnit &funit);

    double call (double x) const;

    std::shared_ptr<FunctionUnit> differential () const;

    std::shared_ptr<FunctionUnit> clone() const;

    std::string toString () const;

private:

    std::vector<double> coef;

    std::shared_ptr<FunctionUnit> nest;
};


}

#endif
