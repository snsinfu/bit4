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
    std::vector<clang::QualType> parameterTypes = {
        context.DoubleTy,
        context.DoubleTy,
    };
    clang::QualType resultType = context.IntTy;
    clang::QualType functionType = context.getFunctionType(
        resultType,
        parameterTypes,
        clang::FunctionProtoType::ExtProtoInfo{}
    );

    // Build the declaration
    clang::FunctionDecl* functionDecl = clang::FunctionDecl::Create(
            context, declarations, clang::SourceLocation{}, clang::SourceLocation{},
            clang::DeclarationName{&identifiers.get("myFunction")}, functionType, nullptr, clang::SC_None);
    std::vector<clang::ParmVarDecl*> parameters = {
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

    declarations->addDecl(functionDecl);
}

void printAST(clang::ASTContext const& context)
{
    context.getTranslationUnitDecl()->print(llvm::outs());
    llvm::outs() << '\n';
    context.getTranslationUnitDecl()->dump(llvm::outs());
}
