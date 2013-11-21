#ifndef __KevoreeBootStrap_H
#define __KevoreeBootStrap_H
#include <kevoree-core/core/KevoreeCoreBean.h>
#include <kevoree-core/core/api/Bootstraper.h>
#include <kevoree-core/model/kevoree/DefaultkevoreeFactory.h>

class KevoreeBootStrap {
public:
	KevoreeBootStrap();
    KevoreeCoreBean* getCore();
    void setNodeName(std::string nodeName);
    void setBootstrapModel(ContainerRoot *bmodel);
    void start(); 
	void stop();
	
private:
  std::string nodeName;
  ContainerRoot *bootstrapModel;
  KevoreeCoreBean *coreBean;
  bool started; //false
  DefaultkevoreeFactory factory;
};


#endif
