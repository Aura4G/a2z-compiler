#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>

#include "./generation.hpp"

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

    Parser parser(std::move(tokens));
    std::optional<NodeProg> prog = parser.parse_prog();

    if (!prog.has_value()) {
        std::cerr << "Invalid program" << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(prog.value());
    {
        std::fstream file("out.asm", std::ios::out); //asm file created and written to
        file << generator.gen_prog();
    }

    //asm file assembled and executed
    system("nasm -felf64 out.asm");
    system("ld -o out out.o");
    
    return EXIT_SUCCESS;
}