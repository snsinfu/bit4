#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

// program:
//      toplevel-declaration *
//
// toplevel-declaration:
//      import-declaration
//      function-declaration
//
// import-declaration:
//      "import" "<" identifier ">"
//
// function-declaration:
//      "func" identifier "(" parameterlist ")" function-body
//
// parameter-list:
//      identifier ":" cr-type
//
// function-body:
//      "{" statement * "}"
//
// statement:
//      expression
//
// expression:
//      shift-expression
//
// shift-expression:
//      primary-expression "<<" primary-expression
//
// primary-expression:
//      qualified-identifier
//
// cr-type:
//      type
//      "&" type
//      "&" "const" type
//
// type:
//      qualified-identifier
//
// qualified-identifier:
//      identifier
//      identifier "::" qualified-identifier

namespace cc
{
    enum class token_kind
    {
        error,

        lparen,
        rparen,
        lbrace,
        rbrace,
        langle,
        rangle,
        lshift,
        rshift,
        colon,
        scope,
        ampersand,

        ident,
        kw_import,
        kw_func,
        kw_const,

        end,
    };

    struct token
    {
        token_kind  kind;
        std::string text;

        explicit operator bool() const
        {
            return kind != token_kind::end;
        }
    };

    class lexer
    {
        lexer(lexer const&) = delete;

      public:
        explicit lexer(std::string_view source)
            : source_{source}
        {
        }

        void bind_keyword(std::string const& keyword, token_kind tok)
        {
            keywords_.emplace(keyword, tok);
        }

        token scan()
        {
            skip_space();

            if (source_.empty()) {
                token tok;
                tok.kind = token_kind::end;
                return tok;
            }

            switch (source_.front()) {
            case '(':
                return scan_lparen();

            case ')':
                return scan_rparen();

            case '{':
                return scan_lbrace();

            case '}':
                return scan_rbrace();

            case '<':
                return scan_langle();

            case '>':
                return scan_rangle();

            case ':':
                return scan_colon();

            case '&':
                return scan_ampersand();

            default:
                return scan_word();
            }
        }

      private:
        void skip_space()
        {
            while (!source_.empty() && std::isspace(source_.front())) {
                source_.remove_prefix(1);
            }
        }

        token scan_lparen()
        {
            consume('(');
            return token{token_kind::lparen, "("};
        }

        token scan_rparen()
        {
            consume(')');
            return token{token_kind::rparen, ")"};
        }

        token scan_lbrace()
        {
            consume('{');
            return token{token_kind::lbrace, "{"};
        }

        token scan_rbrace()
        {
            consume('}');
            return token{token_kind::rbrace, "}"};
        }

        token scan_langle()
        {
            consume('<');
            if (try_consume('<')) {
                return token{token_kind::lshift, "<<"};
            }
            return token{token_kind::langle, "<"};
        }

        token scan_rangle()
        {
            consume('>');
            if (try_consume('>')) {
                return token{token_kind::lshift, ">>"};
            }
            return token{token_kind::rangle, ">"};
        }

        token scan_colon()
        {
            consume(':');
            if (try_consume(':')) {
                return token{token_kind::scope, "::"};
            }
            return token{token_kind::colon, ":"};
        }

        token scan_ampersand()
        {
            consume('&');
            return token{token_kind::ampersand, "&"};
        }

        token scan_word()
        {
            auto const is_word = [](char ch) {
                return std::isalpha(ch) || ch == '_';
            };

            if (!peek(is_word)) {
                throw std::runtime_error("unrecognized token");
            }

            std::string word{
                std::begin(source_),
                std::find_if_not(std::begin(source_), std::end(source_), is_word)
            };
            source_.remove_prefix(word.size());

            return tokenize_word(std::move(word));
        }

        token tokenize_word(std::string word) const
        {
            auto kind = token_kind::ident;

            auto const key_token = keywords_.find(word);
            if (key_token != keywords_.end()) {
                kind = key_token->second;
            }

            return token{kind, std::move(word)};
        }

      private:
        void consume(char ch)
        {
            if (!try_consume(ch)) {
                throw std::logic_error("consume");
            }
        }

        bool try_consume(char ch)
        {
            return try_consume([=](char front) {
                return front == ch;
            });
        }

        template<typename Predicate>
        bool try_consume(Predicate pred)
        {
            if (!peek(pred)) {
                return false;
            }
            source_.remove_prefix(1);
            return true;
        }

        template<typename Predicate>
        bool peek(Predicate pred) const
        {
            return !source_.empty() && pred(source_.front());
        }

      private:
        std::string_view source_;
        std::unordered_map<std::string, token_kind> keywords_;
    };

    void run(std::string_view source)
    {
        auto lex = lexer{source};

        lex.bind_keyword("import", token_kind::kw_import);
        lex.bind_keyword("func", token_kind::kw_func);
        lex.bind_keyword("const", token_kind::kw_const);

        while (auto tok = lex.scan()) {
            std::cout << static_cast<int>(tok.kind)
                      << '\t'
                      << tok.text
                      << '\n';
        }
    }
}

int main()
{
    std::string const source = R"-(
import <iostream>
import <string>

func greet(msg: &const std::string)
{
    std::cout << msg
}
)-";
    cc::run(source);
}
