///////////////////////////////////////////////////////////
//  HelperMethods.h
//  Implementation of the Class HelperMethods
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(HELPERMETHODS_H)
#define HELPERMETHODS_H

#include <string>
#include <vector>

/**
 * Static class that hold helper methods for data management
 */
class HelperMethods
{

public:
	HelperMethods();
	virtual ~HelperMethods();

	/**
	 * Function that generates file name with the given preffix
	 */
	static std::string generateFileName(std::string preffix);
	/**
	 * Function that deletes all files according to setting in ServiceSettings class
	 */
	static int deleteOldFiles();
	/**
	 * Function deletes passed file
	 */
	static int deleteFile(std::string path);
	/**
	 * Function returns pointer of pointer of CGI vars
	 */
	static char **getcgivars();
	/**
	 * Function splits passed string accordint to passed delimeter
	 */
	static std::vector<std::string> split(const std::string& s, char delim);
	/**
	 * Function returns random value from the given interval
	 */
	static int getRrandomInRange (unsigned int, unsigned int);
	/**
	 * Function returns mean vector value
	 */
	static double getMean(std::vector<double>);
	/**
	 * Function returns deviation of vector
	 */
	static double getStd(std::vector<double>);
	/**
	 * Function reads arff file and returns indicated attribute value
	 */
	static double getAttributeValue(std::string statFile, std::string attName);
	//static double getAlgorithmError(std::string statFile);


private:
	/**
	 * Aphanumeric pattern for unique input and output file generation
	 */
	static const char* alphanum;

	static char x2c(char* what);
	static void unescape_url(char* url);
	static char *strdup(const char* s);
	static std::vector<std::string> split(const std::string& s, char delim, std::vector<std::string> &elems);

};
#endif // !defined(EHELPERMETHODS_H)
