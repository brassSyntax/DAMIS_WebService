///////////////////////////////////////////////////////////
//  ErrorResponse.h
//  Implementation of the Class ErrorResponse
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined (ERRORRESPONSE_H)
#define ERRORRESPONSE_H

#include <string>

 /** \brief Class that saves the errors and returns to the SOAP client if present
  *
  */
class ErrorResponse
{

public:
    /** \brief Constructor
     *
     */
	ErrorResponse();
    /** \brief Destructor
     *
     */
	virtual ~ErrorResponse();
    /** \brief Returns fault string
     *
     * \return std::string
     */
	static std::string getFaultString();
    /** \brief Returns fault details
     * \return std::string
     *
     */
	static std::string getFaultDetail();
    /** \brief Sets fault details
     *
     * \param std::string detail fault description
     * \param bool append = true if append faults
     * \return void
     *
     */
	static void setFaultDetail(std::string detail, bool append = true);
    /** \brief Indicates if there were faults
     * \return bool
     *
     */
	static bool isFaultFound();

private:

	static std::string faultString; /**< Fault string */

	static std::string faultDetail; /**< Attribute that holds fault description */

	static bool fault; /**< Indicates if the fault has been trigerred */

};
#endif //!(ERRORRESPONSE_H)
