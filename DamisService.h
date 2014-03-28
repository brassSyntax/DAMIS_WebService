///////////////////////////////////////////////////////////
//  DamisService.h
//  Implementation of the Class DamisService
//  Created on:      05-Vas-2014 17:36:41
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(DAMISSERVICE_H)
#define DAMISSERVICE_H

#include <vector>
#include <string>
#include "InitDamisServiceFile.h"
#include "DamisFile.h"

/**
 * Class that prepares data for any calculations regarding requested operations
 */
class DamisService
{

public:
	/**
    * Class constructor. Accepts InitDamisServiceFile variable
    */
	DamisService(InitDamisServiceFile* inFile);
	/**
	 * Empty destructor
	 */
	virtual ~DamisService();
	/**
    * Attribute that holds the structure of each output file
    */
	DamisFile *outFile;
    /**
    * Attribute that holds the statistics data
    */
	DamisFile *statFile;

protected:
    /**
    * Method that returns vector of vector of double data that are present in arff file data section
    */
	std::vector<std::vector<double>> getDataDoubleFormat();
	/**
    * Method that returns vector of int class atrributes
    */
	std::vector<int> getIntClassAttribute();
	/**
    * Method that returns vector of string class atrributes
    */
	std::vector<std::string> getStringClassAttribute();
	/**
    * Method that prepares and returns vector of vector of string data atrributes
    */
	//std::vector<std::vector<std::string>> prepareDataSection(std::vector<std::vector<double>> dataSection,  std::vector<int> classAttribute); //concats data + class
	/**
    * Method that prepares and returns vector of vector of string data atrributes
    */
	//std::vector<std::vector<std::string>> prepareDataSection(std::vector<std::vector<std::string>> dataSection,  std::vector<std::string> classAttribute); //concats data + class
	/**
    * Method that prepares and returns vector of vector of string data atrributes
    */
	std::vector<std::vector<std::string>> prepareDataSection(std::vector<std::vector<double>> dataSection,  std::vector<std::string> classAttribute); //concats data + class
    /**
    * Method that prepares and returns vector of vector of string that describes arff file data section
    */
    //std::vector<std::vector<std::string>> prepareDataSection(std::vector<std::vector<std::string>> dataSection,  std::vector<int> classAttribute); //concats data + class
	/**
    * Method that prepares and returns vector of string that describes arff attribute section
    */
	std::vector<std::string> prepareAttributeSection(std::vector<std::string> attributeNameList,  std::vector<std::string> attributeTypeList, std::vector<std::string> possibleClasses); //concats attribute + class
	/**
    * Method that prepares and returns vector of string that describes arff attribute section
    */
	//std::vector<std::string> prepareAttributeSection(std::vector<std::string> attributeNameList,  std::vector<std::string> attributeTypeList, std::vector<int> possibleClasses); //concats attribute + class
	/**
    * Method that writes the data and attribute section to output file
    */
	void writeDataToFile(std::string outputFile, std::vector<std::vector<std::string>> dataSection, std::vector<std::string> atributeSection);
	/**
    * Attribute that holds valid input file data (attribute section and data section)
    */
    InitDamisServiceFile *serveFile;
        /**
	 * Variable that holds data to be written to output file
	 */
    std::vector<std::vector<double>> writeData;
    /**
	 * temporary variable that holds one object data
	 */
    std::vector<double> tmpDataVector;
    /**
	 * Variable that holds data section rows count
	 */
    int noOfDataRows;
    /**
	 * Variable that holds data section column count
	 */
    int noOfDataAttr;

private:

    /**
	 * Attribute represents raw string data (without class) attribute
	 */
	std::vector<std::vector<double>> rawDoubleData;
	/**
	 * Attribute that holds each object class in int representation
	 */
	std::vector<int> intClassAttributes;
		/**
	 * Attribute that holds each object class in string representation (as is in arff data file)
	 */
	std::vector<std::string> stringClassAttributes;
	/**
	 * Attribute that holds alternative file http path
	 */
	 //std::string altHttpPath;
	 /**
	 * Attribute that holds alternative file local path
	 */
	 //std::string altLocalPath;
	 /**
	 * Methot that reads and converts all string data to double type (arff file data section and maps class attribute to int type)
	 */
     void initialize();


};
#endif // !defined(DAMISSERVICE_H)
