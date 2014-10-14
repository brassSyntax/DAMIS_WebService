///////////////////////////////////////////////////////////
//  ClassificationGrouping.cpp
//  Implementation of the Class ClassificationGrouping
//  Created on:      05-Vas-2014 17:36:50
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "logging/easylogging++.h"
#include "ClassificationGrouping.h"
#include "CallCalculus.h"


ClassificationGrouping::ClassificationGrouping(int p, int maxCalcTime, InitDamisService* initFile) : CallCalculus(p, maxCalcTime, initFile)
{
    LOG (INFO) << "Classification/Grouping has been called";
    this->exeParams = "";
}


ClassificationGrouping::~ClassificationGrouping(){

}
	/**
	*Method runs SOM
	*/
	void ClassificationGrouping::runSOM(int rows, int columns, int eHat)
	{
        this->exeParams = " -al SOM -rows " + std::to_string(static_cast<long long>(rows)) + " -columns " + std::to_string(static_cast<long long>(columns)) + " -eHat " + std::to_string(static_cast<long long>(eHat));
        this->run();
	}
	/**
	*Method runs KMEANS
	*/
	void ClassificationGrouping::runKMEANS(int maxIter, int noOfClusters)
	{
	    this->exeParams = " -al KMEANS -maxIter " + std::to_string(static_cast<long long>(maxIter)) + " -noOfClust " + std::to_string(static_cast<long long>(noOfClusters));
	    this->run();
	}
    /**
	*Method runs MLP
	*/
    void ClassificationGrouping::runMLP(int h1, int h2, double qty, int maxIter, bool valMethod)
    {
        char bufferqty[32];
        snprintf(bufferqty, sizeof(bufferqty), "%g", qty);

        this->exeParams = " -al MLP -h1pNo " + std::to_string(static_cast<long long>(h1)) + " -h2pNo " + std::to_string(static_cast<long long>(h2)) + " -qty " +  bufferqty + " -maxIter " +  std::to_string(static_cast<long long>(maxIter)) + " -kFoldVal " + std::to_string(static_cast<long long>(valMethod));
        this->run();
    }

    void ClassificationGrouping::runDecForest(double r, double dL, double dT, int nTree)
    {
        char bufferR[32];
        snprintf(bufferR, sizeof(bufferR), "%g", r);

        char bufferdL[32];
        snprintf(bufferdL, sizeof(bufferdL), "%g", dL);

        char bufferdT[32];
        snprintf(bufferdT, sizeof(bufferdT), "%g", dT);

            std::string a =  " -al DECTREE -dL ";

        this->exeParams = a + bufferdL + " -dT " + bufferdT + " -r " + bufferR + " -nTree " + std::to_string(static_cast<long long>(nTree));
        this->run();
    }
