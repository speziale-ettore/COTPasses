
#include "cot/AllPasses.h"
#include "cot/InstructionCount.h"

// This macro is need to associate a label to statistics.
#define DEBUG_TYPE "function-eraser"

#include "llvm/ADT/Statistic.h"
#include "llvm/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

using namespace cot;

namespace {

// LLVM command line library allows straightforward declaration of command line
// options. Here we are declaring 'EraseThreshold' as a command line argument of
// type unsigned. It can be set using the '-erase-threshold' command line
// option and its default value is the greatest 'unsigned'.
llvm::cl::opt<unsigned> EraseThreshold(
                          "erase-threshold",
                          llvm::cl::init(std::numeric_limits<unsigned>::max()),
                          llvm::cl::Hidden,
                          llvm::cl::desc("Erasing threshold in number "
                                         "of instructions"));

// Statistics are global counters you can use to collect information about the
// behavior of your pass during compilation.
STATISTIC(ErasedFunctions, "Number of functions erased");

class FunctionEraser : public llvm::FunctionPass {
public:
  static char ID;

public:
  FunctionEraser() : llvm::FunctionPass(ID) { }

public:
  virtual bool runOnFunction(llvm::Function &Fun);

  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;

  virtual const char *getPassName() const {
    return "Function eraser";
  }
};

char FunctionEraser::ID = 0;

} // End anonymous namespace.

bool FunctionEraser::runOnFunction(llvm::Function &Fun) {
  // Get the analysis we need. Please notice that we are not giving any argument
  // to 'getAnalysis'. This is the correct way for requesting analysis from
  // 'llvm::FunctionPass'. If you need to get analysis information from a
  // 'llvm::ModulePass', then you have to give the Function you want to analyze
  // as 'getAnalysis' argument.
  InstructionCount &ICount = getAnalysis<InstructionCount>();

  if(ICount.GetInstructionCount() < EraseThreshold)
    return false;

  // Apply the transformation by erasing function body.
  Fun.deleteBody();

  // Update pass statistics.
  ++ErasedFunctions;

  return true;
}

void FunctionEraser::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  // We need to known the number of instructions in a function in order to
  // decide whether it has to be deleted. We have an analysis doing that --
  // request LLVM pass manager to run it.
  AU.addRequired<InstructionCount>();
}

llvm::Pass *cot::CreateFunctionEraserPass() { return new FunctionEraser(); }

using namespace llvm;

INITIALIZE_PASS_BEGIN(FunctionEraser,
                      "function-eraser",
                      "Delete functions",
                      false,
                      false)
INITIALIZE_PASS_END(FunctionEraser,
                    "function-eraser",
                    "Delete functions",
                    false,
                    false)
