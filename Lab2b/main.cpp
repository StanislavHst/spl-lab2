#include <iostream>
#include <fstream>
#include <windows.h>
#include "../Lexer.h"

Lex current;
bool error = false;
string errorText;
int mode;
int letterPos = 0;
ifstream tokens;
int tokenRow = 0;
int lastPosition = 0;
bool tokenFileError = false;

Lex fileError() {
    error = true;
    tokenFileError = true;
    errorText = "Помилка файлу лексем";
    return {lineNumber, END, "", 1};
}

Lex get_token() {
    if (mode == 1) {
        return scanToken();
    }

    Lex lex;
    string name;
    if (!(tokens >> lex.num >> lex.position >> name >> lex.value)) {
        return fileError();
    }
    if (tokenRow == 0) {
        tokenRow = lex.num;
    }
    if (lex.num < 1 || lex.num != tokenRow || lex.position < 1 || lex.position < lastPosition) {
        return fileError();
    }

    lex.token = tokenType(lex.value);
    if (name == "END" && lex.value == "-") {
        lex.token = END;
    }
    if (name != tokenName(lex.token)) {
        return fileError();
    }
    lastPosition = lex.position;
    if (lex.token != END) {
        lastPosition += (int)lex.value.length();
    }
    return lex;
}

void nextToken() {
    current = get_token();
    letterPos = 0;
    if (!error && current.token == UNKNOWN) {
        error = true;
        errorText = "Лексична помилка: " + current.value;
    }
}

void syntaxError(string expected) {
    error = true;
    errorText = "Синтаксична помилка: потрібен " + expected;
}

void match(Token type, string expected) {
    if (error) {
        return;
    }
    if (current.token != type) {
        syntaxError(expected);
        return;
    }
    nextToken();
}

void J() {
    if (error) {
        return;
    }
    if (current.token != IDEN || !isLetterABCD(current.value[letterPos])) {
        syntaxError("символ a, b, c або d");
        return;
    }
    letterPos++;
}

void I() {
    if (error) {
        return;
    }
    J();
    if (error) {
        return;
    }
    if (letterPos < (int)current.value.length()) {
        I();
    }
    else {
        nextToken();
        if (!error && current.token == IDEN) {
            I();
        }
    }
}

void A() {
    if (error) {
        return;
    }
    I();
    if (!error && current.token == DOT) {
        match(DOT, ".");
        A();
    }
}

void B() {
    if (error || current.token == END) {
        return;
    }
    I();
    match(SLASH, "/");
    if (!error) {
        B();
    }
}

void P() {
    match(PROTOCOL, "протокол http, https або ftp");
}

void S() {
    P();
    match(COLON, ":");
    match(SLASH, "/");
    match(SLASH, "/");
    A();
    match(SLASH, "/");
    B();
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    cout << "1 - речення, 2 - лексеми: ";
    if (!(cin >> mode) || (mode != 1 && mode != 2)) {
        cout << "Потрібно вибрати 1 або 2" << endl;
        return 1;
    }
    ifstream input;
    if (mode == 1) {
        input.open("input.txt");
    }
    else {
        tokens.open("../Lab2a/tokens.txt");
    }
    if ((mode == 1 && !input) || (mode == 2 && !tokens)) {
        cout << "Не вдалося відкрити файл. Для режиму 2 спочатку запусти 2а." << endl;
        return 1;
    }

    while (true) {
        if (mode == 1) {
            if (!getline(input, line)) {
                break;
            }
            lineNumber++;
            pos = 0;
            length = (int)line.length();
        } else {
            tokens >> ws;
            if (tokens.peek() == EOF) {
                break;
            }
        }
        tokenRow = 0;
        lastPosition = 0;
        error = false;
        nextToken();
        if (mode == 2) {
            lineNumber = current.num;
        }
        S();
        if (!error && current.token != END) {
            syntaxError("кінець рядка");
        }

        cout << "Рядок " << lineNumber << ": ";
        if (error) {
            cout << "не належить. Позиція " << current.position << ". " << errorText << endl;
        }
        else {
            cout << "належить" << endl;
        }

        if (mode == 2) {
            if (tokenFileError) {
                break;
            }
            while (current.token != END && tokens) {
                current = get_token();
            }
            if (tokenFileError) {
                cout << errorText << endl;
                break;
            }
        }
    }
    if (tokenFileError) {
        return 1;
    }
    return 0;
}
