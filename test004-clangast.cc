#if 0
exec c++                                                            \
  -std=c++14 -pedantic -Wall -Wextra -O2 -g                         \
  "$0" "$@"                                                         \
  $(llvm-config --cxxflags --ldflags --libs --system-libs)          \
  -lclangFrontend -lclangSerialization -lclangDriver -lclangTooling \
  -lclangParse -lclangSema -lclangStaticAnalyzerFrontend            \
  -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore           \
  -lclangAnalysis -lclangRewriteFrontend -lclangRewrite -lclangEdit \
  -lclangAST -lclangLex -lclangBasic
#endif

#include <memory>
#include <vector>

#include <llvm/Support/Host.h>

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/ExprCXX.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/IdentifierTable.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>

void initCompiler(clang::CompilerInstance& compiler);
void buildAST(clang::ASTContext& context);
void printAST(clang::ASTContext const& context);

int main()
{
    clang::CompilerInstance compiler;
    initCompiler(compiler);

    clang::ASTContext& context = compiler.getASTContext();
    buildAST(context);
    printAST(context);
}

void initCompiler(clang::CompilerInstance& compiler)
{
    compiler.createDiagnostics();
    compiler.createFileManager();
    compiler.createSourceManager(compiler.getFileManager());

    auto targetOptions = std::make_shared<clang::TargetOptions>();
    targetOptions->Triple = llvm::sys::getProcessTriple();
    auto targetInfo = clang::TargetInfo::CreateTargetInfo(compiler.getDiagnostics(), targetOptions);
    compiler.setTarget(targetInfo);

    compiler.createPreprocessor(clang::TU_Complete);
    compiler.createASTContext();
}

void buildAST(clang::ASTContext& context)
{
    clang::IdentifierTable& identifiers = context.Idents;
    clang::TranslationUnitDecl* declarations = context.getTranslationUnitDecl();

    // Build the function type
    std::vector<clang::QualType> const parameterTypes = {
        context.DoubleTy,
        context.DoubleTy,
    };
    clang::QualType const resultType = context.IntTy;
    clang::QualType const functionType = context.getFunctionType(
        resultType,
        parameterTypes,
        clang::FunctionProtoType::ExtProtoInfo{}
    );

    // Build the declaration
    clang::FunctionDecl* const functionDecl = clang::FunctionDecl::Create(
        context, declarations, clang::SourceLocation{}, clang::SourceLocation{},
        clang::DeclarationName{&identifiers.get("myFunction")}, functionType,
        nullptr, clang::SC_None
    );
    std::vector<clang::ParmVarDecl*> const parameters = {
        clang::ParmVarDecl::Create(
            context, functionDecl, clang::SourceLocation{}, clang::SourceLocation{},
            &identifiers.get("x"), parameterTypes[0], nullptr, clang::SC_None, nullptr
        ),
        clang::ParmVarDecl::Create(
            context, functionDecl, clang::SourceLocation{}, clang::SourceLocation{},
            &identifiers.get("y"), parameterTypes[1], nullptr, clang::SC_None, nullptr
        ),
    };
    functionDecl->setParams(parameters);

    // Build the function body
    clang::CompoundStmt* const body = new(context) clang::CompoundStmt{clang::SourceLocation{}};

    clang::VarDecl* const sumVarDecl = clang::VarDecl::Create(
        context, functionDecl, clang::SourceLocation{}, clang::SourceLocation{},
        &identifiers.get("sum"), context.DoubleTy, nullptr, clang::SC_None
    );
    sumVarDecl->setInit(
        new(context) clang::BinaryOperator{
            new(context) clang::DeclRefExpr{
                parameters[0], false, parameterTypes[0], clang::VK_LValue, clang::SourceLocation{}
            },
            new(context) clang::DeclRefExpr{
                parameters[1], false, parameterTypes[1], clang::VK_LValue, clang::SourceLocation{}
            },
            clang::BO_Add, // BinaryOperatorKind in <clang/AST/OperationKinds.h>
            context.DoubleTy, clang::VK_RValue, clang::OK_Ordinary, clang::SourceLocation{},
            /*fpContractable*/false
        }
    );

    clang::CXXStaticCastExpr* const resultExpr = clang::CXXStaticCastExpr::Create(
        context, resultType, clang::VK_RValue, clang::CK_FloatingToIntegral,
        new(context) clang::DeclRefExpr{
            sumVarDecl, false, sumVarDecl->getType(), clang::VK_LValue, clang::SourceLocation{}
        },
        nullptr, context.CreateTypeSourceInfo(resultType),
        clang::SourceLocation{}, clang::SourceLocation{}, clang::SourceRange{}
    );

    std::vector<clang::Stmt*> const statements = {
        new(context) clang::DeclStmt{
            clang::DeclGroupRef{sumVarDecl},
            clang::SourceLocation{},
            clang::SourceLocation{}
        },
        new(context) clang::ReturnStmt{
            clang::SourceLocation{},
            resultExpr,
            nullptr
        },
    };
    body->setStmts(context, statements);
    functionDecl->setBody(body);

    declarations->addDecl(functionDecl);
}

void printAST(clang::ASTContext const& context)
{
    context.getTranslationUnitDecl()->print(llvm::outs());
    llvm::outs() << '\n';
    context.getTranslationUnitDecl()->dump(llvm::outs());
}
