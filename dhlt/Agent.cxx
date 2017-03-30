#include "Agent.h"
#include "AliHLTMUONDigitReaderComponent.h"

ClassImp(Agent);

namespace {
    Agent a; // create an object as soon as the library is loaded...
}

Agent::Agent() : AliHLTModuleAgent("dhlt") 
{
}

int Agent::RegisterComponents(AliHLTComponentHandler* pHandler) const
{
  ///
  /// Registers all available components of this module.
  /// @param [in] pHandler  instance of the component handler.
  ///

  if (pHandler == nullptr) {
    HLTError("Got a null handler !");
      return -EINVAL;
  }

  pHandler->AddComponent(new AliHLTMUONDigitReaderComponent);

  return 0;
}

