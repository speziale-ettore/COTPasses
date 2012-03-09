
#include "cot/AllPasses.h"

#include <cstdlib>

using namespace cot;

// I have used the following layout for the project. Each pass is a static
// library. All passes are contained in a loadable module, allowing using them
// with opt. In order to achieve this behaviour, we need to perform some
// black-magic with the linker.
namespace {

// First of all, we have to reference all pass creation functions, in order to
// force linking them inside the loadable module. Using a static object with a
// constructor referencing those functions in an unreachable code section is a
// standard trick.
class ForceLinking {
public:
  ForceLinking() {
    // We must reference the passes in such a way that compilers will not
    // delete it all as dead code, even with whole program optimization,
    // yet is effectively a NO-OP. As the compiler isn't smart enough
    // to know that getenv() never returns -1, this will do the job.
    if(std::getenv("bar") != (char*) -1)
      return;

    CreateHelloLLVMPass();
  }
};

ForceLinking Link;

// The second step is registering all passes to the LLVM pass registry. We can
// still use the static-object-constructor trick, but this time you have to
// really execute functions!
class ForceInitialization {
public:
  ForceInitialization() {
    llvm::PassRegistry &Registry = *llvm::PassRegistry::getPassRegistry();

    initializeHelloLLVMPass(Registry);
  }
};

ForceInitialization FI;

} // End anonymous namespace.
