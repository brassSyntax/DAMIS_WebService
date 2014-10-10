///////////////////////////////////////////////////////////
//  InitDamisServiceFile.h
//  Implementation of the Class InitDamisServiceFile
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(INITDAMISSERVICE_H)
#define INITDAMISSERVICE_H

#include "ArffFile.h"
#include <string>

/** \brief Structure that holds data for the curl file download
 *
 */
struct FtpFile {
		std::string filename; /**< File name to be saved stream under */
		FILE *stream; /**< File data stream */
	};


 /** \brief Class that represents Damis service working file
  */
class InitDamisService : public ArffFile
{

public:
    /** \brief Constructor
     *
     * \param std::string path to the file
     * \param std::string file prefix
     * \param bool validateFile = true indicates if the file data vil be validated after download
     *
     */
	InitDamisService(std::string fPath, std::string pref, bool validateFile = true);
    /** \brief Destructor
     */

	virtual ~InitDamisService();
    /** \brief Function returns file download URI
     *
     * \return std::string
     *
     */
	std::string getDownloadFileURI();

private:
    std::string downloadFileURI; /**< Download file URI */

    bool validateFile; /**< If file data must be validated */
    /** \brief Function checks if stream contains File not found HTTP server response
     * \return bool if got error from the server
     *
     */
	bool checkForHttpError();
    /** \brief Downloads file
     * \return bool if file downloaded succesfully
     *
     */
	bool downloadFile();
    /** \brief Performs file download and http error checking
     * \return bool that indicates if the download and validation were sucessfull
     *
     */
    bool initialize();

    /** \brief Callback to the stream write delegate
     *
     * \param void* Pointer to the array of elements to be written
     * \param size_t Size in bytes of each element to be written
     * \param size_t Number of elements, each one with a size of size bytes
     * \param void* Pointer to a FILE object that specifies an output stream
     * \return size_t The total number of elements successfully written
     *
     */
    static size_t fileWriteDelegeate(void *buffer, size_t size, size_t nmemb, void *stream);

};
#endif //!defined(INITDAMISSERVICE_H)
