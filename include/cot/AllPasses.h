
#ifndef COT_ALLPASSES_H
#define COT_ALLPASSES_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Pass.h"

// LLVM passes are classes, but usually they are created using a factory
// function. This allows to hide optimization pass implementation. With analysis
// passes, you have to follow the same idiom. The difference is that instead of
// returning a llvm::Pass * you have to return a pointer to the analysis class.
namespace cot {

class InstructionCount;

// Analysis.
InstructionCount *CreateInstructionCountPass();

// Transformations.
llvm::Pass *CreateHelloLLVMPass();
llvm::Pass *CreateFunctionEraserPass();

} // End namespace cot.

namespace llvm {

class PassRegistry;

// Analysis.
void initializeInstructionCountPass(PassRegistry &Registry);

// Transformations.
void initializeHelloLLVMPass(PassRegistry &Registry);
void initializeFunctionEraserPass(PassRegistry &Registry);

} // End namespace llvm.

#endif // COT_ALLPASSES_H
