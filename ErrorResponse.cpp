///////////////////////////////////////////////////////////
//  ErrorResponse.cpp
//  Implementation of the Class ErrorResponse
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "ErrorResponse.h"
#include "logging/easylogging++.h"

std::string ErrorResponse::faultString = "Damis error";
std::string ErrorResponse::faultDetail = "";
bool ErrorResponse::fault = false;


ErrorResponse::ErrorResponse(){

}



ErrorResponse::~ErrorResponse(){

}


/**
 * Method returns fault string
 */
std::string ErrorResponse::getFaultString(){

    return ErrorResponse::faultString;
}


/**
 * Method that returns fault detail
 */
std::string ErrorResponse::getFaultDetail(){

     return ErrorResponse::faultDetail;
}


void ErrorResponse::setFaultDetail(std::string detail, bool append){

    LOG(INFO) << "Initiating set FaultDetail function, got fault detail: " << detail << ". Append " << append;

    if (!ErrorResponse::fault)
        ErrorResponse::fault = true;

    if (append == false && !detail.empty())
        ErrorResponse::faultDetail = detail;

    if (append && !detail.empty() && ErrorResponse::faultDetail.empty())
        ErrorResponse::faultDetail = detail;
    else if (append && !detail.empty() && !ErrorResponse::faultDetail.empty())
    {
        ErrorResponse::faultDetail +=" ";
        ErrorResponse::faultDetail += detail;
        ErrorResponse::faultDetail += ".";
    }
}


/**
 * Returns true/false if fault has been found
 */
bool ErrorResponse::isFaultFound(){

    return ErrorResponse::fault;
}
