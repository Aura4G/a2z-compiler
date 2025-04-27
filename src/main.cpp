#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

/**
 * Enum class to represent all the different categories of
 * lexers possible from a single word or expression in a file
 * 
 * @author Aria Noroozi
 */
enum class TokenType {
    _return,
    int_lit,
    semi
};

/**
 * Struct to contain the type of token present in a given file, 
 * along with a value variable in the case of quantifiable data
 * 
 * @author Aria Noroozi
 */
struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

/**
 * Function that takes the read text from an a2z file and returns a vector of tokens, 
 * of which corresponds to the kind of word in the string.
 * 
 * @param str The stream of contents from an entered file, converted to a string
 * @return The tokens in order of appearance in the string
 * @author Aria Noroozi
 */
std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens {}; //return vector

    std::string buf {};
    //iterates character by character through string
    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        //checks if a letter has been entered
        if (std::isalpha(c)) {
            buf.push_back(c);
            i++;
            //so it can continue adding a token to the vector
            while (std::isalnum(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;

            //if the contents of the discovered word through iterating makes "return"
            if (buf == "return") {
                tokens.push_back({.type = TokenType::_return}); //it is a _return type token
                buf.clear();
                continue;
            } else {
                std::cerr << "You messed up." << std::endl; //makeshift error statement
                exit(EXIT_FAILURE);
            }
        }
        //checks if a lexer starts with a digit
        else if (std::isdigit(c)) {
            buf.push_back(c);
            i++;
            //and iterates to achieve the entire number in the buffer
            while (std::isdigit(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::int_lit, .value = buf});
            buf.clear(); //buffer always clears for next iteration once token is saved
        }
        else if (c == ';') { //semicolon token
            tokens.push_back({.type = TokenType::semi});
            continue;
        }
        else if (std::isspace(c)) { //spaces are ignored
            continue;
        }
        else { //blanket error message
            std::cerr << "You messed up." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

/**
 * Converts a vector of tokens to a string equivalent in assembly language
 * 
 * @param tokens The vector of tokens
 * @return The assembly language equivalent of the tokenised file text
 * @author Aria Noroozi
 */
std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;
    output << "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++) {
        const Token& token = tokens.at(i);
        if (token.type == TokenType::_return) { //if the token is a return, it checks to see if the appropriate tokens follow
            if (i+1 < tokens.size() && tokens.at(i+1).type == TokenType::int_lit) {
                if (i+2 < tokens.size() && tokens.at(i+2).type == TokenType::semi) {
                    output << "    mov rax, 60\n"; //moves instruction 60 (exit) to syscall
                    output << "    mov rdi, " << tokens.at(i+1).value.value() << "\n";
                    output << "    syscall"; //runs syscall
                }
            }
        }
    }
    return output.str();
}

/**Main function taking in a file */
int main(int argc, char** argv) {
    if (argc != 2) { //Condition to ensure two arguments exactly are entered
        std::cerr << "Incorrect usage" << std::endl;
        std::cerr << "a2z <input.a2z>" << std::endl;
        return EXIT_FAILURE;
    }

    //puts file contents into a buffer and converts said contents into a string
    std::string contents;
    {
        std::stringstream contents_stream;
        std::ifstream input(argv[1]);
        contents_stream << input.rdbuf();
        contents= contents_stream.str();
    }

    //lexical analysis
    std::vector<Token> tokens = tokenize(contents);

    {
        std::fstream file("out.asm", std::ios::out); //asm file created and written to
        file << tokens_to_asm(tokens);
    }

    //asm file assembled and executed
    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    
    return EXIT_SUCCESS;
}