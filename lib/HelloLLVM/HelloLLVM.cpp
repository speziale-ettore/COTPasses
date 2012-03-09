
#include "cot/AllPasses.h"

#include "llvm/Support/ErrorHandling.h"

using namespace cot;

namespace {

class HelloLLVM : public llvm::ModulePass {
public:
  static char ID;

public:
  HelloLLVM() : llvm::ModulePass(ID) { }

public:
  // This member function must implement the code of your pass.
  virtual bool runOnModule(llvm::Module &Mod);

  // The getAnalysisUsage allows to tell LLVM pass manager which analysis are
  // used by the pass. It is also used to declare which analysis are preserved
  // by the pass.
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;

  virtual const char *getPassName() const {
    return "Hello llvm";
  }
};

char HelloLLVM::ID = 0;

} // End anonymous namespace.

bool HelloLLVM::runOnModule(llvm::Module &Mod) {
  llvm_unreachable("Not yet implemented");
}

void HelloLLVM::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  llvm_unreachable("Not yet implemented");
}

llvm::Pass *cot::CreateHelloLLVMPass() { return new HelloLLVM(); }

using namespace llvm;

// The INITIALIZE_PASS_{BEGIN,END} macros generates some functions that can be
// used to register the pass to the LLVM pass registry.
// Parameters:
//
// HelloLLVM: pass class name
// "hello-llvm": command line switch to enable pass
// "Build an hello world": pass description
// false: the pass doesn't look only at the CFG
// false: the pass isn't an analysis.
INITIALIZE_PASS_BEGIN(HelloLLVM,
                      "hello-llvm",
                      "Build an hello world",
                      false,
                      false)
INITIALIZE_PASS_END(HelloLLVM,
                    "hello-llvm",
                    "Build an hello world",
                    false,
                    false)
