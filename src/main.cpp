#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "./tokenization.hpp"

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
        if (token.type == TokenType::exit) { //if the token is an exit, it checks to see if the appropriate tokens follow
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
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.tokenize();

    {
        std::fstream file("out.asm", std::ios::out); //asm file created and written to
        file << tokens_to_asm(tokens);
    }

    //asm file assembled and executed
    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    
    return EXIT_SUCCESS;
}