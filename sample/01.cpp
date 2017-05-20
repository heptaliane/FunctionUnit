#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <memory>

#include "../FunctionUnit.h"

int main() {

    std::vector<double> coef;

    coef.push_back(0.0);
    coef.push_back(1.0);
    coef.push_back(1.5);

    // f(x) = 1.5 x ^ 2 + x
    func::Polynominal f(coef, func::Variable());

    // g(x) = f(f(x))
    func::Polynominal g(coef, *f.clone());

    // stringify Polynominal instance
    std::cout << "f(x) = " << f.toString() << std::endl;
    std::cout << "g(x) = f(f(x)) = " << g.toString() << std::endl;
    std::cout << "g'(x) = " << g.differential()->toString() << std::endl;

    // assign value
    std::cout << "f(2.0) = " << f.call(2.0) << std::endl;
    std::cout << "f'(2.0) = " << f.differential()->call(2.0) << std::endl;
    std::cout << "f(f(2.0)) = " << f.call(f.call(2.0)) << std::endl;
    std::cout << "f'(f(2.0)) = " <<
        f.differential()->call(f.call(2.0)) << std::endl;
    std::cout << "g(2.0) = " << g.call(2.0) << std::endl;
    std::cout << "g'(2.0) = " << g.differential()->call(2.0) << std::endl;

    return 0;
}
