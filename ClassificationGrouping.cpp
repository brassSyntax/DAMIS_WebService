///////////////////////////////////////////////////////////
//  ClassificationGrouping.cpp
//  Implementation of the Class ClassificationGrouping
//  Created on:      05-Vas-2014 17:36:50
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "logging\easylogging++.h"
#include "ClassificationGrouping.h"
#include "CallCalculus.h"


ClassificationGrouping::ClassificationGrouping(int p, int maxCalcTime, InitDamisServiceFile* initFile) : CallCalculus(p, maxCalcTime, initFile)
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
        this->exeParams = " -al SOM -rows " + std::to_string(rows) + " -columns " + std::to_string(columns) + " -eHat " + std::to_string(eHat);
        this->run();
	}
