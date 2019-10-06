#include <minisat/core/Solver.h>

#include <iostream>


int main()
{
    using Minisat::mkLit;
    using Minisat::lbool;

    Minisat::Solver solver;

    auto A = solver.newVar();
    auto B = solver.newVar();
    auto C = solver.newVar();

    // (A∨B∨C) ∧ (¬A∨B∨C) ∧ (A∨¬B∨C) ∧ (A∨B∨¬C)
    solver.addClause( mkLit(A),  mkLit(B),  mkLit(C));
    solver.addClause(~mkLit(A),  mkLit(B),  mkLit(C));
    solver.addClause( mkLit(A), ~mkLit(B),  mkLit(C));
    solver.addClause( mkLit(A),  mkLit(B), ~mkLit(C));

    auto sat = solver.solve();
    if (sat) {
        std::cout << "A = " << (solver.modelValue(A) == l_True) << '\n';
        std::cout << "B = " << (solver.modelValue(B) == l_True) << '\n';
        std::cout << "C = " << (solver.modelValue(C) == l_True) << '\n';
    } else {
        std::cout << "UNSAT\n";
    }
}
