
#include "cot/AllPasses.h"
#include "cot/InstructionCount.h"

#include "llvm/Function.h"
#include "llvm/Support/ErrorHandling.h"

using namespace cot;

char InstructionCount::ID = 0;

bool InstructionCount::runOnFunction(llvm::Function &Fun) {
  ICount = 0;

  // A llvm::Function is just a list of llvm::BasicBlock. In order to get
  // instruction count we can visit all llvm::BasicBlocks ...
  for(llvm::Function::const_iterator I = Fun.begin(),
                                     E = Fun.end();
                                     I != E;
                                     ++I)
    // ... and sum the llvm::BasicBlock size -- A llvm::BasicBlock size is just
    // a list of instructions!
    ICount += I->size();

  return false;
}

void InstructionCount::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  // This is an analysis, nothing is modified, so other analysis are preserved.
  AU.setPreservesAll();
}

void InstructionCount::print(llvm::raw_ostream &OS,
                             const llvm::Module *Mod) const {
  if(!Mod)
    return;

  OS << "  Instruction count: " << ICount << "\n";
}

using namespace llvm;

INITIALIZE_PASS_BEGIN(InstructionCount,
                      "instruction-count",
                      "Count instructions",
                      true,
                      true)
INITIALIZE_PASS_END(InstructionCount,
                    "instruction-count",
                    "Count instructions",
                    true,
                    true)
