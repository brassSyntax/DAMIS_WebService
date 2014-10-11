///////////////////////////////////////////////////////////
//  DamisService.cpp
//  Implementation of the Class DamisService
//  Created on:      05-Vas-2014 17:36:42
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "ServeRequest.h"
#include "logging/easylogging++.h"
#include "HelperMethods.h"
#include <vector>


ServeRequest::ServeRequest(InitDamisService* inFile)
{
    ServeRequest::serveFile = inFile;
    ServeRequest::writeData.reserve(0);
    ServeRequest::tmpDataVector.reserve(0);

    this->outFile = new DamisFile("_output_");
    this->statFile = new DamisFile("_algStat_");
}
/*
std::vector<std::string> DamisService::prepareAttributeSection(std::vector<std::string> attributeNameList,  std::vector<std::string> attributeTypeList, std::vector<int> possibleClasses)
{
    LOG(INFO) <<"Preparing attribute section for output file writing (string string int)";

    std::vector<std::string> retData;

    retData.reserve(0);

    if (attributeNameList.size() > 0)
    {
        retData.swap(attributeNameList);

        if (!attributeTypeList.empty())
        {
            LOG(INFO) <<"Found type, no class information (string string int)";

            for (int i = 0; i < attributeTypeList.size(); i++)
            {
                retData.at(i).append(" ");
                retData.at(i).append(attributeTypeList.at(i));
            }
        }
        else
        {
            LOG(INFO) <<"No type, no class information (string string int)";
            for (int i = 0; i < attributeNameList.size(); i++)
            {
                retData.at(i).append(" NUMBER");
            }
        }
        if (!possibleClasses.empty())
        {
            LOG(INFO) <<"Faund type and class information (string string int)";

            std::string tmp = "CLASS {";

            for (int i = 0; i < possibleClasses.size() - 1; i++)
            {
                tmp.append(std::to_string(possibleClasses.at(i)));
                tmp.append(", ");
            }
            tmp.append(std::to_string(possibleClasses.at(possibleClasses.size() - 1)));
            tmp.append("}");
            retData.push_back(tmp);
        }
        LOG(INFO) <<"Prepare attribute section OK";
    }
    else
    {
        LOG(WARNING) <<"Something went wrong in preparing attribute section or possibly got error";
    }
    return retData;
}
*/

std::vector<std::string> ServeRequest::prepareAttributeSection(std::vector<std::string> attributeNameList,  std::vector<std::string> attributeTypeList, std::vector<std::string> possibleClasses)    //concats attribute + class
{
    LOG(INFO) <<"Preparing attribute section for output file writing (string string string)";

    std::vector<std::string> retData;

    retData.reserve(0);

    if (attributeNameList.size() > 0)
    {
        retData.swap(attributeNameList);

        if (!attributeTypeList.empty())
        {
            LOG(INFO) <<"Found type, no class information (string string string)";

            for (unsigned int i = 0; i < attributeTypeList.size(); i++)
            {
                retData.at(i).append(" ");
                retData.at(i).append(attributeTypeList.at(i));
            }
        }
        else
        {
            LOG(INFO) <<"No type, no class information (string string string)";
            for (unsigned int i = 0; i < attributeNameList.size(); i++)
            {
                retData.at(i).append(" NUMERIC");
            }
        }
        if (!possibleClasses.empty())
        {
            LOG(INFO) <<"Found type and class information (string string string)";

            std::string tmp = "CLASS {";

            for (unsigned int i = 0; i < possibleClasses.size() - 1; i++)
            {
                tmp.append(possibleClasses.at(i));
                tmp.append(", ");
            }
            tmp.append(possibleClasses.at(possibleClasses.size() - 1));
            tmp.append("}");
            retData.push_back(tmp);
        }
        LOG(INFO) <<"Prepare attribute section OK";
    }
    else
    {
        LOG(WARNING) <<"Something went wrong in preparing attribute section or possibly got error";
    }
    return retData;
} //concats attribute + class

std::vector<std::vector<std::string>> ServeRequest::prepareDataSection(std::vector<std::vector<double>> dataSection,  std::vector<std::string> classAttribute)  //concats data + class
{
    LOG(INFO) <<"Preparing data section for output file writing (double string)";

    std::vector<std::vector<std::string>> retData;
    std::vector<std::string> oneObject;

    retData.reserve(0);
    oneObject.reserve(0);

        char bufferData[32];


    if (!dataSection.empty() && dataSection.at(0).size() > 0 )
    {
        for (unsigned int i = 0; i < dataSection.size(); i++ )
        {
            for (unsigned int j = 0; j < dataSection.at(0).size(); j++)
            {
                snprintf(bufferData, sizeof(bufferData), "%g", dataSection.at(i).at(j));
                oneObject.push_back(bufferData);
//                 std::cout << i << " " << j << " " << dataSection.at(i).at(j);

            }
         //   std::cout << std::endl;
            if (classAttribute.size() == dataSection.size())
            {
                oneObject.push_back(classAttribute.at(i));
            }
            retData.push_back(oneObject);
            oneObject.clear();
        }

        LOG(INFO) <<"Prepare data section (double string) OK";
    }
    else
    {
        LOG(WARNING) <<"No data in data section or possibly got error (double string)";
        // return retData;
    }
    return retData;
}

/*
std::vector<std::vector<std::string>> DamisService::prepareDataSection(std::vector<std::vector<std::string>> dataSection,  std::vector<int> classAttribute)  //concats data + class
{
    LOG(INFO) <<"Preparing data section for output file writing (string int)";
    std::vector<std::vector<std::string>> retData;
    retData.reserve(0);

    if (!dataSection.empty() && dataSection.at(0).size() > 0 )
    {
        retData.swap(dataSection);
        if (classAttribute.size() == dataSection.size())
        {
            for (int i = 0; i < classAttribute.size(); i++ )
            {
                retData.at(i).push_back(std::to_string(classAttribute.at(i)));
            }
            LOG(INFO) <<"Prepare data section OK";
        }
    }
    else
    {
        LOG(WARNING) <<"No data in data section or possibly got error (string int)";
    }
    return retData;
}
*/

/*
std::vector<std::vector<std::string>> DamisService::prepareDataSection(std::vector<std::vector<std::string>> dataSection,  std::vector<std::string> classAttribute)  //concats data + class
{

    LOG(INFO) <<"Preparing data section for output file writing (string string)";
    std::vector<std::vector<std::string>> retData;
    retData.reserve(0);

    if (dataSection.empty() && dataSection.at(0).size() > 0 )
    {
        int sk = dataSection.size();
        int sk2 = classAttribute.size();
        retData.swap(dataSection);
        if (sk == sk2)
        {
            for (int i = 0; i < classAttribute.size(); i++ )
            {
                retData.at(i).push_back(classAttribute.at(i));
            }
            LOG(INFO) <<"Prepare data section OK";
        }
    }
    else
    {
        LOG(WARNING) <<"No data in data section or possibly got error (string string)";
    }
    return retData;
}

*/

/*
std::vector<std::vector<std::string>> DamisService::prepareDataSection(std::vector<std::vector<double>> dataSection,  std::vector<int> classAttribute)  //concats data + class
{
    LOG(INFO) <<"Preparing data section for output file writing (double int)";

    std::vector<std::vector<std::string>> retData;
    std::vector<std::string> oneObject;

    retData.reserve(0);
    oneObject.reserve(0);

    if (dataSection.at(0).size() > 0 )
    {
        for (int i = 0; i < dataSection.size(); i++ )
        {
            for (int j = 0; j < dataSection.at(0).size(); j++)
            {
                oneObject.push_back(std::to_string(dataSection.at(i).at(j)));
            }
            if (classAttribute.size() == dataSection.size())
            {
                oneObject.push_back(std::to_string(classAttribute.at(i)));
            }
            retData.push_back(oneObject);
            oneObject.clear();
        }
        LOG(INFO) <<"Prepare data section OK";
    }
    else
    {
        LOG(WARNING) <<"No data in data section or possibly got error (double int)";
    }
    return retData;
}

*/

/*std::string DamisService::getAltOutputHttp()
{
    return DamisService::altHttpPath;
}*/
/*std::string DamisService::getAltOutputPath()
{
    return DamisService::altLocalPath;
}*/

void ServeRequest::writeDataToFile(std::string path, std::vector<std::vector<std::string>> data, std::vector<std::string> attributeData)
{
    LOG (INFO) << "Starting data write to output file " << path;
    std::ofstream file (path);

    if (!attributeData.empty())
    {
        int noOfAttrInAttr = attributeData.size();
        if (noOfAttrInAttr > 0)
        {
            file<<"%"<<std::endl;

            for (int i = 0; i < noOfAttrInAttr; i++)
                file<<"@ATTRIBUTE " << attributeData.at(i) << std::endl;

            file<<"%"<<std::endl;
            LOG (INFO) << "Writing to file " << path << " attribute section OK";
            file<<"@DATA"<<std::endl;

            if (!data.empty())
            {
                int noOfAttrInData = data.at(0).size();
                int noOfObjects = data.size();
                if (noOfObjects > 0)
                {
                    for (int i = 0; i < noOfObjects; i++)
                    {
                        for (int j = 0; j < noOfAttrInData - 1; j++)
                          // {
                             file << data.at(i).at(j) << ", ";
                         //    std::cout <<
                          // }
                        file<<data.at(i).at(noOfAttrInData - 1)<<std::endl; //last attribute - do not add comma at the end
                    }
                    LOG (INFO) << "Writing to file " << path << " succeeded OK";
                }
            }
            else
            {
                LOG (ERROR) << "Writing to file " << path << " data section unsucessfull (probably no data)";
            }
        }
        else
        {
            LOG (ERROR) << "Writing to file " << path << " attribute section unsucessfull";
        }
    }
    else
    {
        file.close();
        HelperMethods::deleteFile(path);
        LOG (ERROR) << "No data in attribute section, aborting write";
    }
    file.close();
}

/*void DamisService::initialize()
{
    LOG(INFO) << "Initiating file " << DamisService::serveFile->getFilePath() << " data conversion to double format ";

    std::vector<double> tmpObject;
    int dataRows = DamisService::serveFile->getRawDataStringFormat().size();
    int dataColumns = DamisService::serveFile->getRawDataStringFormat().at(0).size();


    ///  int columns = arrffFile.getData()[0].size();
    //    int rows = arrffFile.getData().size();
    //    double a = arrffFile.getData()[0][1];*/
 /*   bool isFileValid = true;
    bool classMatch;
    bool isDataValid;
    if ( dataRows > 0 && dataColumns > 0)
    {
        for (unsigned int i = 0; i < dataRows; i++ )
        {
            // skipObject = false; //if data values are ok
            classMatch = false; //if match found
            isDataValid = true;


            for (unsigned int j = 0; j < dataColumns; j++)
            {
                std::string dataVal = DamisService::serveFile->getRawDataStringFormat().at(i).at(j);
               // std::cout << dataVal << " ";
                // double doubleVal = atof(dataVal.c_str());
                // std::string tmpStrVal= std::to_string(doubleVal);

                const char *str = dataVal.c_str();
                char *err;
                double x = strtod(str, &err);

                if (*err == 0 && dataVal !="")
                {
                    //std::cout << x;
                    //uncomment on data cleaning
                    tmpObject.push_back(atof(dataVal.c_str()));
                    //double tmp = tmpObject[j];
                    //std::cout << std::to_string(tmp);
                }
                else
                {
                    //   std::cout << *err;
                    isDataValid = false;
                    isFileValid = false;
                    //  ErrorResponse::setFaultDetail("File is not valid, found non numeric value \\ " + std::string(ValidateParams::initDamisServFile->getRawDataStringFormat().at(i).at(j)) + " \\ at line " + std::to_string (i) + std::string("at position ") + std::to_string (j) + std::string(" "));
                    LOG(ERROR) << "File is not valid (skipping object), found non numeric value \\ " << DamisService::serveFile->getRawDataStringFormat().at(i).at(j) << " \\ at line " <<i <<" at position " <<j;

                    //uncomment on data cleaning
                    //skipObject = true; //not needed for
                    //break; //comment on data cleaning

                }
            }

            //these two lines must be uncomented in data cleaning
            if (isDataValid) // if data section is ok checking if class provided in an object corresponds to class provided in the atrribute section
            {
                if (DamisService::serveFile->isClassFound())
                {
                    int clIndex;

                    for (unsigned int cL = 0; cL < DamisService::serveFile->getClasses().size(); cL++)
                    {

// TODO (Povilas#2#): Palyginti didziosiomis arba mazosiomis raidemis ...
//
                        if (DamisService::serveFile->getClassAttribute().at(i) ==  DamisService::serveFile->getClasses().at(cL))
                        {
                            // std::cout << tmp[i];
                            classMatch = true;
                            clIndex = cL;
                            break;
                        }
                    }
                    if (classMatch)
                    {
                        //uncomment on data cleaning
                        DamisService::intClassAttributes.push_back(clIndex);
                        DamisService::stringClassAttributes.push_back(DamisService::serveFile->getClasses().at(clIndex));
                        //std::cout << DamisService::serveFile->getClasses().at(clIndex) << " \n";
                    }
                    else
                    {
                        isFileValid = false;
                        isDataValid = false;
                        // ErrorResponse::setFaultDetail(std::string("File is not valid. Data section has unknown class ") + std::string (ValidateParams::initDamisServFile->getClassAttribute().at(i)) + std::string(" at line ") + std::to_string(i) + std::string(" "));
                        LOG(ERROR) << "File is not valid. Data section line " << i << " contains unknown class " << DamisService::serveFile->getClassAttribute().at(i);
                        //  break; //comment on data cleaning
                    }
                }
               // uncomment on data cleaning*/

          /*  } //uncomment on data cleaning
            if (isDataValid)
            {
                DamisService::rawDoubleData.push_back(tmpObject); //add if there were no skip objects
            }
            tmpObject.clear();
        }
    }

    if (!isFileValid)
    {
        LOG(ERROR) << "File " << DamisService::serveFile->getFilePath() << " is not valid";

        // must comment since deletion is moved to ValidateParams::isValid function
        //HelperMethods::deleteFile(ValidateParams::initDamisServFile->getFilePath());
    }
    else
    {
        LOG(INFO) << "File " << DamisService::serveFile->getFilePath() << " data to double converssion suceeded (OK)";
    }

}*/

/*std::vector<std::vector<double>> DamisService::getDataDoubleFormat()
{
    return DamisService::rawDoubleData;
}

std::vector<int> DamisService::getIntClassAttribute()
{
    return DamisService::intClassAttributes;
}

std::vector<std::string> DamisService::getStringClassAttribute()
{
    return DamisService::stringClassAttributes;
}*/

ServeRequest::~ServeRequest()
{

}
