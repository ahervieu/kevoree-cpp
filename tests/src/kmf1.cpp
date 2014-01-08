#include <microframework/api/trace/DefaultTraceConverter.h>
#include <microframework/api/json/JSONModelLoader.h>
#include <microframework/api/compare/ModelCompare.h>
#include <kevoree-core/model/kevoree/DefaultkevoreeFactory.h>
#include <kevoree-core/model/kevoree/ContainerRoot.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/time.h>

int main(int argc,char **argv)
{

	DefaultkevoreeFactory factory;

	JSONModelLoader loader;

	loader.setFactory(&factory);
	ifstream myfile;
	myfile.open ("model1.json");
	if(!myfile){
		cout << "no file trace" << endl;
	}

	ifstream myfile2;
	myfile2.open ("model2.json");
	if(!myfile2){
		cout << "no file trace" << endl;
	}

	clock_t start = clock();
	ContainerRoot *model = (ContainerRoot*)loader.loadModelFromStream(myfile)->front();
	ContainerRoot *model2 = (ContainerRoot*)loader.loadModelFromStream(myfile2)->front();
	clock_t finish = clock();

	std::cout << "time delta (ms) = " << Utils::mstimer(start,finish) << std::endl;

		start = clock();
		ModelCompare *kompare = new ModelCompare();
		TraceSequence *seq = kompare->diff(model,model2);
		finish = clock();
		std::cout << "time delta (ms) = " << Utils::mstimer(start,finish) << std::endl;
		cout << seq->exportToString() << endl;
return 0;
}




