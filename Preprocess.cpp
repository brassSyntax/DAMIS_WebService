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
#include "logging\easylogging++.h"


Preprocess::Preprocess(InitDamisServiceFile* initFile):DamisService(initFile)
{
    LOG (INFO) << "Data preprocessing has been called";
    Preprocess::writeClass.reserve(0);

   /* DamisService::writeData.reserve(0);
    DamisService::tmpDataVector.reserve(0);

    DamisService::noOfDataRows = this->getDataDoubleFormat().size();
    DamisService::noOfDataAttr = this->getDataDoubleFormat().at(0).size();*/
}

void Preprocess::filterData(bool retFilteredData, double zValue, int attrIndex)
{
    LOG (INFO) << "Initiating filter data function. Got parameters retFilteredData - " << retFilteredData <<" zValue - " << zValue<< " attrIndex - " << attrIndex;

    //attrIndex = attrIndex - 1;

    std::vector<int> exObjIndex;
    exObjIndex.reserve(0);

    //double mean = 0;
    double stdev = 0;

    for (int j = 0; j < DamisService::noOfDataRows; j++)
        DamisService::tmpDataVector.push_back(this->getDataDoubleFormat().at(j).at(attrIndex));

    //mean = HelperMethods::getMean(DamisService::tmpDataVector);
    stdev = HelperMethods::getStd(DamisService::tmpDataVector); //daliname is N o ne is N-1

    for (int i = 0; i < DamisService::noOfDataRows; i++)
        if (fabs(DamisService::tmpDataVector.at(i))> zValue * stdev)
            exObjIndex.push_back(i);

    DamisService::tmpDataVector.clear();

    bool skip;
    for (int i = 0; i < DamisService::noOfDataRows; i++)
    {
        skip = false;
        for (unsigned int z = 0; z < exObjIndex.size(); z++)
            if (i == exObjIndex.at(z))
            {
                skip = true;
                break;
            }
        for (int j = 0; j < DamisService::noOfDataAttr; j++)
        {
            if (retFilteredData && !skip)
                DamisService::tmpDataVector.push_back(this->getDataDoubleFormat().at(i).at(j));
            else if (!retFilteredData && skip)
                DamisService::tmpDataVector.push_back(this->getDataDoubleFormat().at(i).at(j));
            else
                 break;
        }
        if (!DamisService::tmpDataVector.empty())
            {
                if (serveFile->isClassFound())
                    Preprocess::writeClass.push_back(this->getStringClassAttribute().at(i));
                DamisService::writeData.push_back(DamisService::tmpDataVector);
                DamisService::tmpDataVector.clear();
            }
    }
    this->writeDataToFile(outFile->getFilePath(),prepareDataSection(this->writeData, this->writeClass), prepareAttributeSection(serveFile->getAttributeNames(),serveFile->getAttributeTypes(),serveFile->getClasses()));
}

void Preprocess::normData(bool normMeanStd, double a, double b)
{
    LOG (INFO) << "Initialiing norma data function. Got paramters normMeanStd - "<<normMeanStd <<" a - " <<a << " b - " << b;

    for (int i = 0; i < DamisService::noOfDataRows; i++) //need to calculate mean and std for each attribute in every row
    {
        for (int j = 0; j < DamisService::noOfDataAttr; j++)
            DamisService::tmpDataVector.push_back(this->getDataDoubleFormat().at(i).at(j));

        DamisService::writeData.push_back(DamisService::tmpDataVector);
        DamisService::tmpDataVector.clear();
    }

    for (int i = 0; i < DamisService::noOfDataAttr; i++) //need to calculate mean and std for each attribute in every row
    {
        for (int j = 0; j < DamisService::noOfDataRows; j++)
            DamisService::tmpDataVector.push_back(DamisService::writeData.at(j).at(i));

        if (normMeanStd) //make average equal to a and dispersion equal to b
        {
            double mean = HelperMethods::getMean(DamisService::tmpDataVector);
            double stdev = HelperMethods::getStd(DamisService::tmpDataVector); //daliname is N o ne is N-1

            for (int z = 0; z < DamisService::noOfDataRows; z++) // update attribute walues in each row
                DamisService::writeData.at(z).at(i) = (DamisService::tmpDataVector.at(z) - mean + a) * b / stdev; //update data
        }
        else
        {
            double min = *std::min_element(DamisService::tmpDataVector.begin(), DamisService::tmpDataVector.end());
            double max = *std::max_element(DamisService::tmpDataVector.begin(), DamisService::tmpDataVector.end());

            for (int z = 0; z < DamisService::noOfDataRows; z++) // update attribute walues in each row
                DamisService::writeData.at(z).at(i) = a + (double)((DamisService::writeData.at(z).at(i) - min ) * (b - a)) / (max - min); //update data
        }
        DamisService::tmpDataVector.clear();
    }
    this->writeDataToFile(outFile->getFilePath(),prepareDataSection(DamisService::writeData, this->getStringClassAttribute()), prepareAttributeSection(serveFile->getAttributeNames(),serveFile->getAttributeTypes(),serveFile->getClasses()));
}

void Preprocess::splitData(bool reshufleObjects,double firstSubsetPerc, double secondSubsetPerc)
{
    LOG (INFO) << "Initiaing data split. Got parameters reshufleObjects - "<< reshufleObjects <<" firstSubsetPerc - " <<firstSubsetPerc << " secondSubsetPerc - "<< secondSubsetPerc;

    std::vector<int> objIndex;
    objIndex.reserve(DamisService::noOfDataRows);

    int firstCount = ceil(firstSubsetPerc / 100.0 * DamisService::noOfDataRows);  //qtt of vectors in first set
    int secondCount = secondSubsetPerc / 100.0 * DamisService::noOfDataRows;                //qtt of vectors in second set

    if ((firstCount + secondCount) > DamisService::noOfDataRows)
            secondCount -=DamisService::noOfDataRows - (firstCount + secondCount);

    //initialize vector with index values
    for( int i = 0; i < DamisService::noOfDataRows; i++)
        objIndex.push_back(i);

    int fIndex, sIndex; // index that mvalues must be swaped
    int tmp;

    if (reshufleObjects)
    {
        //chanege index order
        for (int i = 0; i < DamisService::noOfDataRows * 5; i++)
        {
            //generate indexes
            fIndex = HelperMethods::getRrandomInRange(0, DamisService::noOfDataRows);
            sIndex = HelperMethods::getRrandomInRange(0, DamisService::noOfDataRows);
            //change index order
            tmp = objIndex.at(fIndex);
            objIndex.at(fIndex) = objIndex.at(sIndex);
            objIndex.at(sIndex) = tmp;
        }
    }

    std::vector<std::vector<double>> secondData;
    secondData.reserve(0);
    std::vector<std::string> secondClass;
    secondClass.reserve(0);


    for (int i = 0; i < firstCount + secondCount; i++)
    {
       //std::cout << i;
        for (int j = 0; j < DamisService::noOfDataAttr; j++)
            DamisService::tmpDataVector.push_back(this->getDataDoubleFormat().at(objIndex.at(i)).at(j));

        if (i < firstCount)
        {
            if (serveFile->isClassFound())
                Preprocess::writeClass.push_back(this->getStringClassAttribute().at(objIndex.at(i)));
            DamisService::writeData.push_back(DamisService::tmpDataVector);
        }
        else
        {
            if (serveFile->isClassFound())
                secondClass.push_back(this->getStringClassAttribute().at(objIndex.at(i)));
            secondData.push_back(DamisService::tmpDataVector);
        }
        DamisService::tmpDataVector.clear();
    }

    this->writeDataToFile(outFile->getFilePath(),prepareDataSection(DamisService::writeData , Preprocess::writeClass),prepareAttributeSection(serveFile->getAttributeNames(),serveFile->getAttributeTypes(),serveFile->getClasses()));

    this->altOutFile = new DamisFile("_output_");

    this->writeDataToFile(altOutFile->getFilePath(),prepareDataSection(secondData , secondClass),prepareAttributeSection(serveFile->getAttributeNames(),serveFile->getAttributeTypes(),serveFile->getClasses()));
}

void Preprocess::transposeData()
{
    LOG (INFO) << "Initiating data transpose";

    std::vector<std::string> attrNames;
    std::string tmp = "attr_";

    for (int i = 0; i < DamisService::noOfDataRows; i++) //rows becomes attributes
    {
        tmp.append(std::to_string(i+1));
        tmp.append(" NUMBER");
        attrNames.push_back(tmp);
        tmp = "attr_";
    }

    std::vector<std::string> dummy;
    dummy.reserve(0); //pass dummy vector to write function since no attributre after transform are left

    for (int i = 0; i < DamisService::noOfDataAttr; i++)
    {
        for (int j = 0; j < DamisService::noOfDataRows; j++)
        {
            DamisService::tmpDataVector.push_back(this->getDataDoubleFormat().at(j).at(i));
        }
        DamisService::writeData.push_back(DamisService::tmpDataVector);
        DamisService::tmpDataVector.clear();
    }
    this->writeDataToFile(outFile->getFilePath(), prepareDataSection(DamisService::writeData, dummy), prepareAttributeSection(attrNames, dummy, dummy));
}

void Preprocess::cleanData()
{
    LOG (INFO) << "Initiating data cleaning";
    this->writeDataToFile(outFile->getFilePath(), prepareDataSection(this->getDataDoubleFormat(), this->getStringClassAttribute()),prepareAttributeSection(serveFile->getAttributeNames(),serveFile->getAttributeTypes(),serveFile->getClasses()));
}

Preprocess::~Preprocess()
{

}
