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

        //continues iterating so long as the peaking ahead returns a value;
        while(peak().has_value()) {
            //checks if a letter has been entered
            if (std::isalpha(peak().value())) {
                buf.push_back(consume());
                //so it can continue adding the full phrase to the buffer
                while (peak().has_value() && std::isalnum(peak().value())) {
                    buf.push_back(consume());
                }
                //if the contents of the discovered word through iterating makes "exit"
                if (buf == "exit") {
                    tokens.push_back({.type = TokenType::exit}); //it is an exit type token
                    buf.clear();
                    continue;
                } else {
                    std::cerr << "You messed up." << std::endl; //makeshift error statement
                    exit(EXIT_FAILURE);
                }
            }
            //checks if a lexer starts with a digit
            else if (std::isdigit(peak().value())) {
                buf.push_back(consume());
                //and iterates to consume the entire number in the buffer
                while (peak().has_value() && std::isdigit(peak().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear(); //buffer always clears for next iteration once token is saved
                continue;
            }
            else if (peak().value() == ';') { //semicolon token
                consume();
                tokens.push_back({.type = TokenType::semi});
                continue;
            }
            else if (std::isspace(peak().value())) { //spaces still need to be consumed
                consume();
                continue;
            } else { //blanket error message
                std::cerr << "You messed up." << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] inline std::optional<char> peak(int ahead = 1) const {
        if (m_index + ahead > m_src.length()) {
            return {};
        } else {
            return m_src.at(m_index);
        }
    }
    
    inline char consume() {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    int m_index;
};

#endif