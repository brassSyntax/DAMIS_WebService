///////////////////////////////////////////////////////////
//  Preprocess.cpp
//  Implementation of the Class Preprocess
//  Created on:      05-Vas-2014 17:36:45
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "Preprocess.h"
#include "HelperMethods.h"
#include <math.h>
#include <numeric>
#include "logging/easylogging++.h"


Preprocess::Preprocess(InitDamisService* initFile):ServeRequest(initFile)
{
    LOG (INFO) << "Data preprocessing has been called";
//pakeista
altOutFile = NULL;

// Preprocess::writeClass.reserve(0);
   // ServeRequest::writeData.reserve(0);
   // ServeRequest::tmpDataVector.reserve(0);

   /* DamisService::noOfDataRows = this->getDataDoubleFormat().size();
    DamisService::noOfDataAttr = this->getDataDoubleFormat().at(0).size();*/
}

void Preprocess::filterData(bool retFilteredData, double zValue, int attrIndex)
{
    LOG (INFO) << "Initiating filter data function. Got parameters retFilteredData - " << retFilteredData <<" zValue - " << zValue<< " attrIndex - " << attrIndex;

    attrIndex = attrIndex - 1; //clumn indexing starts at 0

    std::vector<int> exObjIndex;
    int noOfObjects = serveFile->getNumberOfObjects();
    exObjIndex.reserve(0);

    ServeRequest::tmpDataVector.reserve(noOfObjects);
    std::vector<std::vector<double>> currentData = serveFile->getDoubleData();
    //double mean = 0;

    for (int j = 0; j < noOfObjects; j++)
        ServeRequest::tmpDataVector.push_back(currentData[j][attrIndex]);

    //mean = HelperMethods::getMean(DamisService::tmpDataVector);
    double checkStdev (HelperMethods::getStd(ServeRequest::tmpDataVector) * zValue); //daliname is N o ne is N-1

    for (int i = 0; i < noOfObjects; i++)
        if (fabs(ServeRequest::tmpDataVector.at(i)) > checkStdev)
            exObjIndex.push_back(i);

    ServeRequest::tmpDataVector.clear();

    for (int i = 0; i < noOfObjects; i++)
    {
        bool skip (false);

        for (int z = 0; z < exObjIndex.size(); z++)
            if (i == exObjIndex.at(z))
            {
                skip = true;
                break;
            }

        for (int j = 0; j < serveFile->getNumberOfAttributes(); j++)
        {
            if (retFilteredData ^ skip){
                ServeRequest::tmpDataVector.push_back(currentData[i][j]);
                LOG(INFO) << i << " " << j;}
            else
                 break;
        }
        if (!ServeRequest::tmpDataVector.empty())
            {
                if (serveFile->isClassFound())
                    Preprocess::writeClass.push_back(serveFile->getStringClass().at(i));
                ServeRequest::writeData.push_back(ServeRequest::tmpDataVector);

                ServeRequest::tmpDataVector.clear();
            }
    }

    this->writeDataToFile(outFile->getFilePath(),prepareDataSection(ServeRequest::writeData, Preprocess::writeClass), prepareAttributeSection(serveFile->getAttributeName(),serveFile->getAttributeType(),serveFile->getStringClassAttribute()));
}

void Preprocess::normData(bool normMeanStd, double a, double b)
{
    LOG (INFO) << "Initializing norm data function. Got parameters normMeanStd - "<<normMeanStd <<" a - " <<a << " b - " << b;
    int numbOfObj (serveFile->getNumberOfObjects());
    std::vector<std::vector<double>> currentData = serveFile->getDoubleData();

    ServeRequest::tmpDataVector.reserve(numbOfObj); //norms each columns not row, thus initialize elements for total row number
    ServeRequest::writeData.reserve(numbOfObj);

    //gal galima atsisakyti 6i8 dviej7 cikl7

    for (int i = 0; i < serveFile->getNumberOfAttributes(); i++) //need to calculate mean and std for each attribute in every row
    {
        for (int j = 0; j < numbOfObj; j++)
            {
                if(i==0)
                    ServeRequest::writeData.push_back(currentData[i]);
                ServeRequest::tmpDataVector.push_back(currentData[j][i]);
               // std::cout << i << " " << j << " " << tmpDataVector.at(j) << std::endl;
            }

        if (normMeanStd) //make average equal to a and dispersion equal to b
        {
            double mean = HelperMethods::getMean(ServeRequest::tmpDataVector);
            double stdev = HelperMethods::getStd(ServeRequest::tmpDataVector); //daliname is N o ne is N-1

            for (int z = 0; z < numbOfObj; z++) // update attribute values in each row
                ServeRequest::writeData.at(z).at(i) = (ServeRequest::tmpDataVector.at(z) - mean + a) * b / stdev; //update data
        }
        else
        {
            double min = *std::min_element(ServeRequest::tmpDataVector.begin(), ServeRequest::tmpDataVector.end());
            double max = *std::max_element(ServeRequest::tmpDataVector.begin(), ServeRequest::tmpDataVector.end());

            for (int z = 0; z < numbOfObj; z++) // update attribute values in each row
                    ServeRequest::writeData.at(z).at(i) = a + (double)((ServeRequest::tmpDataVector.at(z) - min) * (b - a)) / (max - min); //update data
        }
        ServeRequest::tmpDataVector.clear();
    }
    this->writeDataToFile(outFile->getFilePath(),prepareDataSection(ServeRequest::writeData, serveFile->getStringClass()), prepareAttributeSection(serveFile->getAttributeName(),serveFile->getAttributeType(),serveFile->getStringClassAttribute()));
}

void Preprocess::splitData(bool reshufleObjects,double firstSubsetPerc, double secondSubsetPerc)
{
    LOG (INFO) << "Initiaing data split. Got parameters reshufleObjects - "<< reshufleObjects <<" firstSubsetPerc - " <<firstSubsetPerc << " secondSubsetPerc - "<< secondSubsetPerc;

   // ServeRequest::tmpDataVector.resize(serveFile->getNumberOfAttributes());
    int objNo = serveFile->getNumberOfObjects();

    std::vector<int> objIndex;
    objIndex.reserve(objNo);

    int firstCount = ceil(firstSubsetPerc / 100.0 * objNo);  //qtt of vectors in first set
    int secondCount = ceil(secondSubsetPerc / 100.0 * objNo);                //qtt of vectors in second set

    if ((firstCount + secondCount) > objNo)
            secondCount -= objNo - (firstCount + secondCount);

  //  ServeRequest::writeData.resize(firstCount, ServeRequest::tmpDataVector);
   // Preprocess::writeClass.resize(firstCount);

    std::vector<std::vector<double>> secondData; secondData.reserve(secondCount);
    std::vector<std::string> secondClass; secondClass.reserve(secondCount);

    //initialize vector with index values
    for( int i = 0; i < objNo; i++)
        objIndex.push_back(i);

    if (reshufleObjects)
    {
        //chanege index order
        for (int i = 0; i < objNo * 5; i++)
        {
            //generate indexes
            int fIndex (HelperMethods::getRrandomInRange(0, objNo));
            int sIndex (HelperMethods::getRrandomInRange(0, objNo));
            //change index order

            int tmp (objIndex.at(fIndex));

            objIndex.at(fIndex) = objIndex.at(sIndex);
            objIndex.at(sIndex) = tmp;
        }
    }

    std::vector<std::vector<double>> currentData = serveFile->getDoubleData();
    for (int i = 0; i < firstCount + secondCount; i++)
    {
       //std::cout << i;
        for (int j = 0; j < serveFile->getNumberOfAttributes(); j++)
            ServeRequest::tmpDataVector.push_back(currentData[objIndex.at(i)][j]);

        if (i < firstCount)
        {
            if (serveFile->isClassFound())
                Preprocess::writeClass.push_back(serveFile->getStringClass().at(objIndex.at(i)));
            ServeRequest::writeData.push_back(ServeRequest::tmpDataVector);
        }
        else
        {
            if (serveFile->isClassFound())
                secondClass.push_back(serveFile->getStringClass().at(objIndex.at(i)));
            secondData.push_back(ServeRequest::tmpDataVector);
        }
        ServeRequest::tmpDataVector.clear();
    }

    this->writeDataToFile(outFile->getFilePath(),prepareDataSection(ServeRequest::writeData , Preprocess::writeClass),prepareAttributeSection(serveFile->getAttributeName(),serveFile->getAttributeType(),serveFile->getStringClassAttribute()));

    this->altOutFile = new DamisFile("_output_");

    this->writeDataToFile(altOutFile->getFilePath(),prepareDataSection(secondData , secondClass),prepareAttributeSection(serveFile->getAttributeName(),serveFile->getAttributeType(),serveFile->getStringClassAttribute()));
}

void Preprocess::transposeData()
{
    LOG (INFO) << "Initiating data transpose";

   /* ServeRequest::tmpDataVector.resize(serveFile->getNumberOfAttributes());
    ServeRequest::writeData.resize(serveFile->getNumberOfObjects(), ServeRequest::tmpDataVector);*/

    std::vector<std::string> attrNames;

    for (int i = 0; i < serveFile->getNumberOfObjects(); i++) //rows becomes attributes
    {
        std::string tmp("attr");
        tmp.append(std::to_string(static_cast<long long>(i+1)));
        tmp.append(" NUMERIC");
        attrNames.push_back(tmp);
    }

    std::vector<std::vector<double>> currentData = serveFile->getDoubleData();
    for (int i = 0; i < serveFile ->getNumberOfAttributes(); i++)
    {
        for (int j = 0; j < serveFile->getNumberOfObjects(); j++)
            ServeRequest::tmpDataVector.push_back(currentData[j][i]);

        ServeRequest::writeData.push_back(ServeRequest::tmpDataVector);
        ServeRequest::tmpDataVector.clear();
    }
    {
        std::vector<std::string> dummy; dummy.reserve(0); //pass dummy vector to write function since no attribute after transform are left
        this->writeDataToFile(outFile->getFilePath(), prepareDataSection(ServeRequest::writeData, dummy), prepareAttributeSection(attrNames, dummy, dummy));
    }
 }

void Preprocess::cleanData()
{
    LOG (INFO) << "Initiating data cleaning";
    this->writeDataToFile(outFile->getFilePath(), prepareDataSection(serveFile->getDoubleData(), serveFile->getStringClass()),prepareAttributeSection(serveFile->getAttributeName(),serveFile->getAttributeType(),serveFile->getStringClassAttribute()));
}

Preprocess::~Preprocess()
{

}
