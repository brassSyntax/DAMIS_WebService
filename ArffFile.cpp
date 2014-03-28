///////////////////////////////////////////////////////////
//  ArffFile.cpp
//  Implementation of the Class ArffFile
//  Created on:      30-Sau-2014 17:06:17
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "ArffFile.h"
#include "ErrorResponse.h"
#include "HelperMethods.h"
#include "logging\easylogging++.h"

#include <string>

/**
 * Class contructor. Initializes structures for data section, attribute names, attribute types, class atrribute, object attribute, no of atributes of the arff file
 */
ArffFile::ArffFile(std::string pref) : DamisFile(pref) //call super class constructor and pass prefix
{
    attributeNames.reserve(0);
    attributeTypes.reserve(0);

    rawStringData.reserve(0);

    objectClasses.reserve(0);
    classAttribute.reserve(0);

    numberOfAttributes = 0;
    classFound = false;

}

ArffFile::~ArffFile()
{

}

/**
 * Returns vector of attribute names
 */
std::vector<std::string> ArffFile::getAttributeNames()
{

    return  ArffFile::attributeNames;
}
/**
 * Method that returns attribute types
 */
std::vector<std::string> ArffFile::getAttributeTypes()
{

    return  ArffFile::attributeTypes;
}
/**
 * Returns valid class list that objects may have
 */
std::vector<std::string> ArffFile::getClasses()
{

    return  ArffFile::objectClasses;
}
/**
 * Returns raw data in string format
 */
std::vector<std::vector<std::string>> ArffFile::getRawDataStringFormat()
{
    return  rawStringData;
}
/**
 * Returns number of attributes that must be presnet in each data section line
 */
int ArffFile::getNumberOfAttributes()
{

    return  numberOfAttributes;
}
/**
 * Reads the content of the arff file into string variables and validates the file
 */
bool ArffFile::readArffFile()
{

    LOG(INFO) << "Initiating arff file " << DamisFile::getFilePath() << " read";

    std::stringstream s;
    std::ifstream file (DamisFile::getFilePath());
    std::string line_from_file;
    std::string tmp1, tmp2, tmp3;
    std::vector<std::string> tmp, tmp4;
    std::vector<std::string> stringVector;
    int line_no = 1;
    if (file.is_open() != false)
    {
        while (std::getline(file, line_from_file))
        {
            if (line_from_file.length() == 0)
            {
                line_no++;
                continue;
            }
            std::istringstream iss(line_from_file);
            std::string sub;
            iss >> sub;
            if (sub == "%")
            {
                line_no++;
                continue;
            }
            else
            {
                std::transform(sub.begin(), sub.end(), sub.begin(), ::toupper);
                if (sub == "@ATTRIBUTE")
                {
                    iss >> tmp1;
                    iss >> tmp2;

                    std::transform(tmp1.begin(), tmp1.end(), tmp1.begin(), ::toupper);
                    std::transform(tmp2.begin(), tmp2.end(), tmp2.begin(), ::toupper);
                    if (tmp1 == "CLASS" || tmp1 == "'CLASS'")
                    {

                        ArffFile::classFound = true;
                        ArffFile::attributeClassIndex = ArffFile::attributeNames.size();
                        LOG(INFO) << "Found CLASS attribute at index " << ArffFile::attributeClassIndex;

                        int startClassList = line_from_file.find("{");
                        int endClassList = line_from_file.find("}");

                        if (startClassList != std::string::npos && endClassList != std::string::npos) //if both brackets faund
                        {
                            tmp3.assign(line_from_file, startClassList + 1, endClassList - startClassList - 1);

                            LOG(INFO) << "Classes are " << tmp3;
                            tmp4 = HelperMethods::split(tmp3, ','); //split resulting string

                            for (unsigned int i = 0; i < tmp4.size(); i++) //push back each class label
                            {
                                //trim spaces from start
                                tmp4[i].erase(tmp4[i].begin(), std::find_if(tmp4[i].begin(), tmp4[i].end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
                                //trim spaces from end
                                tmp4[i].erase(std::find_if(tmp4[i].rbegin(), tmp4[i].rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), tmp4[i].end());

                                ArffFile::objectClasses.push_back(tmp4[i]);
                            }
                        }
                        else
                        {
                            ErrorResponse::setFaultDetail(std::string("Invalid class definition in attribute list, cannot find pair of brackets {} at line ") + std::to_string(line_no) );
                            LOG(ERROR) << "Class atribute is ill defined, cannot find {} brackets";
                         //   file.close();

                        //    HelperMethods::deleteFile(DamisFile::getFilePath());

                        //    return false; //returns error code
                        }
                    }
                    else if (tmp2 == "REAL"  || tmp2 == "INTEGER" || tmp2 == "NUMERIC")
                    {
                        ArffFile::numberOfAttributes++;
                        ArffFile::attributeNames.push_back(tmp1);
                        ArffFile::attributeTypes.push_back(tmp2);
                    }
                }
                else if (sub == "@DATA" || sub == "@RELATION")
                {
                    line_no++;
                    continue;
                }
                else
                {
                    tmp = HelperMethods::split(line_from_file, ','); //split data section by comma

                    int noOfAttr = (ArffFile::isClassFound()) ? ArffFile::getNumberOfAttributes() + 1 : ArffFile::getNumberOfAttributes(); //if there are class add 1 to atrributes

                    if (tmp.size() == noOfAttr)
                    {
                        for (unsigned int i = 0; i < tmp.size(); i++) // for each attribute in each data line
                        {
                            if (ArffFile::isClassFound() && (i == ArffFile::getClassAttributeIndex()))
                            {
                                ArffFile::classAttribute.push_back(tmp[i]); //save class attribute
                            }
                            else
                            {
                                stringVector.push_back(tmp[i]); //save data attribute
                            }
                        }
                        ArffFile::rawStringData.push_back(stringVector); //add to matrix
                        stringVector.clear(); //clear vector
                    }
                    else
                    {
                        ErrorResponse::setFaultDetail(std::string("Number of features ") + std::to_string(tmp.size()) + std::string(" at line ") + std::to_string(line_no) + std::string(" in @Data section of arff file does not correspond to number of attributes ") + std::to_string(ArffFile::getNumberOfAttributes()) + std::string(" of the @Attribute section "));
                        LOG(ERROR) << "Data section line " << line_no << " does not have required quantity features ";
                        //file.close();

                      //  HelperMethods::deleteFile(DamisFile::getFilePath());

                      //  return false; //returns error code
                    }
                }
            }
            line_no++;
        }
        file.close();
    }
    else
    {
        ErrorResponse::setFaultDetail("Cannot open file for reading into string data structure");
        LOG(ERROR) << "Cannot open file " << DamisFile::getFilePath() << " for reading into ARFF object";
    }

    if(ErrorResponse::isFaultFound() || ArffFile::rawStringData.empty() || ArffFile::attributeNames.empty())
    {
        file.close();
        ErrorResponse::setFaultDetail("Data file does not contain either attribute either correct data section");
        LOG(ERROR) << "Data file " << DamisFile::getFilePath() << " does not contain either attribute either correct data section";
        HelperMethods::deleteFile(DamisFile::getFilePath());
        return false;
    }
    else
    {
        LOG(INFO) << "Reading file " << DamisFile::getFilePath() << " into string format was OK";
        return true;
    }
}
/**
 * Returns class attribute index in arff file
 */
int ArffFile::getClassAttributeIndex()
{
    return ArffFile::attributeClassIndex;
}
/**
 * Returns indicates is class attribute found
 */
bool ArffFile::isClassFound()
{
  return ArffFile::classFound;
}
/**
 * Function returns vector of class attributes
 */
std::vector<std::string> ArffFile::getClassAttribute()
{
    return ArffFile::classAttribute;
}
