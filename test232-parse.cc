#include <algorithm>
#include <cctype>
#include <deque>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

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

    class identifier
    {
      public:
        explicit identifier(std::string const& name)
            : name_{name}
        {
        }

      private:
        std::string name_;
    };

    class type
    {
      public:
        explicit type(identifier const& ident)
            : ident_{ident}
        {
        }

      private:
        identifier ident_;
    };

    class declaration
    {
      public:
        virtual ~declaration() = default;
    };

    class import_declaration : public declaration
    {
    };

    class function_declaration : public declaration
    {
    };

    class program
    {
      public:
        void add_declaration(std::unique_ptr<declaration> decl)
        {
            decls_.push_back(std::move(decl));
        }

        void run() const
        {
        }

      private:
        std::vector<std::unique_ptr<declaration>> decls_;
    };

    class parser
    {
      public:
        explicit parser(lexer lex)
            : lex_{lex}
        {
        }

        program parse()
        {
            program prog;

            while (peek()) {
                prog.add_declaration(parse_toplevel_declaration());
            }

            return prog;
        }

      private:
        std::unique_ptr<declaration> parse_toplevel_declaration()
        {
            switch (peek().kind) {
            case token_kind::kw_import:
                return parse_import_declaration();

            case token_kind::kw_func:
                return parse_function_declaration();

            default:
                throw std::runtime_error("syntax error: program");
            }
        }

        std::unique_ptr<import_declaration> parse_import_declaration()
        {
            if (scan().kind != token_kind::kw_import) {
                throw std::logic_error("parse_import_declaration");
            }

            if (scan().kind != token_kind::langle) {
                throw std::runtime_error("syntax error: import");
            }

            if (scan().kind != token_kind::ident) {
                throw std::runtime_error("syntax error: import");
            }

            if (scan().kind != token_kind::rangle) {
                throw std::runtime_error("syntax error: import");
            }

            return nullptr;
        }

        std::unique_ptr<function_declaration> parse_function_declaration()
        {
            if (scan().kind != token_kind::kw_func) {
                throw std::logic_error("parse_function_declaration");
            }

            if (scan().kind != token_kind::ident) {
                throw std::runtime_error("syntax error: func");
            }

            if (scan().kind != token_kind::lparen) {
                throw std::runtime_error("syntax error: func");
            }

            parse_parameter_list();
            parse_function_body();

            return nullptr;
        }

        void parse_parameter_list()
        {
            if (scan().kind != token_kind::ident) {
                throw std::runtime_error("syntax error: parameter list: ident");
            }

            if (scan().kind != token_kind::colon) {
                throw std::runtime_error("syntax error: parameter list: colon");
            }

            parse_qual_type();

            if (scan().kind != token_kind::rparen) {
                throw std::runtime_error("syntax error: parameter list: rparen");
            }
        }

        void parse_qual_type()
        {
            switch (peek().kind) {
            case token_kind::ampersand:
                return parse_ref_type();

            case token_kind::kw_const:
                return parse_const_type();

            case token_kind::ident:
                return parse_naked_type();

            default:
                throw std::runtime_error("syntax error: qual-type");
            }
        }

        void parse_ref_type()
        {
            if (scan().kind != token_kind::ampersand) {
                throw std::logic_error("parse_ref_type");
            }

            switch (peek().kind) {
            case token_kind::kw_const:
                return parse_const_type();

            case token_kind::ident:
                return parse_naked_type();

            default:
                throw std::runtime_error("syntax error: ref-type");
            }
        }

        void parse_const_type()
        {
            if (scan().kind != token_kind::kw_const) {
                throw std::logic_error("parse_const_type");
            }

            parse_naked_type();
        }

        void parse_naked_type()
        {
            parse_qual_name();
        }

        void parse_function_body()
        {
            if (scan().kind != token_kind::lbrace) {
                throw std::runtime_error("syntax error: functino-body: lbrace");
            }

            parse_statement();

            if (scan().kind != token_kind::rbrace) {
                throw std::runtime_error("syntax error: function-body: rbrace");
            }
        }

        void parse_statement()
        {
            parse_expression();
        }

        void parse_expression()
        {
            parse_primary_expression();

            switch (peek().kind) {
              case token_kind::lshift:
                parse_lshift_expression();
                break;

              default:
                break;
            }
        }

        void parse_lshift_expression()
        {
            if (scan().kind != token_kind::lshift) {
                throw std::runtime_error("syntax error: lshift-expression");
            }

            parse_primary_expression();
        }

        void parse_primary_expression()
        {
            parse_qual_name();
        }

        void parse_qual_name()
        {
            for (;;) {
                if (scan().kind != token_kind::ident) {
                    throw std::runtime_error("syntax error: qual-name");
                }

                if (peek().kind != token_kind::scope) {
                    break;
                }
                scan();
            }
        }

        token const& peek()
        {
            auto& tok = scan();
            unscan(tok);
            return tok;
        }

        token const& scan()
        {
            if (!token_buffer_.empty()) {
                token_buffer_.pop_front();
            }

            token_buffer_.push_back(lex_.scan());

            return current_token();
        }

        void unscan(token const& tok)
        {
            token_buffer_.push_front(tok);
        }

        token const& current_token() const
        {
            return token_buffer_.front();
        }

      private:
        lexer lex_;
        std::deque<token> token_buffer_;
    };

    void run(std::string_view source)
    {
        auto lex = lexer{source};

        lex.bind_keyword("import", token_kind::kw_import);
        lex.bind_keyword("func", token_kind::kw_func);
        lex.bind_keyword("const", token_kind::kw_const);

        auto par = parser{lex};
        auto prog = par.parse();

        prog.run();
    }
}

int main()
{
    std::string const source = R"-(
import <iostream>
import <string>

func print(msg: &const std::string)
{
    std::cout << msg
}
)-";
    try {
        cc::run(source);
    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
    }
}
