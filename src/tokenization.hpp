#ifndef __TOKENIZATION_HPP__
#define __TOKENIZATION_HPP__

#include <string>
#include <vector>
#include <optional>

/**
 * Enum class to represent all the different categories of
 * lexers possible from a single word or expression in a file
 * 
 * @author Aria Noroozi
 */
enum class TokenType {
    exit,
    int_lit,
    semi,
    open_paren,
    close_paren,
    ident,
    let,
    eq
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


class Tokenizer {
public:
    inline explicit Tokenizer(const std::string& src)
        : m_src(std::move(src)), m_index(0)
    {

    }

    /**
    * Function that takes the read text from an a2z file and returns a vector of tokens, 
    * of which corresponds to the kind of word in the string.
    * 
    * @return The tokens in order of appearance in the string
    * @author Aria Noroozi
    */
    inline std::vector<Token> tokenize() {
        std::vector<Token> tokens {}; //return vector

        std::string buf {};

        //continues iterating so long as the peeking ahead returns a value;
        while(peek().has_value()) {
            //checks if a letter has been entered
            if (std::isalpha(peek().value())) {
                buf.push_back(consume());
                //so it can continue adding the full phrase to the buffer
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(consume());
                }
                //if the contents of the discovered word through iterating makes a key word
                if (buf == "exit") {
                    tokens.push_back({.type = TokenType::exit}); //it is an exit type token
                    buf.clear();
                    continue;
                }
                else if (buf == "let") {
                    tokens.push_back({.type = TokenType::let}); //it is an let type token
                    buf.clear();
                    continue;
                } else {
                    tokens.push_back({.type = TokenType::ident, .value = buf});
                    buf.clear();
                    continue;
                }
            }
            //checks if a lexer starts with a digit
            else if (std::isdigit(peek().value())) {
                buf.push_back(consume());
                //and iterates to consume the entire number in the buffer
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear(); //buffer always clears for next iteration once token is saved
                continue;
            }
            else if (peek().value() == '(') {
                consume();
                tokens.push_back({.type = TokenType::open_paren});
                continue;
            }
            else if (peek().value() == ')') {
                consume();
                tokens.push_back({.type = TokenType::close_paren});
                continue;
            }
            else if (peek().value() == '=') {
                consume();
                tokens.push_back({.type = TokenType::eq});
                continue;
            }
            else if (peek().value() == ';') { //semicolon token
                consume();
                tokens.push_back({.type = TokenType::semi});
                continue;
            }
            else if (std::isspace(peek().value())) { //spaces still need to be consumed
                consume();
                continue;
            } else { //blanket error message
                std::cerr << "Unrecognised token." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        m_index = 0;
        return tokens;
    }

private:
    /** 
     * peaks however many characters ahead in the input string
     * @param offset the jump ahead where the character gets peeked
    */
    [[nodiscard]] inline std::optional<char> peek(int offset = 0) const {
        if (m_index + offset >= m_src.length()) {
            return {};
        } else {
            return m_src.at(m_index + offset);
        }
    }
    
    /* adds the current character to the buffer and moves along the string by 1 */
    inline char consume() {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    int m_index;
};

#endif