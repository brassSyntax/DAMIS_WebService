///////////////////////////////////////////////////////////
//  ArffFile.h
//  Implementation of the Class ArffFile
//  Created on:      30-Sau-2014 17:06:17
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined (ARFFFILE_H)
#define ARFFFILE_H

#include "DamisFile.h"
#include <vector>
#include <string>

/**
 * Class represents arrf file that is inherited from DamisFile
 */
class ArffFile : public DamisFile //
{

public:
	 /**
	 * Function returns vector of attribute names (from the attribute section class excluded)
	 */
	ArffFile(std::string pref);

	virtual ~ArffFile();
	 /**
	 * Function returns vector of attribute names (from the attribute section class excluded)
	 */
	std::vector<std::string> getAttributeNames();
	 /**
	 * Function returns vector of attribute types (from the attribute section class excluded)
	 */
	std::vector<std::string> getAttributeTypes();
	 /**
	 * Function returns vector of possible classes (from the attribute section)
	 */
	std::vector<std::string> getStringClassNames();
	 /**
	 * Function returns vector of possible classes (from the attribute section)
	 */
	std::vector<std::string> getClasses();
	 /**
	 * Function returns vector of object's class values
	 */
	std::vector<std::string> getClassAttribute();
     /**
	 * Function returns data in data section (string format without class)
	 */
	std::vector<std::vector<std::string>> getRawDataStringFormat();
     /**
	 * Function returns class attribute index
	 */
	 int getClassAttributeIndex();
	 /**
	 * Function returns number of ttributes (without class)
	 */
	int getNumberOfAttributes();
	 /**
	 * Function indicates if the class section is found in arff file
	 */
	bool isClassFound();

protected:
    /**
	 * Function reads and validates arff file (all the data are stored in string format)
	 */
	bool readArffFile();

private:
	/**
	 * Attribute that holds attribute names of the arff file
	 */
	std::vector<std::string> attributeNames;
	/**
	 * Attribute that holds attribute types
	 */
	std::vector<std::string> attributeTypes;
	/**
	 * Attribute that has class labels
	 */
	std::vector<std::string> objectClasses;
	/**
	 * Attribute that holds each object class
	 */
	std::vector<std::string> classAttribute;
	 /**
	 * Is class attribute found
	 */
    bool classFound;
	/**
	 * Attribute that holds class index
	 */
	int attributeClassIndex;
    /**
	 * Number of features in data section of the arff file
	 */
    int numberOfAttributes;
	/**
	 * Attribute that holds raw data (without class) in double format
	 */
	std::vector<std::vector<std::string>> rawStringData;

};
#endif //!defined (ARFFFILE_H)
