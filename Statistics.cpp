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
#include "logging\easylogging++.h"


Statistics::Statistics(InitDamisServiceFile* initFile):DamisService(initFile){

    LOG (INFO) << "Statistical data processing has been called";
}

void Statistics::statPrimitives()
{

    LOG (INFO) << "Initiating statistical primiteve calculation";

    std::vector<std::string> attrNames;
    std::string tmp;

        tmp = "min";tmp.append(" NUMBER"); attrNames.push_back(tmp);
        tmp = "max"; tmp.append(" NUMBER"); attrNames.push_back(tmp);
        tmp = "mean"; tmp.append(" NUMBER"); attrNames.push_back(tmp);
        tmp = "std"; tmp.append(" NUMBER"); attrNames.push_back(tmp);
        tmp = "median"; tmp.append(" NUMBER"); attrNames.push_back(tmp);


    std::vector<std::string> dummy;
    dummy.reserve(0); //pass dummy vector to write function since no attributre after transform are left


    for (int i = 0; i < DamisService::noOfDataAttr; i++)
    {
        for (int j = 0; j < DamisService::noOfDataRows; j++)
        {
            DamisService::tmpDataVector.push_back(this->getDataDoubleFormat().at(j).at(i));
        }
            double min = *std::min_element(DamisService::tmpDataVector.begin(), DamisService::tmpDataVector.end());
            double max = *std::max_element(DamisService::tmpDataVector.begin(), DamisService::tmpDataVector.end());
            double mean = HelperMethods::getMean(DamisService::tmpDataVector);
            double stdev = HelperMethods::getStd(DamisService::tmpDataVector); //daliname is N o ne is N-1

            size_t n = DamisService::tmpDataVector.size() / 2;
            nth_element(DamisService::tmpDataVector.begin(), DamisService::tmpDataVector.begin()+ n, DamisService::tmpDataVector.end());

            double median = DamisService::tmpDataVector.at(n);
            DamisService::tmpDataVector.clear();

           //  Statistics::tmpDataVector = {min, max, mean, stdev, median}; if C11 compiler
           DamisService::tmpDataVector.push_back(min);
           DamisService::tmpDataVector.push_back(max);
           DamisService::tmpDataVector.push_back(mean);
           DamisService::tmpDataVector.push_back(stdev);
           DamisService::tmpDataVector.push_back(median);

            DamisService::writeData.push_back(DamisService::tmpDataVector);
            DamisService::tmpDataVector.clear();
    }

    this->writeDataToFile(outFile->getFilePath(), prepareDataSection(DamisService::writeData, dummy), prepareAttributeSection(attrNames, dummy, dummy));

}


Statistics::~Statistics(){

}
