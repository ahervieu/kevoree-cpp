#ifndef __CPPNODETYPE_H_
#define __CPPNODETYPE_H_

#include <microframework/api/trace/TraceSequence.h>
#include <kevoree-core/core/api/AbstractNodeType.h>
#include <kevoree-core/model/kevoreeadaptation/DefaultkevoreeadaptationFactory.h>
#include <kevoree-core/model/kevoreeadaptation/AdaptationPrimitive.h>
#include "Planner.h"
#include "CommandMapper.h"


class CPPNodeType : public AbstractNodeType
{
public:
	CPPNodeType();
	virtual ~CPPNodeType();
    void startNode();
    void stopNode();
    AdaptationModel *plan(ContainerRoot *actualModel,ContainerRoot *targetModel,TraceSequence *traces);
    bool execute(ContainerNode *rootNode,AdaptationModel *adaptionModel,AbstractNodeType *nodeInstance);
    PrimitiveCommand* getPrimitive(AdaptationPrimitive *primitive);
    
Planner planner;
CommandMapper mapper;
};

#endif /*CPPNODETYPE_H_*/