#include <iostream>  
#include <fstream>    
#include <string>      
#include <cctype>      

enum class CharClass {
    LETTER,    
    DIGIT,      
    UNKNOWN,    
    EOF_CLASS  
}; 
enum class TokenCode {
    INT_LIT = 10,   
    IDENT = 11,     
    ASSIGN_OP = 20, 
    ADD_OP = 21,      
    SUB_OP = 22,     
    MULT_OP = 23,     
    DIV_OP = 24,      
    LEFT_PAREN = 25,  
    RIGHT_PAREN = 26, 
    EOF_TOKEN = -1     
}; 
class Lexer {
private:
    std::string& input_string;  
    int current_pos;           
    char nextChar;            
    CharClass charClass;         
    std::string lexeme;         
    TokenCode nextToken;         

    void addChar() {
        lexeme += nextChar;
    } 
    void getChar() {
        if (current_pos < input_string.length()) {
            nextChar = input_string[current_pos++];
            if (std::isalpha(static_cast<unsigned char>(nextChar))) {
                charClass = CharClass::LETTER;            }
            else if (std::isdigit(static_cast<unsigned char>(nextChar))) {
                charClass = CharClass::DIGIT;
            }
            else {
                charClass = CharClass::UNKNOWN;
            }
        }
        else {
            charClass = CharClass::EOF_CLASS;
            nextChar = EOF;
        }
    }
    void getNonBlank() {
        while (std::isspace(static_cast<unsigned char>(nextChar))) {
            if (charClass == CharClass::EOF_CLASS) break;
            getChar();
        } 
    }
    TokenCode lookup(char ch) {
        switch (ch) {
        case '(':
            addChar();
            nextToken = TokenCode::LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = TokenCode::RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = TokenCode::ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = TokenCode::SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = TokenCode::MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = TokenCode::DIV_OP;
            break;
        default:
            addChar();
            nextToken = TokenCode::EOF_TOKEN;
            break;
        }
        return nextToken;
    } 

public:
    Lexer(std::string& input) : input_string(input), current_pos(0) {
        getChar();
    } 
    TokenCode lex() {
        lexeme.clear();
        getNonBlank();
        switch (charClass) {
        case CharClass::LETTER:
            addChar();
            getChar();
            while (charClass == CharClass::LETTER || charClass == CharClass::DIGIT) {
                addChar();
                getChar();
            }
            nextToken = TokenCode::IDENT;
            break;
        case CharClass::DIGIT:
            addChar();
            getChar();
            while (charClass == CharClass::DIGIT) {
                addChar();
                getChar();
            } 
            nextToken = TokenCode::INT_LIT;
            break;
        case CharClass::UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        case CharClass::EOF_CLASS:
            nextToken = TokenCode::EOF_TOKEN;
            lexeme = "EOF";
            break;
        } 
        std::cout << "Next token is: " << static_cast<int>(nextToken)
            << ", and the text of the token is: " << lexeme << std::endl;
        return nextToken;
    } 
    bool isEOF() const {
        return charClass == CharClass::EOF_CLASS && nextToken == TokenCode::EOF_TOKEN;
    } 
}; 
int main() {
    std::string fixedStatement = "sum = (a + b) * 42";

    Lexer lexer(fixedStatement);
    while (!lexer.isEOF()) {
        lexer.lex();
    } 
    return 0;
} 