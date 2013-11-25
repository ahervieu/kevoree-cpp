#include "CPPNodeType.h"
#include "deploy/PrimitiveCommandExecutionHelper.h"

extern "C"{  
    AbstractNodeType* create(){
    return new CPPNodeType();
   }                        
}
extern "C" void destroy_object( AbstractNodeType* object )
{
  delete object;
}


CPPNodeType::CPPNodeType()
{

}

CPPNodeType::~CPPNodeType()
{

}



void CPPNodeType::startNode()
{

}
void CPPNodeType::stopNode(){
	
}

AdaptationModel *CPPNodeType::plan(ContainerRoot *currentModel,ContainerRoot *targetModel,TraceSequence *traces)
{
	mapper.internal_update(currentModel,targetModel);
	AdaptationModel *adaptationmodel = planner.compareModels(currentModel,targetModel,node->name,traces);
	return planner.schedule(adaptationmodel,getNodeName());
}

bool CPPNodeType::execute(ContainerNode *rootNode,AdaptationModel *adaptionModel,AbstractNodeType *nodeInstance)
{

	PrimitiveCommandExecutionHelper::execute(rootNode,adaptionModel,nodeInstance);
}

PrimitiveCommand *CPPNodeType::getPrimitive(AdaptationPrimitive *primitive)
{
	return mapper.buildPrimitiveCommand(primitive,getNodeName());	
}
