#include <iostream>
#include <fstream>
#include <windows.h>
#include "../Lexer.h"

Lex get_token() {
    return scanToken();
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    ifstream input("input.txt");
    if (!input) {
        cout << "Не вдалося відкрити input.txt" << endl;
        return 1;
    }
    ofstream output("output.txt");
    ofstream tokens("tokens.txt");
    if (!output || !tokens) {
        cout << "Не вдалося відкрити файл" << endl;
        return 1;
    }

    while (getline(input, line)) {
        lineNumber++;
        pos = 0;
        length = (int)line.length();
        while (true) {
            Lex lex = get_token();
            output << "<" << lex.num << ", " << tokenName(lex.token)
                   << ", " << lex.value << ">" << endl;
            tokens << lex.num << " " << lex.position << " " << tokenName(lex.token) << " ";
            if (lex.token == END) {
                tokens << "-" << endl;
            }
            else {
                tokens << lex.value << endl;
            }
            if (lex.token == UNKNOWN) {
                cout << "Рядок " << lex.num << ", позиція " << lex.position
                     << ": невідома лексема " << lex.value << endl;
                tokens << lex.num << " " << pos + 1 << " END -" << endl;
                break;
            }
            if (lex.token == END) {
                break;
            }
        }
        output << endl;
    }
    cout << "Готово. Лексеми у output.txt, для 2б — у tokens.txt" << endl;
    return 0;
}
