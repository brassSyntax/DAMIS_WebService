///////////////////////////////////////////////////////////
//  HelperMethods.cpp
//  Implementation of the Class HelperMethods
//  Created on:      30-Sau-2014 17:06:18
//  Original author: Povilas
///////////////////////////////////////////////////////////

#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "HelperMethods.h"
#include "ServiceSettings.h"
#include "dirent\dirent.h"
#include "logging\easylogging++.h"

#include <time.h>
#include <string>

const char* HelperMethods::alphanum= "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";


HelperMethods::HelperMethods(){

}

HelperMethods::~HelperMethods(){

}

double HelperMethods::getAttributeValue(std::string statFile, std::string attName)
{
double retVal;
std::string attribute = attName;
std::vector<std::string> tmp;
tmp.reserve(0);
std::string line_from_file, tmp1;

std::transform(attribute.begin(), attribute.end(), attribute.begin(), ::toupper);

int tmpIndex = -1, attIndex = 0;

std::ifstream file (statFile);

if (file.is_open() != false)
    {
        while (std::getline(file, line_from_file))
        {
            if (line_from_file.length() == 0)
            {
                continue;
            }
            std::istringstream iss(line_from_file);
            std::string sub;
            iss >> sub;
            if (sub == "%")
            {
                continue;
            }
            else
            {
                std::transform(sub.begin(), sub.end(), sub.begin(), ::toupper);
                if (sub == "@ATTRIBUTE")
                {
                    tmpIndex ++;
                    iss >> tmp1;
                    //iss >> tmp2;

                    std::transform(tmp1.begin(), tmp1.end(), tmp1.begin(), ::toupper);
                   // std::transform(tmp2.begin(), tmp2.end(), tmp2.begin(), ::toupper);
                    if (tmp1 == attribute)
                    {
                        attIndex = tmpIndex;
                    }
                }
                else if (sub == "@DATA" || sub == "@RELATION")
                {
                    continue;
                }
                else
                {
                    tmp = HelperMethods::split(line_from_file, ','); //split data section by comma
                    const char *str = tmp.at(attIndex).c_str();
                    char *err;
                    double x = strtod(str, &err);
                    if (*err == 0 && tmp.at(attIndex) !="")
                    {
                      retVal = atof(tmp.at(attIndex).c_str());
                    }
                }
            }
        }
      //  file.close();
    }
    file.close();
return retVal;
}


/**
 * Method generates random file name and adds to the prefix
 */
std::string HelperMethods::generateFileName(std::string preffix)
{

    LOG(INFO) << "Starting file name generation with the preffix: " << preffix;
    time_t t;
    time(&t);
    srand((unsigned int)(t+rand()));
    std::string catString;
    catString.assign(preffix);

    int i;
    int qty = catString.length() + ServiceSettings::lenghtOfTmpFileName;
    for ( i = catString.length(); i < qty; ++i)
        catString += (HelperMethods::alphanum[rand() % (strlen(HelperMethods::alphanum) - 1)]);
    catString += ServiceSettings::dataFileExtension;
    if (catString.empty())
    {
        LOG(ERROR) << "Unable to generate file name";
        return NULL;
    }

    LOG(INFO) << "Generated file name: " << catString;

    return  catString;
}


/**
 * Method that checks and deletes old files from specified directory (provided in ServiceSettings file)
 */
int HelperMethods::deleteOldFiles()
{
    LOG(INFO) << "Initiating old file delete check routine";
    DIR *dir;
    struct dirent *ent;
    int fileCount = 0;
    time_t nowTime = time(NULL);                     // get local time

    struct stat attrib;                 // create a file attribute structure
    std::string fPath;                  // tmp construct path to all files

    LOG(INFO) << "Opening file storage directory: " << ServiceSettings::localDataFileSavePath.c_str();

    if ((dir = opendir (ServiceSettings::localDataFileSavePath.c_str())) != NULL)
    {
        /* get all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            fPath = ServiceSettings::localDataFileSavePath + ent->d_name;
            stat(fPath.c_str(), &attrib);         // get the attributes of a file

            //if it is not directory and time has ellapsed DELETE FILE
            if ((attrib.st_mode & S_IFMT) != S_IFDIR && difftime(nowTime, attrib.st_mtime) > ServiceSettings::keepFilesInDirectory)
            {
                fileCount++;
                LOG(INFO) << "Preparing to detele file - " << fPath.c_str();
                if (std::remove(fPath.c_str()) != 0)
                    LOG(WARNING) << "Error deleting file - "<< fPath.c_str();
                else
                    LOG(INFO) << "File " << fPath.c_str() << " was deleted";
            }
        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        LOG(WARNING) << "Unable to open file storage directory " << ServiceSettings::localDataFileSavePath;
    }

    LOG(INFO) << fileCount <<" deleted files in total";
    return fileCount;
}

/**
 * Method that deletes single file passed as a parameter
 */
int HelperMethods::deleteFile(std::string path)
{

    LOG(INFO) << "Preparing to detele file: "  << path;

    if (std::remove(path.c_str()) != 0)
        LOG (WARNING) << "Error deleting file: " << path <<". Another try to delete file will be after " << ServiceSettings::keepFilesInDirectory << " seconds of service invocation";
    else
        LOG(INFO) <<"File deleted: " << path;

return 0;
}

/**
 * Convert a two-char hex string into the char it represents.
 */
char HelperMethods::x2c(char* what)
{
    register char digit;

    digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
    digit *= 16;
    digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
    return(digit);
}

/**
 * Reduce any %xx escape sequences to the characters they represent.
 */
void HelperMethods::unescape_url(char* url){
register int i,j;

    for(i=0,j=0; url[j]; ++i,++j)
    {
        if((url[i] = url[j]) == '%')
        {
            url[i] = HelperMethods::x2c(&url[j+1]) ;
            j+= 2 ;
        }
    }
    url[i] = '\0' ;
}

/**
 * Helper method that adds temination symbol to the end of the string
 */
char *HelperMethods::strdup(const char* s){

    char *d = (char *) malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}


/**
 * Read the CGI input and place all name/val pairs into list.
 * Returns list containing name1, value1, name2, value2, ... , NULL
 */
char **HelperMethods::getcgivars(){

    register int i ;
    char *request_method = NULL ;
    int content_length;
    char *cgiinput = NULL ;
    char **cgivars ;
    char **pairlist ;
    int paircount ;
    char *nvpair ;
    char *eqpos ;

    /** Depending on the request method, read all CGI input into cgiinput. **/
    request_method= getenv("REQUEST_METHOD");
    if (request_method == NULL)
        return NULL;
    /*char *qs ;
    qs= getenv("QUERY_STRING") ;*/

    if (!strcmp(request_method, "GET") || !strcmp(request_method, "HEAD") )
    {
        /* Some servers apparently don't provide QUERY_STRING if it's empty, */
        /*   so avoid strdup()'ing a NULL pointer here.                      */
        char *qs ;
        qs= getenv("QUERY_STRING") ;
        cgiinput= HelperMethods::strdup(qs  ? qs  : "") ;
    }
    else if (!strcmp(request_method, "POST"))
    {
        /* strcasecmp() is not supported in Windows-- use strcmpi() instead */
        if ( strcmp(getenv("CONTENT_TYPE"), "application/x-www-form-urlencoded"))
        {
            printf("Content-Type: text/plain\n\n") ;
            printf("getcgivars(): Unsupported Content-Type.\n") ;
            //    exit(1) ;
            return NULL;
        }
        if ( !(content_length = atoi(getenv("CONTENT_LENGTH"))) )
        {
            printf("Content-Type: text/plain\n\n") ;
            printf("getcgivars(): No Content-Length was sent with the POST request.\n") ;
            // exit(1) ;
            return NULL;
        }
        if ( !(cgiinput= (char *) malloc(content_length+1)) )
        {
            printf("Content-Type: text/plain\n\n") ;
            printf("getcgivars(): Couldn't malloc for cgiinput.\n") ;
            //  exit(1) ;
            return NULL;
        }
        if (!fread(cgiinput, content_length, 1, stdin))
        {
            printf("Content-Type: text/plain\n\n") ;
            printf("getcgivars(): Couldn't read CGI input from STDIN.\n") ;
            // exit(1) ;
            return NULL;
        }
        cgiinput[content_length]='\0' ;
    }
    else
    {
        printf("Content-Type: text/plain\n\n") ;
        printf("getcgivars(): Unsupported REQUEST_METHOD.\n") ;
        exit(1) ;
    }

    /** Change all plusses back to spaces. **/
    for (i=0; cgiinput[i]; i++) if (cgiinput[i] == '+') cgiinput[i] = ' ' ;

    /** First, split on "&" and ";" to extract the name-value pairs into **/
    /**   pairlist.                                                      **/
    pairlist= (char **) malloc(256*sizeof(char **)) ;
    paircount= 0 ;
    nvpair= strtok(cgiinput, "&;") ;
    while (nvpair)
    {
        pairlist[paircount++]= strdup(nvpair) ;
        if (!(paircount%256))
            pairlist= (char **) realloc(pairlist,(paircount+256)*sizeof(char **)) ;
        nvpair= strtok(NULL, "&;") ;
    }
    pairlist[paircount]= 0 ;    /* terminate the list with NULL */

    /** Then, from the list of pairs, extract the names and values. **/
    cgivars= (char **) malloc((paircount*2+1)*sizeof(char **)) ;
    for (i= 0; i<paircount; i++)
    {
        if (eqpos=strchr(pairlist[i], '='))
        {
            *eqpos= '\0' ;
            HelperMethods::unescape_url(cgivars[i*2+1]= strdup(eqpos+1)) ;
        }
        else
        {
            HelperMethods::unescape_url(cgivars[i*2+1]= strdup("")) ;
        }
        HelperMethods::unescape_url(cgivars[i*2]= strdup(pairlist[i])) ;
    }
    cgivars[paircount*2]= 0 ;   /* terminate the list with NULL */

    /** Free anything that needs to be freed. **/
    free(cgiinput) ;
    for (i=0; pairlist[i]; i++) free(pairlist[i]) ;
    free(pairlist) ;

    /** Return the list of name-value strings. **/
    return cgivars ;
}

std::vector<std::string> HelperMethods::split(const std::string& s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> HelperMethods::split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    HelperMethods::split(s, delim, elems);
    return elems;
}


int HelperMethods::getRrandomInRange(unsigned int min, unsigned int max)
{
  int x, randMax = RAND_MAX - RAND_MAX % max;

int p=RAND_MAX+1;
while(p>randMax)
        p=rand();
x= min + p%(max - min);
return x;
}

double HelperMethods::getMean(std::vector<double> dataVector)
{
    return std::accumulate(dataVector.begin(), dataVector.end(), 0.0) / dataVector.size();
}

double HelperMethods::getStd(std::vector<double> dataVector)
{
    std::vector<double> diff(dataVector.size());
    std::transform(dataVector.begin(), dataVector.end(), diff.begin(), std::bind2nd(std::minus<double>(), HelperMethods::getMean(dataVector)));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    return std::sqrt(sq_sum / dataVector.size()); //daliname is N o ne is N-1
}
