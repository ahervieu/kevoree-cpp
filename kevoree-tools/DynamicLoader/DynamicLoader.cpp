#include <kevoree-tools/DynamicLoader/DynamicLoader.h>
#include <map>

DynamicLoader::DynamicLoader(Bootstraper *bootstrap)
{
	this->bootstrap = bootstrap;	
}

void DynamicLoader::setModelService(KevoreeModelHandlerService *mservice){
	this->mservice = mservice;
}


DeployUnit * DynamicLoader::select_du_architecture(TypeDefinition *type) {
	DeployUnit *du=NULL;

	// TODO need to add in the metamodel
	for ( std::map<string,DeployUnit*>::const_iterator it = (type->deployUnit).begin();  it != (type->deployUnit).end(); ++it) {
		return it->second;
	}
	return du;
}


bool DynamicLoader::register_instance(Instance *i)
{

	try
	{
		TypeDefinition *type = i->typeDefinition;
		DeployUnit *du=NULL;

		if(type == NULL)
		{
			LOGGER_WRITE(Logger::ERROR,"There is no TypeDefinition defined");
			return false;
		}

		if(type->deployUnit.size() == 0)
		{
			LOGGER_WRITE(Logger::ERROR,"There is no DeployUnit defined");
			return false;
		}

		du =select_du_architecture(type);

		// check if exist
		map<string, void*>::const_iterator it = deploysUnits.find(du->internalGetKey());
		if (it != deploysUnits.end())
		{
			LOGGER_WRITE(Logger::INFO,"The DeployUnit is already loaded");	
			return true;
		}

		string libpath=	bootstrap->resolveDeployUnit(du);
		if(!libpath.empty())
		{
			LOGGER_WRITE(Logger::DEBUG,"install_deploy_unit "+libpath);
			void *handler = soloader_load(libpath);
			if(handler == NULL)
			{
				LOGGER_WRITE(Logger::ERROR,"failed install_deploy_unit "+libpath);
				return false;
			}
			LOGGER_WRITE(Logger::DEBUG,"done "+libpath);
			deploysUnits[du->internalGetKey()] = handler;
			return true;	
		}else
		{
			LOGGER_WRITE(Logger::ERROR,"The DeployUnit cannot be registred beacause the Typedefintion is not found "+libpath);	
			return false;	
		}		
	}
	catch ( const std::exception & e )
	{
		std::cerr << e.what() << endl;
		LOGGER_WRITE(Logger::ERROR,"");
		return false;
	}

}

AbstractTypeDefinition* DynamicLoader::create_instance(Instance *i)
{
	try
	{
		TypeDefinition *type = i->typeDefinition;
		if(type == NULL)
		{
			LOGGER_WRITE(Logger::ERROR,"There is no TypeDefinition define");
			return NULL;
		}

		DeployUnit *du  =select_du_architecture(type);

		// todo check if for me
		map<string, void*>::const_iterator it = deploysUnits.find(du->internalGetKey());
		if (it == deploysUnits.end())
		{
			LOGGER_WRITE(Logger::ERROR,"The DU is no registred");
			return NULL;
		}
		LOGGER_WRITE(Logger::DEBUG,"newInstance of "+du->name +" "+i->path());
		void *instance = newInstance(it->second);
		if(instance == NULL)
		{
			LOGGER_WRITE(Logger::ERROR,"Error during create Instance of "+du->name);
			return NULL;
		}
		instances[i->path()] = instance;
		return(AbstractTypeDefinition*)instance;
	}
	catch ( const std::exception & e )
	{
		std::cerr << e.what() << endl;
		return NULL;
	}

}


bool DynamicLoader::start_instance(Instance *i)
{

	AbstractTypeDefinition *inst = (AbstractTypeDefinition*)instances.find(i->path())->second;
	if(inst != NULL && dynamic_cast<AbstractTypeDefinition*>(inst) != 0)
	{
		LOGGER_WRITE(Logger::DEBUG,"invoke start "+i->name);
		try
		{
			update_param(i,inst);
			inst->start();
		}
		catch ( const std::exception & e )
		{
			std::cerr << e.what() << endl;
			return false;
		}

		return true;
	}
	return false;

}


bool DynamicLoader::update_param(Instance *i,AbstractTypeDefinition *inst)
{

	TypeDefinition *type = 	i->typeDefinition;
	if(type != NULL && dynamic_cast<TypeDefinition*>(type) != 0)
	{
		DictionaryType *dtype = type->dictionaryType;
		if(dtype != NULL && dynamic_cast<DictionaryType*>(dtype) != 0)
		{
			Dictionary * dico =i->dictionary;
			for (std::map<string,DictionaryAttribute*>::const_iterator it = dtype->attributes.begin();  it != dtype->attributes.end(); ++it)
			{
				DictionaryAttribute *da = it->second;

				//fragmentDictionary
				if(dico != NULL && dico->values[da->name] != NULL)
				{
					inst->params[da->name] = dico->values[da->name]->value;
				}
				else if(i->fragmentDictionary[mservice->getNodeName()] != NULL && i->fragmentDictionary[mservice->getNodeName()]->values[da->name] != NULL )
				{

					LOGGER_WRITE(Logger::DEBUG,"fragmentDictionary "+ i->fragmentDictionary[mservice->getNodeName()]->values[da->name]->value);
					inst->params[da->name] = i->fragmentDictionary[mservice->getNodeName()]->values[da->name]->value;
				}
				else
				{
					cout << da->name << " defaultValue " << da->defaultValue << endl;
					inst->params[da->name] = 	da->defaultValue;
				}

			}
		}
	}
}


bool DynamicLoader::update_instance(Instance *i){

	AbstractTypeDefinition *inst = (AbstractTypeDefinition*)instances.find(i->path())->second;
	if(inst != NULL && dynamic_cast<AbstractTypeDefinition*>(inst) != 0)
	{
		LOGGER_WRITE(Logger::DEBUG,"invoke update "+i->name);
		try
		{
			update_param(i,inst);
			inst->update();
		}
		catch ( const std::exception & e )
		{
			std::cerr << e.what() << endl;
			return false;
		}
		return true;
	}
	return false;

}



bool DynamicLoader::stop_instance(Instance *i)
{
	AbstractTypeDefinition *inst = (AbstractTypeDefinition*)instances.find(i->path())->second;
	if(inst != NULL && dynamic_cast<AbstractTypeDefinition*>(inst) != 0)
	{
		LOGGER_WRITE(Logger::DEBUG,"invoke stop "+i->name);
		inst->stop();
		LOGGER_WRITE(Logger::DEBUG,"Clean instance cache");
		instances.erase(instances.find(i->path()));
		LOGGER_WRITE(Logger::DEBUG,"Cleaned ");
		return true;
	}
	return false;
}


bool DynamicLoader::destroy_instance(Instance *i)
{
	TypeDefinition *type = i->typeDefinition;
	if(type == NULL)
	{
		LOGGER_WRITE(Logger::ERROR,"There is no TypeDefinition define");
		return false;
	}


	if(instances.find(i->path()) != instances.end())
	{

		AbstractTypeDefinition *inst = (AbstractTypeDefinition*)instances.find(i->path())->second;

		if(inst != NULL && dynamic_cast<AbstractTypeDefinition*>(inst) != 0)
		{

			LOGGER_WRITE(Logger::DEBUG,"Clean deployUnits");
			DeployUnit *du  = select_du_architecture(type);

			// todo check if for me
			map<string, void*>::const_iterator it = deploysUnits.find(du->internalGetKey());
			if (it == deploysUnits.end())
			{
				LOGGER_WRITE(Logger::ERROR,"destroy instance but can't find deploy unit of STOP done");
				return false;
			}

			destroyInstance(it->second,inst);

			//FIX ME
			//deploysUnits.erase(deploysUnits.find(du->internalGetKey()));
			// class 
		//	if(dlclose(handler) != 0){
					LOGGER_WRITE(Logger::WARNING,"dlclose");
		//	}



		}else 
		{
			LOGGER_WRITE(Logger::ERROR,"instance cannt be cast");
			return false;	
		}


	}

	return true;
}

void * DynamicLoader::soloader_load(std::string libpath)
{
	void* handle = dlopen(libpath.c_str(),RTLD_LAZY|RTLD_GLOBAL);
	if (!handle)
	{

		LOGGER_WRITE(Logger::ERROR,"dlopen =>"+string(dlerror()));
		return NULL;
	}
	return handle;
}

void DynamicLoader::destroyInstance(void *handler,AbstractTypeDefinition *instance)
{
	LOGGER_WRITE(Logger::DEBUG,"DynamicLoader destroyInstance");
	// destructor
	void (*destroy)(AbstractTypeDefinition*);
	destroy = (void (*)(AbstractTypeDefinition*))dlsym(handler, "destroy_object");
	destroy(instance);
}

AbstractTypeDefinition * DynamicLoader::newInstance(void *handle)
{
	LOGGER_WRITE(Logger::DEBUG,"DynamicLoader Instance");
	AbstractTypeDefinition* (*create)();
	create =  (AbstractTypeDefinition* (*)())dlsym(handle, "create");
	if(!create){
		LOGGER_WRITE(Logger::ERROR,"cannot find symbol newInstance");
		return NULL;
	}
	AbstractTypeDefinition* c = (AbstractTypeDefinition*)create();
	return c;
}


