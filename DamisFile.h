///////////////////////////////////////////////////////////
//  DamisFile.h
//  Implementation of the Class DamisFile
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined (DAMISFILE_H)
#define DAMISFILE_H

#include <string>

     /** \brief Class that describes each file
      *         (local, http paths and file name of the input/output file defined by the prefix that is passed by creating class object)
      *
      */
class DamisFile
{

public:
    /** \brief Destructor
     */
	virtual ~DamisFile();
     /** \brief Constructor that accepts prefix of the file name to be generated
      *
      * \param std::string preffix added to the file name
      */
	DamisFile(std::string preffix);
     /** \brief Method returns generated file name
      * \return std::string
      */
	std::string getFileName();
     /** \brief Method returns file name with the local path
      * \return std::string
      *
      */
	std::string getFilePath();
     /** \brief Method returns file name with the http path
      * \return std::string
      *
      */
	std::string getHttpPath();


private:

	std::string fileName; /**< File name  */

	std::string filePath; /**< File path */

	std::string fileHttpPath; /**< File http path */

};
#endif //!DAMISFILE_H
