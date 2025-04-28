#ifndef __GENERATION_HPP__
#define __GENERATION_HPP__

#include <iostream>
#include <sstream>
#include <string>
#include "./parser.hpp"

class Generator {
public:
    inline explicit Generator(NodeExit root)
        : m_root(std::move(root))
    {
    }

    [[nodiscard]] std::string generate() const{
        std::stringstream output;
        output << "global _start\n_start:\n";
        output << "    mov rax, 60\n"; //moves instruction 60 (exit) to syscall
        output << "    mov rdi, " << m_root.expr.int_lit.value.value() << "\n";
        output << "    syscall"; //runs syscall
        return output.str();
    }

private:
    const NodeExit m_root;
};

#endif