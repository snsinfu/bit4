#if 0
if ! command -v llvm-config; then
    alias llvm-config=/usr/local/opt/llvm/bin/llvm-config
fi
exec c++ -std=c++14 \
         $0 "$@"    \
         $(llvm-config --cxxflags --ldflags --libs --system-libs codegen)
#endif

#include <iostream>

#include <llvm/Support/raw_ostream.h>   // raw_ostream

#include <llvm/IR/IRBuilder.h>          // IRBuilder
#include <llvm/IR/IRPrintingPasses.h>   // PrintModulePass
#include <llvm/IR/LLVMContext.h>        // LLVMContext
#include <llvm/IR/PassManager.h>        // PassManager, AnalysisManager

#include <llvm/IR/BasicBlock.h>         // BasicBlock
#include <llvm/IR/Function.h>           // Function
#include <llvm/IR/Module.h>             // Module

#include <llvm/IR/DerivedTypes.h>       // FunctionType
#include <llvm/IR/Type.h>               // Type

int main()
{
    using namespace llvm;

    LLVMContext ctx;
    IRBuilder<> builder{ctx};
    Module mod{"my_module", ctx};

    Function* func = [&] {
        // These pointers are all non-owning observers.
        Type* par_types[] = {
            Type::getInt64Ty(ctx)
        };
        Type* res_type = Type::getInt64Ty(ctx);
        FunctionType* fun_type = FunctionType::get(res_type, par_types, false);

        // This func pointer is also non-owning. Do not delete it. Although
        // Function::Create() allocates a Function object using new, the newly
        // allocated object will be owned by the Module object mod. [1,2,3]
        Function* func = Function::Create(
            fun_type, Function::ExternalLinkage, "my_func", &mod);
        // ----
        // [1]: lib/IR/Function.cpp
        // The constructor of Function appends the pointer to the constructed
        // object to FunctionList of the module passed to argument:
        //  ParentModule->getFunctionList().push_back(this);
        //
        // [2]: include/llvm/IR/Module.hpp
        // The FunctionList is a FunctionListType, which is a specialization of
        // SymbolTableList, which is a subclass of iplist_impl [3].
        //
        // [3]: include/llvm/ADT/ilist.h
        // iplist_impl is an intrusive list that owns added nodes. That is, a
        // node, of type Function* here, is deleted on erasure.
        // ----
        return func;
    }();

    // TODO: Add instructions to the block...
    BasicBlock* block = BasicBlock::Create(ctx, "body", func);
    builder.SetInsertPoint(block);

    // Recent LLVM seems to make use of static polymorphism (generics). Here
    // PrintModulePass is passed by value.
    ModulePassManager pm;
    ModuleAnalysisManager am;
    {
        llvm::raw_fd_ostream stream{0, false};
        pm.addPass(PrintModulePass{stream}); // <-- pass by value
        pm.run(mod, am);
    }
}
