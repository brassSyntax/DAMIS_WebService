#define _ELPP_THREAD_SAFE
#define _ELPP_STL_LOGGING
#define _ELPP_NO_DEFAULT_LOG_FILE

#include "ServiceSettings.h"

#include "gsoap/soapDAMISService.h"
#include "gsoap/soapH.h"
#include "gsoap/DAMIS.nsmap"

#include "CGIvars/getcgivars.h"

#include "logging/easylogging++.h"
#include "InitDamisService.h"
#include "ValidateParams.h"
#include "ErrorResponse.h"
#include "Preprocess.h"
#include "Statistics.h"
#include "DimensionReduction.h"
#include "ClassificationGrouping.h"
#include "HelperMethods.h"


#include <iostream>

int streamWSDLFile(struct soap *sp); //function prototype for wsdl file streaming
int sendError(struct soap *sp); //function prototype for error sending to client
int sendError(struct soap *sp, std::string message); //function prototype for error sending to client with passed  error

_INITIALIZE_EASYLOGGINGPP

//namespace easyloggingpp {}

int main()
{
    DAMISService ds(true, true);
    if (!ServiceSettings::initialize())
    {
        sendError(ds.soap);
        return SOAP_ERR;
    }


    easyloggingpp::Configurations conf(ServiceSettings::logingConfFilePath);
    // Reconfigure single logger
    easyloggingpp::Loggers::reconfigureAllLoggers(conf);

    // LOG(INFO) << "Invoking service operation";

 /*   InitDamisService *dFile = new InitDamisService("http://158.129.140.146/Damis/Data/testData/iris_su_klasem.arff", "_input_"); //if clen data -> pass validateFile = false

    if (!ErrorResponse::isFaultFound()) //remove error checking if clean data is called
    {
        ValidateParams *validate = new ValidateParams(dFile); //when call clean data do not validate file, i.e also pass FALSE otherwise validate->isValid may return false
        //validate->normData(false, 0, 1, 1);
        //validate->cleanData(1);
        //validate->filterData(true, 3, 4, 1);
        //validate->splitData(false, 10, 20, 1);
        //validate->transposeData(1);
        //validate->statPrimitives(10);
        validate->pca(false,2,2);
        //validate->smacofMds(3,4,0.2,true,2,7);
        //validate->dma(2,10,0.88,1,4);

        //validate->relMds()
        //validate->relMds(2,10,0.0004,10,1,1);
        //validate->samann(8,7,88,-7,0.1478,8,7);
        // validate->somMds(4,3,5,8,0.0001,8,-4);
        //validate->somMds(4,5,4,100,0.0001,2,1);
        //validate->som()
        //validate->som(4,4,8,1,5);
        //validate->kMeans(100,2,9);
        //validate->mlp(2,2,10,false, 1, 100, 1);
        //validate->decForest(0.63, 80, 20, 8);


        if (validate->isValid())
        {
            // Preprocess *dRun = new Preprocess (dFile);


            clock_t start;
            //long double duration;

            //LOG(INFO)<<"Starting clock";
            start = clock();
            //dRun->cleanData();
            //dRun->transposeData();
            //dRun->splitData(true, 10, 20);
            //dRun->normData(false, 0, 1);
            //dRun->filterData(false, 3, 4);

            //Statistics *dStat = new Statistics (dFile);
            //dStat->statPrimitives();

            DimensionReduction *dRun = new DimensionReduction(1, 10, dFile);
             dRun->runPCA(false, 2);
            //dRun->runDMA(2,10,0.0088, 1);
            //void relMds(int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy, int maxCalcTime);
            // dRun->runRELATIVEMDS(2,1,0.0004,10,1);
            // dReduction->runSAMANN(2,100,10,2,0.1);
            // dReduction->runSMACOFMDS(2,100,0.00001,true);
            // dReduction->runSOMMDS(5,5,2,100,0.0001,3);

          // ClassificationGrouping *dRun = new ClassificationGrouping(1,10, dFile);
            //dRun->runSOM(3,3,2);
            //dRun->runKMEANS(100, 2);
            //dRun->runMLP(2, 2, 80, 100, false);
          //  dRun->runDecForest(0.63, 80, 20, 75);


            //          std::cout1 << (clock() - start) / (double) CLOCKS_PER_SEC;

            std::string response = dRun->outFile->getHttpPath();
            std::string statFile = dRun->statFile->getFilePath();
            //std::string altFile = dRun->altOutFile->getFilePath();

            // double algorithmError = HelperMethods::getAttributeValue(dRun->statFile->getFilePath(), "algError");

            //returns only cluster calculataion time
            // double calcTime  = HelperMethods::getAttributeValue(dRun->statFile->getFilePath(), "calcTime");

            //overall time for request serving
            //response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;
            //HelperMethods::deleteFile(dRun->outFile->getFilePath());
            HelperMethods::deleteFile(statFile);

        }
        else
            return -1; //fault was found
    }
    else
        return -1; //fault was found*/


    //std::cout1 << "Hello world!" << std::endl;


    LOG(INFO) <<"Service has been invoked";
    // DAMISService ds;

    //At each service invoke check and delete old file if they are present
    HelperMethods::deleteOldFiles();


    bool parseWSDL = false;
    char **cgivars = NULL;
    int i;

    cgivars = getcgivars();

    if (cgivars != NULL)
    {
        LOG(INFO)<<"Checking CGI variables if it is WSDL content request";
        for (i = 0; cgivars[i]; i += 2)
            if (std::string(cgivars[i]) == "wsdl")
                parseWSDL = true;

        if (parseWSDL)
        {
            LOG(INFO)<<"Found WSDL content request, creating file content stream";
            streamWSDLFile(ds.soap);
            LOG(INFO)<<"Done streaming content";
            return SOAP_OK;
        }
        else
        {
            LOG(INFO)<<"WSDL tag not found, will try to serve the request";
            return ds.serve();
        }

        for (i=0; cgivars[i]; i++)
            free(cgivars[i]);
        free(cgivars);
    }

    LOG(INFO)<<"No CGI variables, trying to serve the request";
    return ds.serve();

    //return SOAP_OK;*/

}

/// Web service operation 'PCA' (returns error code or SOAP_OK)
int DAMISService::PCA(std::string X, bool projType, double d, int maxCalcTime, struct Damis__PCAResponse &_param_1)
{
    LOG(INFO) << "Initiating PCA serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; d - "<< d <<"; projType - "<< projType <<"; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();


    LOG(INFO)<<"Instantiating InitDamisServiceFile object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //   LOG(INFO)<<"Validating passed PCA params";

        //void pca(bool projType, double d, int maxCalcTime);
        //validate->pca(projType, d, maxCalcTime);
        validate->pca(projType, d, maxCalcTime);
        //ValidateParams::pca(bool projType, double d, int maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating DimensionReduction object";
            DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runPCA(projType, d);

            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__PCAResponse *response = new Damis__PCAResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }

        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'SMACOFMDS' (returns error code or SOAP_OK)
int DAMISService::SMACOFMDS(std::string X, int d, int maxIteration, double eps, bool zeidel, int p, int maxCalcTime, struct Damis__SMACOFMDSResponse &_param_1)
{
    LOG(INFO) << "Initiating SMACOFMDS serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; d - "<< d <<"; maxIteration - "<< maxIteration << "; eps - "<< eps<< "; zeidel - "<< zeidel<< "; p - "<< p<< "; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();


    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SMACOFMDS params";

        //void smacofMds(int d, int maxIteration, double eps, bool zeidel, int p, int maxCalcTime);
        validate->smacofMds(d, maxIteration, eps, zeidel, p, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating DimensionReduction object";
            DimensionReduction *dReduction = new DimensionReduction(p, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runSMACOFMDS(d, maxIteration, eps, zeidel);

            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__SMACOFMDSResponse *response = new Damis__SMACOFMDSResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}

/// Web service operation 'DMA' (returns error code or SOAP_OK)
int DAMISService::DMA(std::string X, int d, int maxIteration, double eps, double neighbour, int maxCalcTime, struct Damis__DMAResponse &_param_1)
{
    LOG(INFO) << "Initiating DMA serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; d - "<< d <<"; maxIteration - "<< maxIteration << "; eps - "<< eps<< "; neighbour - "<< neighbour<< "; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    // long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed DMA params";

        //void dma(int d, int maxIteration, double eps, double neighbour, int maxCalcTime);
        validate->dma(d, maxIteration, eps, neighbour, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating DimensionReduction object";
            DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runDMA(d, maxIteration, eps, neighbour);

            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__DMAResponse *response = new Damis__DMAResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'RELMDS' (returns error code or SOAP_OK)
int DAMISService::RELMDS(std::string X, int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy, int maxCalcTime, struct Damis__RELMDSResponse &_param_1)
{
    LOG(INFO) << "Initiating RELMDS serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; d - "<< d <<"; maxIteration - "<< maxIteration << "; eps - "<< eps<< "; noOfBaseVectors - "<< noOfBaseVectors << "; selStrategy - "<< selStrategy<< "; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed RELMDS params";

        //void relMds(int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy, int maxCalcTime);
        validate->relMds(d, maxIteration, eps, noOfBaseVectors, selStrategy, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating DimensionReduction object";
            DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runRELATIVEMDS(d, maxIteration, eps, noOfBaseVectors, selStrategy);
            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__RELMDSResponse *response = new Damis__RELMDSResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'SAMANN' (returns error code or SOAP_OK)
int DAMISService::SAMANN(std::string X, int d, int maxIteration, double mTrain, int nNeurons, double eta, int p, int maxCalcTime, struct Damis__SAMANNResponse &_param_1)
{

    LOG(INFO) << "Initiating SAMANN serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; d - "<< d <<"; maxIteration - "<< maxIteration << "; mTrain - "<< mTrain<< "; nNeurons - "<< nNeurons << "; eta - "<< eta<< "; p - "<< p<<"; maxCalcTime - "<< maxCalcTime;
    clock_t start;

    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SAMANN params";

        //void samann(int d, int maxIteration, double mTrain, int nNeurons, double eta, int p, int maxCalcTime);
        validate->samann(d, maxIteration, mTrain, nNeurons, eta, p, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating DimensionReduction object";
            DimensionReduction *dReduction = new DimensionReduction(p, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            //void runSAMANN(int d, int maxIteration, double mTrain, int nNeurons, double eta);
            dReduction->runSAMANN(d, maxIteration, mTrain, nNeurons, eta);

            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__SAMANNResponse *response = new Damis__SAMANNResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'SOM' (returns error code or SOAP_OK)
int DAMISService::SOM(std::string X, int rows, int columns, int eHat, int p, int maxCalcTime, struct Damis__SOMResponse &_param_1)
{
    LOG(INFO) << "Initiating SOM serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; rows - "<< rows <<"; columns - "<< columns << "; eHat - "<< eHat<< "; p - "<< p <<"; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SOM params";

        //void void som(int rows, int columns, int eHat, int p, int maxCalcTime);
        validate->som(rows, columns, eHat, p, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating DimensionReduction object";
            ClassificationGrouping *dReduction = new ClassificationGrouping(p, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runSOM(rows, columns,eHat);

            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__SOMResponse *response = new Damis__SOMResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'SOMMDS' (returns error code or SOAP_OK)
int DAMISService::SOMMDS(std::string X, int rows, int columns, int eHat, int mdsIteration, double eps, int mdsProjection, int maxCalcTime, struct Damis__SOMMDSResponse &_param_1)
{
    LOG(INFO) << "Initiating SOMMDS serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; rows - "<< rows <<"; columns - "<< columns << "; eHat - "<< eHat<< "; mdsIteration - "<< mdsIteration <<"; eps - "<< eps <<"; mdsProjection - "<< mdsProjection <<"; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SOMMDS params";

        //void somMds(int rows, int columns, int eHat, int mdsIteration, int mdsProjection, double eps, int maxCalcTime);
        validate->somMds(rows, columns, eHat, mdsIteration, mdsProjection, eps, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating DimensionReduction object";
            DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runSOMMDS(rows, columns,eHat, mdsIteration, eps, mdsProjection);
            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__SOMMDSResponse *response = new Damis__SOMMDSResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'MLP' (returns error code or SOAP_OK)
int DAMISService::MLP(std::string X, int h1pNo, int h2pNo, double qty, bool kFoldValidation, int maxIteration, int p, int maxCalcTime, struct Damis__MLPResponse &_param_1)
{
// TODO (Povilas#1#): Ne5gyvendintas
    LOG(INFO) << "Initiating MLP serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; h1pNo - "<< h1pNo <<"; h2pNo - "<< h2pNo << "; qty - "<< qty <<"; kFoldValidation - "<< kFoldValidation << "; p - " << p <<"; maxCalcTime - "<< maxCalcTime;

    //int qty = h3pNo;
    //bool validateMethod = dV;

    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SOMMDS params";

        //void somMds(int rows, int columns, int eHat, int mdsIteration, int mdsProjection, double eps, int maxCalcTime);
        //validate->somMds(rows, columns, eHat, mdsIteration, mdsProjection, eps, maxCalcTime);
        validate->mlp(h1pNo, h2pNo, qty, kFoldValidation, p, maxIteration, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating ClassificationGrouping object";
            ClassificationGrouping *dReduction = new ClassificationGrouping(p, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runMLP(h1pNo, h2pNo, qty, maxIteration, kFoldValidation);
            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__MLPResponse *response = new Damis__MLPResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'RDF' (returns error code or SOAP_OK)
int DAMISService::DF(std::string X, double q, double dL, double dT, int maxCalcTime, struct Damis__DFResponse &_param_1)
{
// TODO (Povilas#1#): Ne5gyvendintas
    // LOG(INFO)<<"C45 not yet implemented, return SOAP_ERR";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; q - "<< q <<"; dL - "<< dL << "; dT - "<< dT<< "; dL - "<< dL <<"; maxCalcTime - "<< maxCalcTime;

    clock_t start;
    //  long double duration;
    double r = q;
    int nTree = 75;
    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SOMMDS params";

        //void somMds(int rows, int columns, int eHat, int mdsIteration, int mdsProjection, double eps, int maxCalcTime);
        //validate->somMds(rows, columns, eHat, mdsIteration, mdsProjection, eps, maxCalcTime);
        validate->decForest(r, dL, dT, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating ClassificationGrouping object";
            ClassificationGrouping *dReduction = new ClassificationGrouping(1, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runDecForest(r, dL, dT, nTree);
            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__DFResponse *response = new Damis__DFResponse();

                response->Y = dReduction->outFile->getHttpPath();
                response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";

    // return sendError(this->soap, "C45 algorithm not yet implemented");
    return SOAP_OK;
}
/// Web service operation 'KMEANS' (returns error code or SOAP_OK)
int DAMISService::KMEANS(std::string X, int kMax, int maxIteration, int maxCalcTime, struct Damis__KMEANSResponse &_param_1)
{
    LOG(INFO) << "Initiating KMEANS serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; kMax - "<< kMax <<"; maxIteration - "<< maxIteration << "; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SOM params";

        //void void som(int rows, int columns, int eHat, int p, int maxCalcTime);
        validate->kMeans(maxIteration, kMax, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating ClassificationGouping object";
            ClassificationGrouping *dReduction = new ClassificationGrouping(1, maxCalcTime, dFile);

            LOG(INFO)<< "Method RUN";
            dReduction->runKMEANS(maxIteration, kMax);
            if (!ErrorResponse::isFaultFound())
            {
                LOG(INFO)<<"Creating parameter response structure";
                struct Damis__KMEANSResponse *response = new Damis__KMEANSResponse();

                response->Y = dReduction->outFile->getHttpPath();
                // response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

                //returns only cluster calculataion time
                //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

                //overall time for request serving
                response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;
                ///
                //  response->kBest = kMax;

                _param_1 = *response;

                //delete fileWith the statistics data
                HelperMethods::deleteFile(dReduction->statFile->getFilePath());
            }
            else
            {
                //got error send error message
                LOG(INFO)<<"Got error on DAMIS algorithm run phase, return SOAP_ERR";
                sendError(this->soap);
                return SOAP_ERR;
            }
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'STATPRIMITIVES' (returns error code or SOAP_OK)

int DAMISService::STATPRIMITIVES(std::string X, int maxCalcTime, struct Damis__STATPRIMITIVESResponse &_param_1)
{
    LOG(INFO) << "Initiating STATPRIMITIVES serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed STATPRIMITIVES params";

        validate->statPrimitives(maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating Statistics object";

            Statistics *dStat = new Statistics (dFile);

            LOG(INFO)<< "Method RUN";
            dStat->statPrimitives();

            //DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);


            // dReduction->runSOMMDS(rows, columns,eHat, mdsIteration, eps, mdsProjection);

            LOG(INFO)<<"Creating parameter response structure";
            struct Damis__STATPRIMITIVESResponse *response = new Damis__STATPRIMITIVESResponse();

            response->Y = dStat->outFile->getHttpPath();
            //response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

            //returns only cluster calculataion time
            //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

            //overall time for request serving
            response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

            _param_1 = *response;

            //delete fileWith the statistics data
            //   HelperMethods::deleteFile(dReduction->statFile->getFilePath());
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'CLEANDATA' (returns error code or SOAP_OK)
int DAMISService::CLEANDATA(std::string X, int maxCalcTime, struct Damis__CLEANDATAResponse &_param_1)
{
    LOG(INFO) << "Initiating CLEANDATA serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<<X<<"; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_", false); //passing also false because we do not need to valida the file

    /* if (!ErrorResponse::isFaultFound())
     {*/
    // LOG(INFO)<<"Instantiating ValidateParams object";
    ValidateParams *validate = new ValidateParams(dFile);
    // LOG(INFO)<<"Validating passed CLEANDATA params";

    validate->cleanData(maxCalcTime);   ///reikia paziureti kaip issisukti jei kvieciama cleandata, failas neturi buti validuojamas

    if (validate->isValid())
    {
        LOG(INFO)<< "Creating Preprocess object";

        Preprocess *dPrep = new Preprocess (dFile);

        LOG(INFO)<< "Method RUN";
        dPrep->cleanData();

        //DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);


        // dReduction->runSOMMDS(rows, columns,eHat, mdsIteration, eps, mdsProjection);

        LOG(INFO)<<"Creating parameter response structure";
        struct Damis__CLEANDATAResponse *response = new Damis__CLEANDATAResponse();

        response->Y = dPrep->outFile->getHttpPath();
        //response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

        //returns only cluster calculataion time
        //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

        //overall time for request serving
        response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

        _param_1 = *response;

        //delete fileWith the statistics data
        // HelperMethods::deleteFile(dReduction->statFile->getFilePath());
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    /*   }
       else
           {
               //got error send error message
               LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
               sendError(this->soap);
               return SOAP_ERR;
           }*/
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'FILTERDATA' (returns error code or SOAP_OK)
int DAMISService::FILTERDATA(std::string X, bool retFilteredData, double zValue, int attrIndex, int maxCalcTime, struct Damis__FILTERDATAResponse &_param_1)
{
    LOG(INFO) << "Initiating FILTERDATA serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<< X <<"; retFilteredData - "<< retFilteredData <<"; zValue - "<< zValue <<"; attrIndex - "<< attrIndex << "; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed FILTERDATA params";

        validate->filterData(retFilteredData, zValue, attrIndex, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating Preprocess object";

            Preprocess *dPrep = new Preprocess (dFile);

            LOG(INFO)<< "Method RUN";
            dPrep->filterData(retFilteredData, zValue, attrIndex);

            //DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);


            // dReduction->runSOMMDS(rows, columns,eHat, mdsIteration, eps, mdsProjection);

            LOG(INFO)<<"Creating parameter response structure";
            struct Damis__FILTERDATAResponse *response = new Damis__FILTERDATAResponse();

            response->Y = dPrep->outFile->getHttpPath();
            //response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

            //returns only cluster calculataion time
            //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

            //overall time for request serving
            response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

            _param_1 = *response;

            //delete fileWith the statistics data
            // HelperMethods::deleteFile(dReduction->statFile->getFilePath());
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'SPLITDATA' (returns error code or SOAP_OK)
int DAMISService::SPLITDATA(std::string X, bool reshufleObjects, double firstSubsetPerc, double secondSubsetPerc, int maxCalcTime, struct Damis__SPLITDATAResponse &_param_1)
{
    LOG(INFO) << "Initiating SPLITDATA serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<< X <<"; reshufleObjects - "<< reshufleObjects <<"; firstSubsetPerc - "<< firstSubsetPerc <<"; secondSubsetPerc - "<< secondSubsetPerc << "; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed SPLITDATA params";

        validate->splitData(reshufleObjects, firstSubsetPerc, secondSubsetPerc, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating Preprocess object";

            Preprocess *dPrep = new Preprocess (dFile);

            LOG(INFO)<< "Method RUN";
            dPrep->splitData(reshufleObjects, firstSubsetPerc, secondSubsetPerc);

            //DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);


            // dReduction->runSOMMDS(rows, columns,eHat, mdsIteration, eps, mdsProjection);

            LOG(INFO)<<"Creating parameter response structure";
            struct Damis__SPLITDATAResponse *response = new Damis__SPLITDATAResponse();

            response->Y = dPrep->outFile->getHttpPath();
            response->Yalt = dPrep->altOutFile->getHttpPath();
            //response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

            //returns only cluster calculataion time
            //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

            //overall time for request serving
            response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

            _param_1 = *response;

            //delete fileWith the statistics data
            // HelperMethods::deleteFile(dReduction->statFile->getFilePath());
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'TRANSPOSEDATA' (returns error code or SOAP_OK)
int DAMISService::TRANSPOSEDATA(std::string X, int maxCalcTime, struct Damis__TRANSPOSEDATAResponse &_param_1)
{
    LOG(INFO) << "Initiating TRANSPOSEDATA serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<< X <<"; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed TRANSPOSEDATA params";

        validate->transposeData(maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating Preprocess object";

            Preprocess *dPrep = new Preprocess (dFile);

            LOG(INFO)<< "Method RUN";
            dPrep->transposeData();

            //DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);


            // dReduction->runSOMMDS(rows, columns,eHat, mdsIteration, eps, mdsProjection);

            LOG(INFO)<<"Creating parameter response structure";
            struct Damis__TRANSPOSEDATAResponse *response = new Damis__TRANSPOSEDATAResponse();

            response->Y = dPrep->outFile->getHttpPath();
            // response->Y = dPrep->altOutFile->getHttpPath();
            //response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

            //returns only cluster calculataion time
            //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

            //overall time for request serving
            response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

            _param_1 = *response;

            //delete fileWith the statistics data
            // HelperMethods::deleteFile(dReduction->statFile->getFilePath());
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}
/// Web service operation 'NORMDATA' (returns error code or SOAP_OK)
int DAMISService::NORMDATA(std::string X, bool normMeanStd, double a, double b, int maxCalcTime, struct Damis__NORMDATAResponse &_param_1)
{
    LOG(INFO) << "Initiating NORMDATA serve request";
    LOG(INFO) <<"Got parameters: "<<"X - "<< X <<"; normMeanStd - "<< normMeanStd <<"; a - "<< a <<"; b - "<< b << "; maxCalcTime - "<< maxCalcTime;
    clock_t start;
    //  long double duration;

    //LOG(INFO)<<"Starting clock";
    start = clock();

    LOG(INFO)<<"Instantiating InitDamisService object";
    InitDamisService *dFile = new InitDamisService (X, "_input_");

    if (!ErrorResponse::isFaultFound())
    {
        //LOG(INFO)<<"Instantiating ValidateParams object";
        ValidateParams *validate = new ValidateParams(dFile);
        //LOG(INFO)<<"Validating passed NORMDATA params";

        validate->normData(normMeanStd, a, b, maxCalcTime);

        if (validate->isValid())
        {
            LOG(INFO)<< "Creating Preprocess object";

            Preprocess *dPrep = new Preprocess (dFile);

            LOG(INFO)<< "Method RUN";
            dPrep->normData(normMeanStd, a, b);

            //DimensionReduction *dReduction = new DimensionReduction(1, maxCalcTime, dFile);


            // dReduction->runSOMMDS(rows, columns,eHat, mdsIteration, eps, mdsProjection);

            LOG(INFO)<<"Creating parameter response structure";
            struct Damis__NORMDATAResponse *response = new Damis__NORMDATAResponse();

            response->Y = dPrep->outFile->getHttpPath();
            // response->Y = dPrep->altOutFile->getHttpPath();
            //response->algorithmError = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "algError");

            //returns only cluster calculataion time
            //response->calcTime  = HelperMethods::getAttributeValue(dReduction->statFile->getFilePath(), "calcTime");

            //overall time for request serving
            response->calcTime = (clock() - start) / (double) CLOCKS_PER_SEC;

            _param_1 = *response;

            //delete fileWith the statistics data
            // HelperMethods::deleteFile(dReduction->statFile->getFilePath());
        }
        else
        {
            //got error send error message
            LOG(INFO)<<"Got error on DAMIS validation phase, return SOAP_ERR";
            sendError(this->soap);
            return SOAP_ERR;
        }
    }
    else
    {
        //got error send error message
        LOG(INFO)<<"Got error on DAMIS initialization phase, return SOAP_ERR";
        sendError(this->soap);
        return SOAP_ERR;
    }
    LOG(INFO)<<"Request served, returning SOAP_OK";
    return SOAP_OK;
}


// function that streams wsdl content to client if requested
int streamWSDLFile(struct soap *sp)
{
    FILE *fd;
    // size_t r;
    //	const char *s = appSettings::wsdlPath;
    fd = fopen(ServiceSettings::wsdlPath.c_str(), "rb"); // open WSDL file to copy
    if (!fd)
        return 404; // return HTTP not found error
    //   soap->http_content

    sp->http_content = "text/xml"; // HTTP header with text/xml content
    soap_response(sp, SOAP_FILE);
    for (;;)
    {
        size_t r = fread(sp->tmpbuf, 1, sizeof(sp->tmpbuf), fd);
        if (!r)
            break;
        if (soap_send_raw(sp, sp->tmpbuf, r))
            break; // can't send, but little we can do about that
    }
    fclose(fd);
    return soap_end_send(sp);
}

int sendError(struct soap *sp)
{
    LOG(INFO)<<"Unable to serve request, returning error description";
    soap_receiver_fault(sp,ErrorResponse::getFaultString().c_str(),ErrorResponse::getFaultDetail().c_str());
    soap_send_fault(sp); // Send SOAP Fault to client
    return SOAP_ERR;
}

int sendError(struct soap *sp, std::string message)
{
    LOG(INFO)<<"Unable to serve request, returning error description";
    soap_receiver_fault(sp,ErrorResponse::getFaultString().c_str(),message.c_str());
    soap_send_fault(sp); // Send SOAP Fault to client
    return SOAP_ERR;
}
