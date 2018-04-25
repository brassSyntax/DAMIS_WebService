///////////////////////////////////////////////////////////
//  Statistics.cpp
//  Implementation of the Class Statistics
//  Created on:      05-Vas-2014 17:36:46
//  Original author: Povilas
///////////////////////////////////////////////////////////


#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "Statistics.h"
#include "HelperMethods.h"
#include "logging/easylogging++.h"


Statistics::Statistics(InitDamisService* initFile):ServeRequest(initFile){

    LOG (INFO) << "Statistical data processing has been called";
}

void Statistics::statPrimitives()
{

    LOG (INFO) << "Initiating statistical primiteve calculation";

    for (int i = 0; i < serveFile->getNumberOfAttributes(); i++)
    {
        for (int j = 0; j < serveFile->getNumberOfObjects(); j++)
        {
            ServeRequest::tmpDataVector.push_back(serveFile->getDoubleDataAt(j, i));
        }
            double min = *std::min_element(ServeRequest::tmpDataVector.begin(), ServeRequest::tmpDataVector.end());
            double max = *std::max_element(ServeRequest::tmpDataVector.begin(), ServeRequest::tmpDataVector.end());
            double mean = HelperMethods::getMean(ServeRequest::tmpDataVector);
            double stdev = HelperMethods::getStd(ServeRequest::tmpDataVector); //daliname is N o ne is N-1

            size_t n = ServeRequest::tmpDataVector.size() >> 1;
            nth_element(ServeRequest::tmpDataVector.begin(), ServeRequest::tmpDataVector.begin()+ n, ServeRequest::tmpDataVector.end());

            double median = ServeRequest::tmpDataVector.at(n);
            ServeRequest::tmpDataVector.clear();

            ServeRequest::writeData.push_back(std::vector<double>{min, max, mean, stdev, median});
    }
    {
        std::vector<std::string> attrNames={"min NUMERIC", "max NUMERIC",
                "mean NUMERIC", "std NUMERIC", "median NUMERIC"};

        std::vector<std::string> dummy;
        dummy.reserve(0); //pass dummy vector to write function since no attribute after transform are left
        this->writeDataToFile(outFile->getFilePath(), prepareDataSection(ServeRequest::writeData, dummy), prepareAttributeSection(attrNames, dummy, dummy));

    }
}


Statistics::~Statistics(){

}
