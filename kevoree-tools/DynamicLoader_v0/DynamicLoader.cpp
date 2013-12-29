#include <kevoree-tools/DynamicLoader/DynamicLoader.h>


DynamicLoader::DynamicLoader(Bootstraper *bootstrap)
{
	this->bootstrap = bootstrap;	
}

bool DynamicLoader::register_instance(Instance *i)
{
	TypeDefinition *type = i->typeDefinition;
	if(type == NULL)
	{
		LOGGER_WRITE(Logger::ERROR,"There is no TypeDefinition define");
		return false;
	}
	if(type->deployUnits.size() == 0)
	{
		LOGGER_WRITE(Logger::WARNING,"There is no DeployUnit to register");
		return false;	
	}
	for (std::map<string,DeployUnit*>::iterator iterator = type->deployUnits.begin(), end = type->deployUnits.end(); iterator != end; ++iterator)
	{
			 DeployUnit *du  = iterator->second;
			// todo check if for me
			map<string, void*>::const_iterator it = deploysUnits.find(du->internalGetKey());
			if (it == deploysUnits.end())
			{
				string libpath=	bootstrap->resolveDeployUnit(du);
				if(!libpath.empty())
				{
					LOGGER_WRITE(Logger::DEBUG,"install_deploy_unit "+libpath);
					void *handler = soloader_load(libpath);
					if(handler == NULL)
					{
						return false;
					}
					deploysUnits[du->internalGetKey()] = handler;		
					return true;	
				}else
				{
					LOGGER_WRITE(Logger::ERROR,"The DeployUnit cannot be registred beacause the Typedefintion is not found "+libpath);	
					return false;	
				}	
			}
	}
}

AbstractTypeDefinition* DynamicLoader::create_instance(Instance *i)
{
	TypeDefinition *type = i->typeDefinition;
	if(type == NULL)
	{
		LOGGER_WRITE(Logger::ERROR,"There is no TypeDefinition define");
		return NULL;
	}
	if(type->deployUnits.size() == 0)
	{
		LOGGER_WRITE(Logger::WARNING,"There is no DeployUnit to register");
		return NULL;	
	}
	
	for (std::map<string,DeployUnit*>::iterator iterator = type->deployUnits.begin(), end = type->deployUnits.end(); iterator != end; ++iterator)
	{
		DeployUnit *du  = iterator->second;
		// todo check if for me
		map<string, void*>::const_iterator it = deploysUnits.find(du->internalGetKey());
		if (it == deploysUnits.end())
		{
			return NULL;
		}
		LOGGER_WRITE(Logger::DEBUG,"newInstance of "+du->name);
		void *instance = newInstance(it->second);
		if(instance == NULL){
			return NULL;
		}
		instances[i->path()] = instance;
		return(AbstractTypeDefinition*)instance;
	}	
	
	return NULL;
}

bool DynamicLoader::unload_instance(Instance *i)
{
	TypeDefinition *type = i->typeDefinition;
	if(type == NULL)
	{
		LOGGER_WRITE(Logger::ERROR,"There is no TypeDefinition define");
		return false;
	}
	if(type->deployUnits.size() == 0)
	{
		LOGGER_WRITE(Logger::WARNING,"There is no DeployUnit to register");
		return false;	
	}
	
	
	if(instances.find(i->path()) != instances.end())
	{
		
		AbstractTypeDefinition *inst = (AbstractTypeDefinition*)instances.find(i->path())->second;
		if(inst != NULL)
		{
			LOGGER_WRITE(Logger::DEBUG,"stop of "+i->name);
			inst->stop();
			for (std::map<string,DeployUnit*>::iterator iterator = type->deployUnits.begin(), end = type->deployUnits.end(); iterator != end; ++iterator)
			{
					DeployUnit *du  = iterator->second;
					// todo check if for me
					map<string, void*>::const_iterator it = deploysUnits.find(du->internalGetKey());
					if (it == deploysUnits.end())
					{
						return NULL;
					}
					destroyInstance(it->second,inst);
			}
							
		}
		
		
	}

	return true;
}

void * DynamicLoader::soloader_load(std::string libpath)
{
		    void* handle = dlopen(libpath.c_str(),RTLD_NOW | RTLD_LAZY);
			if (!handle) 
		    {

					LOGGER_WRITE(Logger::ERROR,"dlopen =>"+string(dlerror()));
					return NULL;
		    }
		  return handle;      
}

void DynamicLoader::destroyInstance(void *handler,AbstractTypeDefinition *instance)
{
			// destructor 
			void (*destroy)(AbstractTypeDefinition*);
			destroy = (void (*)(AbstractTypeDefinition*))dlsym(handler, "destroy_object");
			destroy(instance);
			// class 
			if(dlclose(handler) != 0){
					LOGGER_WRITE(Logger::WARNING,"dlclose");
			}
}

AbstractTypeDefinition * DynamicLoader::newInstance(void *handle)
{
			AbstractTypeDefinition* (*create)();
			create =  (AbstractTypeDefinition* (*)())dlsym(handle, "create");
			AbstractTypeDefinition* c = (AbstractTypeDefinition*)create();
			return c;	
}


