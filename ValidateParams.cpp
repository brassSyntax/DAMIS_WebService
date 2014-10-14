///////////////////////////////////////////////////////////
//  ValidateParams.cpp
//  Implementation of the Class ValidateParams
//  Created on:      03-Vas-2014 21:44:17
//  Original author: Povilas
///////////////////////////////////////////////////////////
#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "ValidateParams.h"
#include "ErrorResponse.h"
#include "logging/easylogging++.h"
#include "HelperMethods.h"
#include "ServiceSettings.h"
#include <vector>
#include <iostream>


/**
 * Constructor.
 */



ValidateParams::ValidateParams(InitDamisService* inFile)
{
    this->inFile = inFile;
    //ValidateParams::fileIsValid = true;
    ValidateParams::methodParamsValid = true;
}

/**
 * Empty destructor
 */
ValidateParams::~ValidateParams()
{

}

	//dimmension reduction
/**
 * Function that checks if pca parameters are OK
 */
void ValidateParams::pca(bool projType, double d, int maxCalcTime)
{
    LOG(INFO) << "Validating PCA parameter projType - " << projType << " d - "<< d <<  " maxCalcTime - " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

    if (projType)
         ValidateParams::validateInterval(d, 1, ValidateParams::inFile->getNumberOfAttributes(), "d");
    else
        ValidateParams::validateInterval(d, 0, 100, "d", false, true);
}
/**
 * Function that checks if smacofMds parameters are OK
 */
void ValidateParams::smacofMds(int d, int maxIteration, double eps, bool zeidel, int p, int maxCalcTime)
{
    LOG(INFO) << "Validating SMACOFMDS parameter d - " << d << " maxIteration - "<< maxIteration << " eps - " << eps<< " zeidel - " << zeidel <<" p - " << p << " maxCalcTime - " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

    ValidateParams::validateInterval(d, 1, ValidateParams::inFile->getNumberOfAttributes(), "d");

    ValidateParams::validateInterval(maxIteration, 1, 10000, "maxIteration");

    ValidateParams::validateGreatherThan(eps, 0, "eps", true);

    ValidateParams::validateInterval(p, 1, ServiceSettings::noOfProcessors, "p");
}
/**
 * Function that checks if dma parameters are OK
 */
void ValidateParams::dma(int d, int maxIteration, double eps, double neighbour, int maxCalcTime)
{
    LOG(INFO) << "Validating DMA parameter d - " << d << " maxIteration - "<< maxIteration << " eps - " << eps<< " neighbour - " << neighbour << " maxCalcTime - " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

    ValidateParams::validateInterval(d, 1, ValidateParams::inFile->getNumberOfAttributes(), "d");

    ValidateParams::validateInterval(maxIteration, 1, 10000, "maxIteration");

    ValidateParams::validateGreatherThan(eps, 0, "eps", true);

    ValidateParams::validateInterval(neighbour, 0, 100, "neighbour", false, true);
}
/**
 * Function that checks if relMds parameters are OK
 */
void ValidateParams::relMds(int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy, int maxCalcTime)
{
    LOG(INFO) << "Validating RELMDS parameter d - " << d << " maxIteration - "<< maxIteration << " eps - " << eps<< " noOfBaseVectors - " << noOfBaseVectors <<" selStrategy - " << selStrategy << " maxCalcTime - " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

    ValidateParams::validateInterval(d, 1, ValidateParams::inFile->getNumberOfAttributes(), "d");

    ValidateParams::validateInterval(maxIteration, 1, 10000, "maxIteration");

    ValidateParams::validateGreatherThan(eps, 0, "eps", true);

    ValidateParams::validateInterval(noOfBaseVectors, 0, 100, "noOfBaseVectors", false, true );

    ValidateParams::validateInterval(selStrategy, 0, 2, "selStrategy");
}
/**
 * Function that checks if samann parameters are OK
 */
void ValidateParams::samann(int d, int maxIteration, double mTrain, int nNeurons, double eta, int p, int maxCalcTime)
{
LOG(INFO) << "Validating SAMANN parameter d - " << d << " maxIteration - "<< maxIteration << " mTrain - " << mTrain<< " nNeurons - " << nNeurons << " eta - " << eta <<" p - " << p <<" maxCalcTime - " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", true);

    ValidateParams::validateInterval(d, 1, ValidateParams::inFile->getNumberOfAttributes(), "d");

    ValidateParams::validateInterval(maxIteration, 1, 10000, "maxIteration");

    ValidateParams::validateInterval(mTrain, 0, 100, "mTrain", false, true);

    ValidateParams::validateGreatherThan(nNeurons, 0, "nNeurons", false);

    ValidateParams::validateGreatherThan(eta, 0, "eta", false);

    ValidateParams::validateInterval(p, 1, ServiceSettings::noOfProcessors, "p");
}

/**
 * Function that checks if samann parameters are OK
 */
void ValidateParams::somMds(int rows, int columns, int eHat, int mdsIteration, int mdsProjection, double eps, int maxCalcTime)
{
    LOG(INFO) << "Validating SOMMDS parameter rows - " << rows << " columns - "<< columns <<  " eHat - " << eHat << " mdsIteration - " << mdsIteration << " mdsProjection - " << mdsProjection << " eps - " << eps <<" maxCalcTime - " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

    ValidateParams::validateInterval(rows, 3, 100, "rows");

    ValidateParams::validateInterval(columns, 3, 100, "columns");

    ValidateParams::validateInterval(eHat, 1, 1000, "eHat");

    ValidateParams::validateInterval(mdsIteration, 1, 10000, "mdsIteration");

    ValidateParams::validateGreatherThan(eps, 0, "eps", true);

    ValidateParams::validateInterval(mdsProjection, 2, 2, "mdsProjection");
}

    //classification, grouping
/**
 * Function that checks if som parameters are OK
 */
void ValidateParams::som(int rows, int columns, int eHat, int p, int maxCalcTime)
{
    LOG(INFO) << "Validating SOM parameter rows - " << rows << " columns - "<< columns <<  " eHat - " << eHat << " maxCalcTime " << maxCalcTime;

     ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

     ValidateParams::validateInterval(rows, 3, 100, "rows");

     ValidateParams::validateInterval(columns, 3, 100, "columns");

     ValidateParams::validateInterval(eHat, 1, 1000, "eHat");

     ValidateParams::validateInterval(p, 1, ServiceSettings::noOfProcessors, "p");
}
/**
 * Function that checks if mlp parameters are OK
 */
void ValidateParams::mlp(int h1pNo, int h2pNo, double qty, bool validationMethod, int p, int maxIteration, int maxCalcTime)
{
    LOG(INFO) << "Validating MLP parameter h1pNo - " << h1pNo << " h2pNo - "<< h2pNo <<  " qty - " << qty << " validationMethod - " << validationMethod << " p - " << p << " maxIteration - " << maxIteration << " maxCalcTime - " << maxCalcTime;

   ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

   if (!validationMethod)
        ValidateParams::validateInterval(qty, 0, 100, "qty", false, false );
    else
        ValidateParams::validateInterval(qty, 2, ValidateParams::inFile->getNumberOfObjects(), "qty", true, true );

  // ValidateParams::validateInterval(dT, 1, 100, "dT");

  // ValidateParams::validateInterval(wDecay, 0.0000001, 100, "wDecay");

   // ValidateParams::validateGreatherThan(100, (dL + dT), "(dL + dT)", true, true);

    ValidateParams::validateInterval(p, 1, ServiceSettings::noOfProcessors, "p");

    ValidateParams::validateInterval(maxIteration, 1, 10000, "maxIteration");

    ValidateParams::validateGreatherThan(h1pNo, 0, "h1pNo", true);

    ValidateParams::validateGreatherThan(h2pNo, 0, "h2pNo", true);

   // ValidateParams::validateGreatherThan(kFold, 1, "kFold", true);
    if (!ValidateParams::inFile->isClassFound())
    {
        ErrorResponse::setFaultDetail("File is not valid. No class attribute");
        LOG(ERROR) << "File is not valid. No class attribute";
        ValidateParams::methodParamsValid = false;
    }
}
/**
 * Function that checks if c45 parameters are OK
 */
void ValidateParams::decForest(double r, double dL, double dT, int maxCalcTime)
{
    LOG(INFO) << "Validating Decision Forest parameter r - "<< r << "  dL - " << dL << " dT - " << dT;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", true);

    ValidateParams::validateInterval(r, 0, 1, "r",false, true);

    ValidateParams::validateInterval(dL, 1, 100, "dL");

    ValidateParams::validateInterval(dT, 1, 100, "dT");

    ValidateParams::validateGreatherThan(100, (dL + dT), "(dL + dT)", true, true);

    if (!ValidateParams::inFile->isClassFound())
    {
        ErrorResponse::setFaultDetail("File is not valid. No class attribute");
        LOG(ERROR) << "File is not valid. No class attribute";
        ValidateParams::methodParamsValid = false;
    }

}
/**
 * Function that checks if kMeans parameters are OK
 */
void ValidateParams::kMeans(int maxIteration, int kMax, int maxCalcTime)
{
    LOG(INFO) << "Validating KMEANS parameter kMax - "<< kMax << "  maxIteration - " << maxIteration << " maxCalcTime - " << maxCalcTime;

    ValidateParams::validateInterval(kMax, 1, ValidateParams::inFile->getNumberOfObjects(), "kMax");
    //ValidateParams::validateInterval(kMax, 1, 100, "kMax");

    ValidateParams::validateInterval(maxIteration, 1, 10000, "maxIteration");

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", true);
}


/**
 * Function that checks if STATPRIMITIVES parameters are OK
 */
void ValidateParams::statPrimitives(int maxCalcTime)
{
    LOG(INFO) << "Validating STATPRIMITIVES parameter maxCalcTime " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", true);
}


/**
 * Function that checks if TRANSPOSE parameters are OK
 */
void ValidateParams::transposeData(int maxCalcTime)
{
    LOG(INFO) << "Validating TRANSPOSE parameter maxCalcTime " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", true);
}

/**
 * Function that checks if SPLITDATA parameters are OK
 */
 void ValidateParams::splitData(bool reshufleObjects,double firstSubsetPerc, double secondSubsetPerc, int maxCalcTime)
 {
    LOG(INFO) << "Validating SPLITDATA parameters reshufleObjects - " << reshufleObjects << " firstSubsetPerc - " << firstSubsetPerc << " secondSubsetPerc - "  << secondSubsetPerc << " maxCalcTime - " << maxCalcTime;

   ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false, false);

    ValidateParams::validateGreatherThan(firstSubsetPerc, 0, "firstSubsetPerc", false, false);

    ValidateParams::validateGreatherThan(secondSubsetPerc, 0, "secondSubsetPerc", false, false);

    ValidateParams::validateGreatherThan(100, firstSubsetPerc + secondSubsetPerc, "(firstSubsetPerc + secondSubsetPerc)", true, true);
 }

/**
 * Function that checks if FILTERDATA parameters are OK
 */
void ValidateParams::filterData(bool retFilteredData, double zValue, int attrIndex, int maxCalcTime)
{

    LOG(INFO) << "Validating FILTERDATA parameters retFilteredData - " << retFilteredData << " zValue - " << zValue << " attrIndex - "  << attrIndex << " maxCalcTime " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false, false);

    int noOfAttr = (ValidateParams::inFile->isClassFound()) ? ValidateParams::inFile->getNumberOfAttributes() + 1 : ValidateParams::inFile->getNumberOfAttributes(); //if there are class add 1 to atrributes

    ValidateParams::validateGreatherThan(zValue, 0, "zValue", false, false);

    ValidateParams::validateGreatherThan(attrIndex, 0, "attrIndex", true, false);

    ValidateParams::validateGreatherThan(noOfAttr, attrIndex, "attrIndex > noOfAttr", true, true);
}

/**
 * Function that checks if CLEANDATA parameters are OK
 */
void ValidateParams::cleanData(int maxCalcTime)
{
    LOG(INFO) << "Validating CLEANDATA parameter maxCalcTime " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);
}

/**
 * Function that checks if NORMDATA parameters are OK
 */
void ValidateParams::normData(bool normMeanStd, double a, double b, int maxCalcTime)
{
    LOG(INFO) << "Validating NORMDATA parameters normMeanStd - " << normMeanStd << " a - " << a << " b - "  << b << " maxCalcTime " << maxCalcTime;

    ValidateParams::validateGreatherThan(maxCalcTime, 0, "maxCalcTime", false);

    if (normMeanStd)
        ValidateParams::validateGreatherThan(b, 0, "b", true, false);
    else
        ValidateParams::validateGreatherThan(b, a, "a>b", false, true);
}


/**
 * Returns true if val is in interval
 */
bool ValidateParams::validateInterval(double val, double lBound, double uBound, std::string printVal, bool lowerInclusive, bool upperInclusive)
{
        char bufferuBound[32];
        snprintf(bufferuBound, sizeof(bufferuBound), "%g", uBound);

        char bufferlBound[32];
        snprintf(bufferlBound, sizeof(bufferlBound), "%g", lBound);

        char bufferval[32];
        snprintf(bufferval, sizeof(bufferval), "%g", val);

    if (lowerInclusive && upperInclusive)
    {
        if (!(val >= lBound && val <= uBound))
        {
            ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be in interval [" + bufferlBound +"; "+ bufferuBound + "]");
            LOG(ERROR) << "Parameter "<< printVal << " - "<< val <<" is not valid. " << printVal << " must be in interval [" <<lBound <<"; " << uBound << "]";
            ValidateParams::methodParamsValid = false;
            return false;
        }
    }
    else if (lowerInclusive && !upperInclusive)
    {
        if (!(val >= lBound && val < uBound))
        {
            ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be in interval [" + bufferlBound +"; "+ bufferuBound + ")");
            LOG(ERROR) << "Parameter "<< printVal << " - "<< val <<" is not valid. " << printVal << " must be in interval [" <<lBound <<"; " << uBound << ")";
            ValidateParams::methodParamsValid = false;
            return false;
        }
    }
    else if (!lowerInclusive && upperInclusive)
    {
        if (!(val > lBound && val <= uBound))
        {
            ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be in interval (" + bufferlBound +"; "+ bufferuBound + "]");
            LOG(ERROR) << "Parameter "<< printVal << " - "<< val <<" is not valid. " << printVal << " must be in interval (" <<lBound <<"; " << uBound << "]";
            ValidateParams::methodParamsValid = false;
            return false;
        }
    }
     else
        {
        if (!(val > lBound && val < uBound))
        {
            ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be in interval (" + bufferlBound +"; "+ bufferuBound + ")");
            LOG(ERROR) << "Parameter "<< printVal << " - "<< val <<" is not valid. " << printVal << " must be in interval (" <<lBound <<"; " << uBound << ")";
            ValidateParams::methodParamsValid = false;
            return false;
        }
        }
        return true;
}

/**
 * Returns true if val is greather or equal to another value
 */
bool ValidateParams::validateGreatherThan(double val, double lBound, std::string printVal, bool lowerInclusive, bool inverse)
{
        char bufferlBound[32];
        snprintf(bufferlBound, sizeof(bufferlBound), "%g", lBound);

        char bufferval[32];
        snprintf(bufferval, sizeof(bufferval), "%g", val);

    if (lowerInclusive)
    {
        if (!(val >= lBound ))
        {
            if (inverse)
                {
                    ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be less or equal than " + bufferval);
                    LOG(ERROR) << "Parameter "<< printVal << " - "<< lBound <<" is not valid. " << printVal << " must be less or equal than " <<val ;
                }
            else
                {
                    ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be greather or equal than " + bufferlBound);
                    LOG(ERROR) << "Parameter "<< printVal << " - "<< val <<" is not valid. " << printVal << " must be greater or equal than " <<lBound ;
                }
            ValidateParams::methodParamsValid = false;
            return false;
        }
    }
    else
    {
        if (!(val > lBound ))
        {
            if (inverse)
                {
                    ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be less or equal than " + bufferval);
                    LOG(ERROR) << "Parameter "<< printVal << " - "<< lBound <<" is not valid. " << printVal << " must be less or equal than " <<val ;
                }
            else
                {
                    ErrorResponse::setFaultDetail("Parameter is not valid. " + printVal + " must be greather or equal than " + bufferlBound);
                    LOG(ERROR) << "Parameter "<< printVal << " - "<< val <<" is not valid. " << printVal << " must be greater or equal than " <<lBound ;
                }
            ValidateParams::methodParamsValid = false;
            return false;
        }
    }
    return true;
}
/**
 * Returns true if all parameters are OK
 */
bool ValidateParams::isValid()
{
    if (!ValidateParams::methodParamsValid)
        {
            LOG(ERROR) << " Parameters are NOT OK";
            HelperMethods::deleteFile(ValidateParams::inFile->getFilePath()); // delete file if parameters are not ok
        }
    else
        LOG(INFO) << " Parameters are OK";

    return (ValidateParams::methodParamsValid);
}

/**
 * Validates the input arff file;
 */
/*void ValidateParams::validateFile()
{
    LOG(INFO) << "Initiating file " << ValidateParams::initDamisServFile->getFilePath() << " data validation";

    int dataRows = ValidateParams::initDamisServFile->getRawDataStringFormat().size();
    int dataColumns = ValidateParams::initDamisServFile->getRawDataStringFormat().at(0).size();
    bool isFileValid = true;

    bool classMatch;
    std::string dataVal;
   // const char *str;
    char *err;
    double x;

    if ( dataRows > 0 && dataColumns > 0)
    {
        for (int i = 0; i < dataRows; i++ )
        {
          //  skipObject = false; //if data values are ok
            classMatch = false; //if match found

            for (int j = 0; j < dataColumns; j++)
            {
                    dataVal = ValidateParams::initDamisServFile->getRawDataStringFormat().at(i).at(j);
                    //str = dataVal.c_str();
                   // char *err;
                    x = strtod(dataVal.c_str(), &err);

                    if (!(*err == 0 && dataVal !=""))
                    {
                        isFileValid = false;
                        ErrorResponse::setFaultDetail("File is not valid, found non numeric value \\ " + std::string(ValidateParams::initDamisServFile->getRawDataStringFormat().at(i).at(j)) + " \\ at line " + std::to_string(static_cast<long long>(i)) + std::string("at position ") + std::to_string (static_cast<long long>(j)) + std::string(" "));
                        LOG(ERROR) << "File is not valid (skipping object), found non numeric value \\ " << ValidateParams::initDamisServFile->getRawDataStringFormat().at(i).at(j) << " \\ at line " <<i <<" at position " <<j;
                    }
            }
            if (ValidateParams::initDamisServFile->isClassFound())
            {
                for (unsigned int cL = 0; cL < ValidateParams::initDamisServFile->getClasses().size(); cL++)
                {

// TODO (Povilas#1#): Palyginti didziosiomis arba mazosiomis raidemis ...
//
                    if (ValidateParams::initDamisServFile->getClassAttribute().at(i) ==  ValidateParams::initDamisServFile->getClasses().at(cL))
                    {
                        classMatch = true;
                        break;
                    }
                }
                if (!classMatch)
                {
                    isFileValid = false;
                    ErrorResponse::setFaultDetail(std::string("File is not valid. Data section has unknown class ") + std::string (ValidateParams::initDamisServFile->getClassAttribute().at(i)) + std::string(" at line ") + std::to_string(static_cast<long long>(i)) + std::string(" "));
                    LOG(ERROR) << "File is not valid. Data section line " << i << " contains unknown class " << ValidateParams::initDamisServFile->getClassAttribute().at(i);
                }
            }
        }
    }

    ValidateParams::fileIsValid = isFileValid;

    if (!isFileValid)
    {
        LOG(ERROR) << "File " << ValidateParams::initDamisServFile->getFilePath() << " is not valid";

        // must comment since deletion is moved to ValidateParams::isValid function
        HelperMethods::deleteFile(ValidateParams::initDamisServFile->getFilePath());
    }
    else
    {
        LOG(INFO) << "File " << ValidateParams::initDamisServFile->getFilePath() << " valid (OK)";
    }
}*/
