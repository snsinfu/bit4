#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Decl {
    virtual ~Decl() = default;
};

struct Stmt {
    virtual ~Stmt() = default;
};

struct Expr {
    virtual ~Expr() = default;
};

struct VarDecl : Decl {
    explicit VarDecl(std::string const& name, std::shared_ptr<Expr> const& init) : name{name}, init{init} {
    }
  private:
    std::string           name;
    std::shared_ptr<Expr> init;
};

struct DeclStmt : Stmt {
    explicit DeclStmt(std::shared_ptr<Decl> const& decl) : decl{decl} {
    }
    std::shared_ptr<Decl> get_decl() const {
        return decl;
    }
  private:
    std::shared_ptr<Decl> decl;
};

struct ValueExpr : Expr {
    explicit ValueExpr(long long value) : value{value} {
    }
    long long get_value() const {
        return value;
    }
  private:
    long long value;
};

int main() {
    auto init = std::make_shared<ValueExpr>(12345);
    auto var = std::make_shared<VarDecl>("x", init);
    auto stmt = std::make_shared<DeclStmt>(var);
}
