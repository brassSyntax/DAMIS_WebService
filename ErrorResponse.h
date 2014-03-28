///////////////////////////////////////////////////////////
//  ErrorResponse.h
//  Implementation of the Class ErrorResponse
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined (ERRORRESPONSE_H)
#define ERRORRESPONSE_H

#include <string>

/**
 * Class that saves the errors and returns to the SOAP client if present
 */
class ErrorResponse
{

public:
	ErrorResponse();
	virtual ~ErrorResponse();

	static std::string getFaultString();
	static std::string getFaultDetail();
	static void setFaultDetail(std::string detail, bool append = true);
	static bool isFaultFound();

private:
	/**
	 * Fault string
	 */
	static std::string faultString;
	/**
	 * Attribute that holds fault description
	 */
	static std::string faultDetail;
	/**
	 * Indicates if the fault has been trigerred
	 */
	static bool fault;

};
#endif // (ERRORRESPONSE_H)
