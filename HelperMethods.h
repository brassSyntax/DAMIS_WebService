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


 /** \brief Static class that hold helper methods for data management
  */
class HelperMethods
{
public:
    /** \brief Constructor
     */
	HelperMethods();
    /** \brief Destructor
     */
	virtual ~HelperMethods();
     /** \brief Function that generates file name with the given preffix
      *
      * \param std::string preffix brefix to be added to the file name
      * \param  bool addExtension = true adds .arff extension
      * \return std::string with te file name
      *
      */
	static std::string generateFileName(std::string preffix, bool addExtension = true );
     /** \brief Deletes old files from the data directory
      * \return int number of deleted files
      *
      */
	static int deleteOldFiles();
     /** \brief Deletes file
      *
      * \param std::string path path to file that must be deleted
      * \return int no of deleted files
      *
      */
	static int deleteFile(std::string path);
     /** \brief Parses CGI variables
      * \return char** of the CGI vars
      *
      */
	static char **getcgivars();
     /** \brief Function splits passed string accordint to passed delimeter
      *
      * \param const std::string& s string to be splitted
      * \param char delim split delimeter
      * \return std::vector<std::string>
      *
      */

	static std::vector<std::string> split(const std::string& s, char delim);
     /** \brief Function returns random value from the given interval
      *
      * \param unsigned int upper bound
      * \param unsigned int lower bound
      * \return int random number
      *
      */
	static int getRrandomInRange (unsigned int, unsigned int);
     /** \brief Function returns mean vector value
      *
      * \param std::vector<double> vector of doubles
      * \return double mean value
      *
      */
	static double getMean(std::vector<double>);
    /** \brief Function returns deviation of vector
      *
      * \param std::vector<double> vector of doubles
      * \return double std devaition value
      *
      */
	static double getStd(std::vector<double>);
     /** \brief Function reads arff file and returns indicated attribute value (satistics file form the calculus)
      *
      * \param std::string path to file
      * \param std::string attribute name
      * \return double atribute value
      *
      */
	static double getAttributeValue(std::string statFile, std::string attName);
     /** \brief Function coverts string to double data type
      *
      * \param std::string value in string format
      * \return double value
      *
      */
	static double strToDouble(std::string cmdParam);
     /** \brief Function coverts string to int data type
      *
      * \param std::string value in string format
      * \return int value
      *
      */
    static int strToInt(std::string cmdParam);


private:
	static const char* alphanum; /**< Aphanumeric pattern for unique input and output file generation */
    /** \brief Convert a two-char hex string into the char it represents.
     *
     * \param char*  in hex to convert
     * \return char character
     *
     */
	static char x2c(char* what);
    /** \brief Reduce any %xx escape sequences to the characters they represent.
     *
     * \param char* of escape sequences
     * \return void
     *
     */
	static void unescape_url(char* url);
    /** \brief Helper method that adds temination symbol to the end of the string
     *
     * \param const char* string to be added
     * \return char* resulting string
     *
     */
	static char *strdup(const char* s);
    /** \brief Splits vector into strings
     *
     * \param const std::string string to be splitted
     * \param char split delimeter
     * \param  std::vector<std::string> accumulative array
     * \return std::vector<std::string>
     *
     */
	static std::vector<std::string> split(const std::string& s, char delim, std::vector<std::string> &elems);

};
#endif //!defined(EHELPERMETHODS_H)
