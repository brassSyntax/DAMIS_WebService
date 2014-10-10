///////////////////////////////////////////////////////////
//  InitDamisServiceFile.cpp
//  Implementation of the Class InitDamisServiceFile
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE


#include "InitDamisService.h"
#include "logging/easylogging++.h"
#include "ErrorResponse.h"
#include "ServiceSettings.h"
#include "curl/curl.h"
#include "HelperMethods.h"


#include <iostream>




InitDamisService::~InitDamisService(){

}


/**
 * Constructor sets the file names and actually tries to download file, than checks if it is not error stream.
 */
InitDamisService::InitDamisService(std::string fURI, std::string pref, bool validateFile) : ArffFile (pref){
InitDamisService::downloadFileURI = fURI;
InitDamisService::validateFile = validateFile;
InitDamisService::initialize();

}

/**
 * Constructor sets the file names
 */
//InitDamisService::InitDamisService(std::string pref) : ArffFile (pref){


//}

/**
 * Method that downloads data from the client
 */
bool InitDamisService::downloadFile(){

LOG(INFO) << "Initiating download file";

    if ( !DamisFile::getFileName().empty() && !InitDamisService::getDownloadFileURI().empty() )
    {
        CURL *curl;
        CURLcode res;

        LOG(INFO) << "Initializing file structure for download at location: "<< DamisFile::getFilePath();
        struct FtpFile ftpfile =
        {
            DamisFile::getFilePath(), /* name to store the file as if successful */
            NULL
        };

        curl_global_init(CURL_GLOBAL_DEFAULT);

        LOG(INFO) << "Initializing curl object";
        curl = curl_easy_init();

        if (!curl)
        {
            LOG(ERROR) << "Error initializing curl";
           // ErrorResponse::setFaultDetail("Aaa");
            ErrorResponse::setFaultDetail("Error initializing file download");
            //printf ("Error initializing Curl");
            return false;
        }
        if(curl)
        {
            LOG(INFO) << "Setting up curl for file download";
            /*
             * You better replace the URL with one that works!
             */
            curl_easy_setopt(curl, CURLOPT_URL, InitDamisService::getDownloadFileURI().c_str());

            /* Define our callback to get called when there's data to be written */
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, InitDamisService::fileWriteDelegeate);

            /* Set a pointer to our struct to pass to the callback */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

            /* Switch on full protocol/debug output */
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);


            res = curl_easy_perform(curl);

            curl_easy_cleanup(curl);

            if(CURLE_OK != res)
            {
                LOG(ERROR) << "Error curl was not able to download file: " << InitDamisService::getDownloadFileURI();
                ErrorResponse::setFaultDetail(std::string("Error downloading file, invalid download path: ") + InitDamisService::getDownloadFileURI());
                return false;
            }
            LOG(INFO) << "Done file download from location: "<< InitDamisService::getDownloadFileURI();
        }
        if(ftpfile.stream)
            fclose(ftpfile.stream); /* close the local file */
        curl_global_cleanup();
    }

	return true;
}

/**
 * Delegate function that accepts and stores stream from the client
 */
size_t InitDamisService::fileWriteDelegeate(void *buffer, size_t size, size_t nmemb, void *stream)
{
    struct FtpFile *out = (struct FtpFile *)stream;
    if(out && !out->stream)
    {
        out->stream = fopen(out->filename.c_str(), "wb");
        if(!out->stream)
        {
            LOG(ERROR) << "Curl callback can't open local file" << out->filename.c_str() << "to write";
            ErrorResponse::setFaultDetail("File download callback failed to stream content of the file to local service file");
            return -1; /* failure, can't open file to write */
        }
    }
    return fwrite(buffer, size, nmemb, out->stream);
}


std::string InitDamisService::getDownloadFileURI()
{
    return InitDamisService::downloadFileURI;
}


/**
 * Checks for error stream in downloaded file.
 */
bool InitDamisService::checkForHttpError()
{

    LOG (INFO) << "Checking if downloaded file " << DamisFile::getFilePath()<< " is not error stream";
    std::ifstream src(DamisFile::getFilePath(), std::ios::binary);
    std::string httpErr1 = "404.0 - Not Found";
    std::string line;

    bool found = false;

    while(std::getline(src,line))
    {
        if(line.find(httpErr1,0)!=std::string::npos) // string::npos is returned if string is not found
        {
            found=true;
            LOG (ERROR) << "HTTP error stream found in downloaded file: " << DamisFile::getFilePath();
            ErrorResponse::setFaultDetail(std::string("File download from ") + InitDamisService::getDownloadFileURI() + std::string(" was not successful, got HTTP error stream "));
            src.close();
            HelperMethods::deleteFile(DamisFile::getFilePath());
            break;
        }
    }
if (!found)
        LOG (INFO) << "Downloaded file " << DamisFile::getFilePath()<< " is OK";
    return !found; //returns true if error was found
}

/**
 * Performs file download and http error checking
 */
bool InitDamisService::initialize(){

     if (InitDamisService::downloadFile())
        if (InitDamisService::checkForHttpError())
            if (InitDamisService::validateFile)
                return ArffFile::readArffFile();
                else
                {
                    ArffFile::readArffFile();
                    return true;
                }


	return false;
}
