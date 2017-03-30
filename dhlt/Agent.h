#ifndef AGENT_H
#define AGENT_H

#include "AliHLTModuleAgent.h"

/**
 * This module agent handles dimuon HLT module registration 
 */
class Agent : public AliHLTModuleAgent
{
public:
	Agent();
	
	virtual ~Agent() = default;

	Agent(const Agent&) = delete;
	Agent& operator=(const Agent&) = delete;

    virtual int RegisterComponents(AliHLTComponentHandler* pHandler) const;

    ClassDef(Agent,0)
	
};

#endif 
