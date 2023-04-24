#include <iostream>

int main()
{
    float a,b=15.7;
    std::cout << "Lab1 - Hi ELF74 \n";
    std::cout << "Date: " << __DATE__ << "\n";
    std::cout << "Time: " << __TIME__ << "\n";
    std::cout << "File: " << __FILE__ << "\n";
    std::cout <<"Digite um numero: \n";
    std::cin >> a;
    a=a+b;
    std::cout<<"Resultado [a+b]:";
    std::cout<<a;
    return 0;
}