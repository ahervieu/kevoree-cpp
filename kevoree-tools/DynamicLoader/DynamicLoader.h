#ifndef __LoaderTypeDefinition_H
#define __LoaderTypeDefinition_H

#include <kevoree-core/api/KevoreeModelHandlerService.h>
#include <kevoree-core/api/AbstractTypeDefinition.h>
#include <kevoree-core/api/AbstractNodeType.h>
#include <kevoree-core/api/AbstractComponent.h>
#include <kevoree-core/api/Bootstraper.h>
#include <kevoree-core/api/IDynamicLoader.h>

#include <kevoree-core/model/kevoree/TypeDefinition.h>
#include <kevoree-core/model/kevoree/Dictionary.h>
#include <kevoree-core/model/kevoree/DictionaryValue.h>
#include <kevoree-core/model/kevoree/DeployUnit.h>
#include <kevoree-core/model/kevoree/FragmentDictionary.h>
#include <kevoree-core/model/kevoree/DictionaryType.h>
#include <kevoree-core/model/kevoree/DictionaryAttribute.h>

#include <map>
#include <string>
#include <dlfcn.h>

class DynamicLoader : public IDynamicLoader
{
public:
	DynamicLoader(Bootstraper *bootstrap);
	bool register_instance(Instance *i);
	AbstractTypeDefinition * create_instance(Instance *i);
	bool destroy_instance(Instance *i);

	// call the start method in the instance
	bool start_instance(Instance *i);
	bool stop_instance(Instance *i);
	bool update_instance(Instance *i);

	// select the good deploy unit depending on architecture os ; ....
	DeployUnit * select_du_architecture(TypeDefinition *type);

	void setModelService(KevoreeModelHandlerService *mservice);

private:
	bool update_param(Instance *i,AbstractTypeDefinition *inst);
	void * soloader_load(std::string type);
	AbstractTypeDefinition * newInstance(void *handle);
	void destroyInstance(void *handler,AbstractTypeDefinition *instance);
	map<string, void*>	deploysUnits;
	map<string, void*>	instances;
	Bootstraper *bootstrap;
	KevoreeModelHandlerService *mservice;
};


#endif






