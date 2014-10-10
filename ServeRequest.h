///////////////////////////////////////////////////////////
//  DamisService.h
//  Implementation of the Class DamisService
//  Created on:      05-Vas-2014 17:36:41
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(SERVEREQUEST_H)
#define SERVEREQUEST_H

#include <vector>
#include <string>
#include "InitDamisService.h"
#include "DamisFile.h"

/** \brief Class that prepares data for any calculations regarding requested operations
 *
 */
class ServeRequest
{

public:
	/** \brief Class constructor.
    *  \param InitDamisService* accepts InitDamisServiceFile pointer
    */
	ServeRequest(InitDamisService* inFile);
	/** \brief Destructor
	 *
	 */
	virtual ~ServeRequest();

	DamisFile *outFile; /**< Pointer to the results file (after algorithm run) */

	DamisFile *statFile; /**< Pointer to the calulus statistics file (after algorithm run) */

protected:
    /** \brief ethod that prepares and returns vector of vector of string of data atrributes
     *
     * \param std::vector<std::vector<double>> data section
     * \param std::vector<std::string> class attribute of the data section
     * \return std::vector<std::vector<std::string>> concatenated data section
     *
     */
	std::vector<std::vector<std::string>> prepareDataSection(std::vector<std::vector<double>> dataSection,  std::vector<std::string> classAttribute); //concats data + class
    /** \brief Method that prepares and returns vector of string that describes arff attribute section
     *
     * \param std::vector<std::string> attribute names
     * \param std::vector<std::string> attribute types
     * \param std::vector<std::string> possible classes
     * \return std::vector<std::string> prepared attribute section
     *
     */
	std::vector<std::string> prepareAttributeSection(std::vector<std::string> attributeNameList,  std::vector<std::string> attributeTypeList, std::vector<std::string> possibleClasses); //concats attribute + class
    /** \brief Method that writes the data and attribute section to output file
     *
     * \param std::string output file
     * \param std::vector<std::vector<std::string>> data section of the arff file
     * \param std::vector<std::string> attribute section
     * \return void
     *
     */
	void writeDataToFile(std::string outputFile, std::vector<std::vector<std::string>> dataSection, std::vector<std::string> atributeSection);

    InitDamisService *serveFile; /**< Pointer that holds valid input file data (attribute section and data section) */

    std::vector<std::vector<double>> writeData; /**< Variable that holds data to be written to output file */

    std::vector<double> tmpDataVector; /**< Temporary variable that holds one object data */

private:
	 /** \brief Methot that reads and converts all string data to double type (arff file data section and maps class attribute to int type)
	 * \return void
	 */
     void initialize();

};
#endif //!defined(SERVEREQUEST_H)
