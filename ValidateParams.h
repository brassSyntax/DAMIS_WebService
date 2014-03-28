///////////////////////////////////////////////////////////
//  ValidateParams.h
//  Implementation of the Class ValidateParams
//  Created on:      03-Vas-2014 21:44:17
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(VALIDATEPARAMS_H)
#define VALIDATEPARAMS_H

#include "InitDamisServiceFile.h"

/**
 * Class that actually performs input parameter vand arffFile object validation
 */
class ValidateParams
{

public:
    /**
    * Class contructor that initiates ValidateParams object
    */
	ValidateParams(InitDamisServiceFile *inFile, bool validateFile = true);
	/**
    * Empty destructor
    */
	virtual ~ValidateParams();
	/**
    * Method that returns the validation result
    */
	bool isValid();

	//preprocessing
    /**
    * Validate data normalization prameters
    */
	void normData(bool normMeanStd, double a, double b, int maxCalcTime);
    /**
    * Validate data transpose parameters
    */
	void transposeData(int maxCalcTime);
    /**
    * Validate data splitting parameters
    */
	void splitData(bool reshufleObjects,double firstSubsetPerc, double secondSubsetPerc, int maxCalcTime);
    /**
    * Validate data filtering parameters
    */
	void filterData (bool retFilteredData, double zValue, int attrIndex, int maxCalcTime);
    /**
    * Validate data cleaning parameters
    */
	void cleanData(int maxCalcTime);

	//statistical primitives
    /**
    * Validate the parameters of the statistic calculations
    */
	void statPrimitives (int maxCalcTime);

	//dimmension reduction
    /**
    * Validate pca parameters
    */
	void pca(bool projType, double d, int maxCalcTime);
	/**
    * Validate smacofMds parameters
    */
	void smacofMds(int d, int maxIteration, double eps, bool zeidel, int p, int maxCalcTime);
	/**
    * Validate dma parameters
    */
    void dma(int d, int maxIteration, double eps, double neighbour, int maxCalcTime);
    /**
    * Validate relMds parameters
    */
	void relMds(int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy, int maxCalcTime);
    /**
    * Validate samann parameters
    */
	void samann(int d, int maxIteration, double mTrain, int nNeurons, double eta, int p, int maxCalcTime);
    /**
    * Validate somMds parameters
    */
	void somMds(int rows, int columns, int eHat, int mdsIteration, int mdsProjection, double eps, int maxCalcTime);

    //classification, grouping
    /**
    * Validate som parameters
    */
    void som(int rows, int columns, int eHat, int p, int maxCalcTime);
    /**
    * Validate mlp parameters
    */
	void mlp(int h1pNo, int h2pNo, int h3pNo, double dL, double dT, double dV, int p, int maxIteration, int maxCalcTime);
    /**
    * Validate c45 parameters
    */
	void c45(double q, double dL, double dT, int maxCalcTime);
    /**
    * Validate kMeans parameters
    */
	void kMeans(int maxIteration, int kMax, int maxCalcTime);


private:
    /**
     *Pointer to the InitDamisService file
     */
    InitDamisServiceFile *initDamisServFile;
    /**
     *Indicates if the parameters are valid
     */
    bool methodParamsValid;
     /**
     *Indicates if the file data is valid
     */
    bool fileIsValid;
	/**
	 * Validates arff file
	 */
	void validateFile();
	/**
	 * Validates int time parameter
	 */
	//bool validateTime(int maxCalcTime);
	/**
	 * Validates projection dimension parameter
	 */
	//bool validateProjectionDimension(int d);
	/**
	 * Validates maxIteration parameter
	 */
	//bool validateMaxIteration(int maxIteration);
	/**
	 * Validates maxIteration parameter
	 */
	//bool validateNoOfProcessors(int p);
	/**
	 * Validates if values is in interval
	 */
	bool validateInterval(double val, double lBound, double uBound, std::string printVal, bool lowerInclusive = true, bool upperInclusive = true);
    /**
	 * Validates if value is greather than
	 */
	bool validateGreatherThan(double val, double lBound, std::string printVal, bool lowerInclusive = true, bool inverse = false);


};
#endif // !defined(VALIDATEPARAMS_H)
