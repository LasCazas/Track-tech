#include <iostream>
#include <cmath>

int binarioParaDecimal(int binario) {
    int decimal = 0;
    int expoente = 0;

    while (binario != 0) {
        int digito = binario % 10;
        decimal += digito * pow(2, expoente);
        binario /= 10;
        expoente++;
    }

    return decimal;
}

int main() {
    //int binario;
    while (true){
    int SenOE,SenE, SenC, SenD, SenOD;
    std::cout << "Valor Oposto Esquerdo número binário: ";
    std::cin >> SenOE;
    std::cout << "Valor Esquerdo número binário: ";
    std::cin >> SenE;
    std::cout << "Valor Central número binário: ";
    std::cin >> SenC;
    std::cout << "Valor Direito número binário: ";
    std::cin >> SenD;
    std::cout << "Valor Oposto Direito número binário: ";
    std::cin >> SenOD;
    
    int decimal = binarioParaDecimal(SenOE * 10000  + SenE * 1000 + SenC * 100 + SenD * 10 + SenOD);
    std::cout << "====================================\n";
    std::cout << "O número decimal correspondente é: " << decimal << std::endl;
    std::cout << "====================================\n";
    }
    return 0;
}
