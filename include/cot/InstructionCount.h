
#ifndef COT_INSTRUCTIONCOUNT_H
#define COT_INSTRUCTIONCOUNT_H

#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

namespace cot {

// A Function pass analyze one function at time. Selecting the right pass type
// allows LLVM to schedule them aggressively, improving compiler data-locality.
class InstructionCount : public llvm::FunctionPass {
public:
  static char ID;

public:
  InstructionCount() : llvm::FunctionPass(ID),
                       ICount(0) { }

public:
  // This member function will be invoked on every function found on the module
  // currently considered by the compiler.
  virtual bool runOnFunction(llvm::Function &Fun);

  // Allows to require analysis and declare which analysis are invalidated by
  // this pass.
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;

  // Analysis passes should implement this member function to print a human
  // readable version of analysis info. Its invocation is triggered by the
  // '-analyze' 'opt' command line switch.
  virtual void print(llvm::raw_ostream &OS, const llvm::Module *Mod) const;

  virtual const char *getPassName() const {
    return "Instruction counter";
  }

public:
  unsigned GetInstructionCount() const { return ICount; }

private:
  // This is the information computed by the analysis.
  unsigned ICount;
};

} // End cot namespace.

#endif // COT_INSTRUCTIONCOUNT_H
