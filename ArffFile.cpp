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
#include "logging/easylogging++.h"

#include <string>

/**
 * Class contructor. Initializes structures for data section, attribute names, attribute types, class atrribute, object attribute, no of atributes of the arff file
 */
ArffFile::ArffFile(std::string pref) : DamisFile(pref) //call super class constructor and pass prefix
{
    attributeName.reserve(0);
    attributeType.reserve(0);

    stringData.reserve(0);
    doubleData.reserve(0);

    objIntClass.reserve(0);
    objStringClass.reserve(0);
   // objClassAttribute.reserve(0);

    noOfAtt = 0;
    noOfObj = 0;
    classFound = false;

}

ArffFile::~ArffFile()
{

}
/**
 * Returns vector of attribute names
 */
std::vector<std::string> ArffFile::getAttributeName()
{

    return  ArffFile::attributeName;
}
/**
 * Method that returns attribute types
 */
std::vector<std::string> ArffFile::getAttributeType()
{

    return  ArffFile::attributeType;
}
/**
 * Returns objects class list <string> (data section)
 */
std::vector<std::string> ArffFile::getStringClass()
{

    return  ArffFile::objStringClass;
}
/**
 * Returns objects class list <int> (data section)
 */
std::vector<int> ArffFile::getIntClass()
{

    return  ArffFile::objIntClass;
}
/**
 * Returns raw data in string format
 */
std::vector<std::vector<std::string>> ArffFile::getStringData()
{
    return  stringData;
}

/**
 * Returns raw data in string format
 */
std::vector<std::vector<double>> ArffFile::getDoubleData()
{
    return  doubleData;
}

/**
 * Returns number of attributes that must be present in each data section line
 */
int ArffFile::getNumberOfAttributes()
{

    return  noOfAtt;
}
/**
 * Returns number of objects
 */
int ArffFile::getNumberOfObjects()
{
    return  noOfObj;
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
    std::vector<std::string> stringVector; stringVector.reserve(0);
    std::vector<double> doubleVector; doubleVector.reserve(0);

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
                        ArffFile::classAttributeIndex = ArffFile::attributeName.size();
                        LOG(INFO) << "Found CLASS attribute at index " << ArffFile::classAttributeIndex;

                        int startClassList = line_from_file.find("{");
                        int endClassList = line_from_file.find("}");

                        if (startClassList != std::string::npos && endClassList != std::string::npos) //if both brackets found
                        {
                            tmp3.assign(line_from_file, startClassList + 1, endClassList - startClassList - 1);

                            LOG(INFO) << "Classes are " << tmp3;
                            tmp4 = HelperMethods::split(tmp3, ','); //split resulting string
                            std::string className;
                            for (unsigned int i = 0; i < tmp4.size(); i++) //push back each class label
                            {
                                //trim spaces from start
                                tmp4[i].erase(tmp4[i].begin(), std::find_if(tmp4[i].begin(), tmp4[i].end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
                                //trim spaces from end
                                tmp4[i].erase(std::find_if(tmp4[i].rbegin(), tmp4[i].rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), tmp4[i].end());

                                className.assign(tmp4[i]);
                                std::transform(className.begin(), className.end(), className.begin(), ::toupper);

                                ArffFile::attributeStringClass.push_back(className);
                                ArffFile::attributeIntClass.push_back(i);
                            }
                        }
                        else
                        {
                            ErrorResponse::setFaultDetail(std::string("Invalid class definition in attribute list, cannot find pair of brackets {} at line ") + std::to_string(static_cast<long long>(line_no)) );
                            LOG(ERROR) << "Class atribute is ill-defined, cannot find {} brackets";
                        }
                    }
                    else if (tmp2 == "REAL"  || tmp2 == "INTEGER" || tmp2 == "NUMERIC")
                    {
                        ArffFile::noOfAtt++;
                        ArffFile::attributeName.push_back(tmp1);
                        ArffFile::attributeType.push_back(tmp2);
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

                    std::string::size_type pos = 0;

                    while ((pos = tmp[noOfAttr-1].find("\r", pos)) != std::string::npos)
                    {
                        tmp[noOfAttr-1].replace(pos, 2, "\0");
                    }

                    if (tmp.size() == noOfAttr)
                    {
                        bool badClass;

                        if (isClassFound())
                                badClass = true;
                             else
                                badClass = false;

                        bool badAtrrSection = false;

                        std::string className;
                        int classInt;

                        for (unsigned int i = 0; i < tmp.size(); i++) // for each attribute in each data line
                        {
                            if (ArffFile::isClassFound() && (i == ArffFile::getClassAttributeIndex()))
                            {
                                //check if the class attribute in data section is valid
                                                                 //trim spaces from start
                                tmp[i].erase(tmp[i].begin(), std::find_if(tmp[i].begin(), tmp[i].end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
                                //trim spaces from end
                                tmp[i].erase(std::find_if(tmp[i].rbegin(), tmp[i].rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), tmp[i].end());

                                className.assign(tmp[i]);
                                std::transform(className.begin(), className.end(), className.begin(), ::toupper);

                                ///class may have unknown values i.e. ? so skip it
                                if (className !="?")
                                {
                                    for (int cA = 0; cA < ArffFile::attributeStringClass.size(); cA++)
                                    {
                                        if (ArffFile::attributeStringClass.at(cA) == className)
                                        {
                                            badClass = false;
                                            classInt = ArffFile::getIntClassAttribute().at(cA);
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    badClass = false;
                                }

                                if (badClass)
                                {
                                    ErrorResponse::setFaultDetail(std::string("Invalid class definition of the object at line: ") + std::to_string(static_cast<long long>(line_no)) );
                                    LOG(ERROR) << "Found ivalid class at line - " << line_no;
                                    //continue;
                                }
                            }
                            else
                            {
                                //try to convert to double
                                char *err;
                                double x = strtod(tmp[i].c_str(), &err);

                                if (*err == 0 && tmp[i] !="")
                                {
                                    stringVector.push_back(tmp[i]); //save data attribute
                                    doubleVector.push_back(x);
                                }
                                else
                                {
                                    ErrorResponse::setFaultDetail("File is not valid, found non numeric value \\ " + tmp[i] + " \\ at line " + std::to_string(static_cast<long long>(line_no)) + std::string(" at position ") + std::to_string (static_cast<long long>(i)) );
                                    LOG(ERROR) << "File is not valid (skipping object), found non numeric value \\ " << tmp[i] << " \\ at line " <<line_no <<" at position " <<i;
                                    badAtrrSection = true;

                                }
                            }
                        } //end for tmp.size();

                        if (badAtrrSection == false  && badClass == false)
                        {
                            if (isClassFound())
                            {
                                ArffFile::objStringClass.push_back(className); //save class attribute
                                if (className != "?")
                                    ArffFile::objIntClass.push_back(ArffFile::getIntClassAttribute().at(classInt));
                                else
                                    ArffFile::objIntClass.push_back(-1);
                            }

                            ArffFile::stringData.push_back(stringVector); //add to matrix
                            ArffFile::doubleData.push_back(doubleVector); //add to matrix

                            noOfObj++;
                        }

                        stringVector.clear(); //clear vectors
                        doubleVector.clear();
                    }
                    else
                    {
                        ErrorResponse::setFaultDetail(std::string("Number of features ") + std::to_string(static_cast<long long>(tmp.size())) + std::string(" at line ") + std::to_string(static_cast<long long>(line_no)) + std::string(" in @Data section of arff file does not correspond to number of attributes ") + std::to_string( static_cast<long long>(ArffFile::getNumberOfAttributes())) + std::string(" of the @Attribute section "));
                        LOG(ERROR) << "Data section line " << line_no << " does not have required quantity features ";
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

    if(ErrorResponse::isFaultFound())
    {
        file.close();
        //ErrorResponse::setFaultDetail("Data file does not contain either attribute either correct data section");
        //LOG(ERROR) << "Data file " << DamisFile::getFilePath() << " does not contain either attribute or correct data section";
        HelperMethods::deleteFile(DamisFile::getFilePath());
        return false;
    }
    else if (ArffFile::stringData.empty() || ArffFile::attributeName.empty())
    {
        file.close();
        ErrorResponse::setFaultDetail("Data file does not contain either attribute either correct data section");
        LOG(ERROR) << "Data file " << DamisFile::getFilePath() << " does not contain either attribute or correct data section ";
        HelperMethods::deleteFile(DamisFile::getFilePath());
        return false;
    }
    else
    {
        LOG(INFO) << "Reading file " << DamisFile::getFilePath() << " into string format was OK";
        return true;
    }
}

double ArffFile::getDoubleDataAt(int i, int j)
{
    return doubleData[i][j];
}
/**
 * Returns class attribute index in arff file
 */
int ArffFile::getClassAttributeIndex()
{
    return ArffFile::classAttributeIndex;
}
/**
 * Returns indicates is class attribute found
 */
bool ArffFile::isClassFound()
{
  return ArffFile::classFound;
}
/**
 * Function returns vector of strings of class attributes (attribute section)
 */
std::vector<std::string> ArffFile::getStringClassAttribute()
{
    return ArffFile::attributeStringClass;
}
/**
 * Function returns vector of ints of class attributes (attribute section)
 */
std::vector<int> ArffFile::getIntClassAttribute()
{
    return ArffFile::attributeIntClass;
}
