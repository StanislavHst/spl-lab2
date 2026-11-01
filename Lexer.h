#pragma once
#include <string>
using namespace std;

enum Token { PROTOCOL, IDEN, COLON, SLASH, DOT, END, UNKNOWN };

struct Lex {
    int num;
    Token token;
    string value;
    int position;
};

string line;
int lineNumber = 0;
int pos = 0;
int length = 0;

bool isLetterABCD(char c) {
    return c == 'a' || c == 'b' || c == 'c' || c == 'd';
}

bool isIdentifier(string word) {
    for (int i = 0; i < (int)word.length(); i++) {
        if (!isLetterABCD(word[i])) {
            return false;
        }
    }
    return word != "";
}

Token tokenType(string word) {
    if (word == "http" || word == "https" || word == "ftp") {
        return PROTOCOL;
    }
    if (isIdentifier(word)) {
        return IDEN;
    }
    if (word == ":") {
        return COLON;
    }
    if (word == "/") {
        return SLASH;
    }
    if (word == ".") {
        return DOT;
    }
    return UNKNOWN;
}

string tokenName(Token t) {
    switch (t) {
        case PROTOCOL: return "PROTOCOL";
        case IDEN: return "IDEN";
        case COLON: return "COLON";
        case SLASH: return "SLASH";
        case DOT: return "DOT";
        case END: return "END";
        default: return "UNKNOWN";
    }
}

Lex scanToken() {
    while (pos < length && (line[pos] == ' ' || line[pos] == '\t' || line[pos] == '\r')) {
        pos++;
    }
    int start = pos + 1;
    if (pos == length) {
        return {lineNumber, END, "", start};
    }

    string word = "";
    if (line.substr(pos, 5) == "https") {
        word = "https";
        pos += 5;
    } else if (line.substr(pos, 4) == "http") {
        word = "http";
        pos += 4;
    } else if (line.substr(pos, 3) == "ftp") {
        word = "ftp";
        pos += 3;
    } else if (isLetterABCD(line[pos])) {
        while (pos < length && isLetterABCD(line[pos])) {
            word += line[pos];
            pos++;
        }
    } else {
        word += line[pos];
        pos++;
    }
    return {lineNumber, tokenType(word), word, start};
}
