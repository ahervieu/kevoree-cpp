
#ifndef __KevoreeModelHandlerService_H
#define __KevoreeModelHandlerService_H

#include <list>
#include <string>
#include <ContainerRoot.h>
#include <ctime>

class KevoreeModelHandlerService {
	
	public:
    virtual ContainerRoot *getLastModel();

     //UUIDModel getLastUUIDModel();

     time_t getLastModification();

     virtual void updateModel(ContainerRoot *model);

    // void updateModel(ContainerRoot *model, ModelUpdateCallback callback);

     //Date atomicUpdateModel(ContainerRoot model);

   //  void compareAndSwapModel(UUIDModel previousModel, ContainerRoot targetModel);

    // void compareAndSwapModel(UUIDModel previousModel, ContainerRoot targetModel, ModelUpdateCallback callback);

   //  Date atomicCompareAndSwapModel(UUIDModel previousModel, ContainerRoot targetModel) throws KevoreeModelUpdateException;

    /**
     *
     * @return a list of the previous models that have been applied on the runtime
     */
     virtual std::list<ContainerRoot> getPreviousModels();

     virtual std::string getNodeName();

     //void registerModelListener(ModelListener listener);

    // void unregisterModelListener(ModelListener listener);

    // void acquireLock(ModelHandlerLockCallBack callBack, Long timeout);

     //void releaseLock(UUID uuid);

     virtual bool checkModel(ContainerRoot *targetModel);
	
};
#endif
