#ifndef __NodeTypeBootstrapHelper_H
#define __NodeTypeBootstrapHelper_H


#include <kevoree-core/api/AbstractNodeType.h>
#include <kevoree-core/api/Bootstraper.h>

#include <kevoree-core/model/kevoree/Repository.h>


#include <kevoree-tools/BootstrapHelper/NodeTypeBootstrapHelper.h>
#include <kevoree-tools/DynamicLoader/DynamicLoader.h>
#include <maven-resolver/api/MavenResolver.h>

#include <network/http/impl/BoostHTTPClient.h>
#include <network/http/api/HTTPRequest.h>

class NodeTypeBootstrapHelper : public Bootstraper 
{
public:
	 NodeTypeBootstrapHelper(std::string mvn_maven_base_path);
	~NodeTypeBootstrapHelper();
	AbstractNodeType *bootstrapNodeType(std::string destNodeName, KevoreeModelHandlerService *mservice);
	IDynamicLoader* getDynamicLoader();
	std::string resolveDeployUnit(DeployUnit *du);
	void setproposedNewModel(ContainerRoot *root);
private:
DynamicLoader *dynamicLoader;
maven::resolver::MavenResolver resolver;
KevoreeModelHandlerService *mservice;
ContainerRoot *proposedNewModel;
};
#endif

 
