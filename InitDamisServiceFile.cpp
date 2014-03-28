///////////////////////////////////////////////////////////
//  InitDamisServiceFile.cpp
//  Implementation of the Class InitDamisServiceFile
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE


#include "InitDamisServiceFile.h"
#include "logging\easylogging++.h"
#include "ErrorResponse.h"
#include "ServiceSettings.h"
#include "curl\curl.h"
#include "HelperMethods.h"


#include <iostream>




InitDamisServiceFile::~InitDamisServiceFile(){

}


/**
 * Constructor sets the file names and actually tries to download file, than checks if it is not error stream.
 */
InitDamisServiceFile::InitDamisServiceFile(std::string fURI, std::string pref) : ArffFile (pref){
InitDamisServiceFile::downloadFileURI = fURI;
InitDamisServiceFile::initialize();

}

/**
 * Constructor sets the file names
 */
InitDamisServiceFile::InitDamisServiceFile(std::string pref) : ArffFile (pref){


}

/**
 * Method that downloads data from the client
 */
bool InitDamisServiceFile::downloadFile(){

LOG(INFO) << "Initiating download file";

    if ( !DamisFile::getFileName().empty() && !InitDamisServiceFile::getDownloadFileURI().empty() )
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
            curl_easy_setopt(curl, CURLOPT_URL, InitDamisServiceFile::getDownloadFileURI().c_str());

            /* Define our callback to get called when there's data to be written */
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, InitDamisServiceFile::fileWriteDelegeate);

            /* Set a pointer to our struct to pass to the callback */
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

            /* Switch on full protocol/debug output */
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


            res = curl_easy_perform(curl);

            curl_easy_cleanup(curl);

            if(CURLE_OK != res)
            {
                LOG(ERROR) << "Error curl was not able to download file: " << InitDamisServiceFile::getDownloadFileURI();
                ErrorResponse::setFaultDetail(std::string("Error downloading file, invalid download path: ") + InitDamisServiceFile::getDownloadFileURI());
                return false;
            }
            LOG(INFO) << "Done file download from location: "<< InitDamisServiceFile::getDownloadFileURI();
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
size_t InitDamisServiceFile::fileWriteDelegeate(void *buffer, size_t size, size_t nmemb, void *stream)
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


std::string InitDamisServiceFile::getDownloadFileURI()
{
    return InitDamisServiceFile::downloadFileURI;
}


/**
 * Checks for error stream in downloaded file.
 */
bool InitDamisServiceFile::checkForHttpError()
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
            ErrorResponse::setFaultDetail(std::string("File download from ") + InitDamisServiceFile::getDownloadFileURI() + std::string(" was not successful, got HTTP error stream "));
            src.close();

            HelperMethods::deleteFile(DamisFile::getFilePath());
           /* LOG(INFO) << "Preparing to detele file: "  << DamisFile::getFilePath();



            if (std::remove(DamisFile::getFilePath().c_str()) != 0)
                LOG (WARNING) << "Error deleting file: " << DamisFile::getFilePath() <<". Another try to delete file will be after " << ServiceSettings::keepFilesInDirectory << " seconds of service invocation";
            else
                LOG(INFO) <<"File deleted: " << DamisFile::getFilePath();*/

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
bool InitDamisServiceFile::initialize(){

     if (InitDamisServiceFile::downloadFile())
        if (InitDamisServiceFile::checkForHttpError())
            if (ArffFile::readArffFile())
                return true;

	return false;
}
