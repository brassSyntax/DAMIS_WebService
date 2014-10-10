///////////////////////////////////////////////////////////
//  ValidateParams.h
//  Implementation of the Class ValidateParams
//  Created on:      03-Vas-2014 21:44:17
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(VALIDATEPARAMS_H)
#define VALIDATEPARAMS_H

#include <string>
#include "InitDamisService.h"

/** \brief Class that actually performs input parameter and arff file object validation
 *
 */
class ValidateParams
{

public:
    /** \brief Constructor
    * \param InitDamisService* pointer of the InitDamisService object
    *
    */
	ValidateParams(InitDamisService *inFile);
	/** \brief  Destructor
    *
    */
	virtual ~ValidateParams();
	/** \brief Method that returns the validation result and deletes input file if parameters are not valid
    *  \return bool validation result
    */
	bool isValid();

	///Preprocessing components

    /** \brief Validate data normalization parameters
     *
     * \param bool normalization type
     * \param double mean or l bound
     * \param double std or ubound
     * \param int predictive calulus time
     * \return void
     *
     */
	void normData(bool normMeanStd, double a, double b, int maxCalcTime);
    /** \brief Validate data transpose parameters
     *
     * \param int predictive calulus time
     * \return void
     *
     */
    void transposeData(int maxCalcTime);
    /** \brief Validate data splitting parameters
     *
     * \param bool reshufle data objects
     * \param double relative object count in firs set
     * \param double relative object count in second set
     * \param int predictive calulus time
     * \return void
     *
     */
	void splitData(bool reshufleObjects,double firstSubsetPerc, double secondSubsetPerc, int maxCalcTime);
    /** \brief Validate data filtering parameters
     *
     * \param bool return either filtered data or outliers
     * \param double Z value
     * \param int attribute index
     * \param int predictive calulus time
     * \return void
     *
     */
	void filterData (bool retFilteredData, double zValue, int attrIndex, int maxCalcTime);
    /** \brief Validate data cleaning parameters
     *
     * \param int predictive calulus time
     * \return void
     *
     */
	void cleanData(int maxCalcTime);

	///Statistical primitives components

    /** \brief Validate the parameters of the statistic calculations
     *
     * \param int predictive calulus time
     * \return void
     *
     */
	void statPrimitives (int maxCalcTime);

	///Dimmension reduction components

    /** \brief Validate pca parameters
     *
     * \param bool dimension or dispersion
     * \param double dimmension or dispersion part
     * \param int predictive calulus time
     * \return void
     *
     */
	void pca(bool projType, double d, int maxCalcTime);
    /** \brief Validate smacofMds parameters
     *
     * \param int dimmension
     * \param int number of maximum iterations
     * \param double difference between two susequent errors
     * \param bool apply Zeidel modification
     * \param int no of processors to use
     * \param int predictive calulus time
     * \return void
     *
     */
	void smacofMds(int d, int maxIteration, double eps, bool zeidel, int p, int maxCalcTime);
    /** \brief Validate dma parameters
     *
     * \param int dimmension of the projection
     * \param int max number of iterations
     * \param double difference between two subsequent iterations
     * \param double relative number of neighbours
     * \param int predictive calulus time
     * \return void
     *
     */
    void dma(int d, int maxIteration, double eps, double neighbour, int maxCalcTime);
    /** \brief Validate relMds parameters
     *
     * \param int dimmension of the projection
     * \param int max number of iterations
     * \param double difference between two subsequent iterations
     * \param double relative number of base vectors
     * \param int (0 - random, 1 - PCA (1 dimmension), 2 - according to highest dispersion)
     * \param int predictive calulus time
     * \return void
     *
     */
	void relMds(int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy, int maxCalcTime);
    /** \brief Validate samann parameters
     *
     * \param in dimmension of the projection
     * \param int max number of iterations
     * \param double relative size of raining set
     * \param int neurons in hidden layer
     * \param double learning rate
     * \param int predictive calulus time
     * \return void
     *
     */
	void samann(int d, int maxIteration, double mTrain, int nNeurons, double eta, int p, int maxCalcTime);
    /** \brief Validate somMds parameters
     *
     * \param int number of rows of the resulting matrix
     * \param int number of columns of resulting matix
     * \param int number of MDS iterations
     * \param double difference between two subsequent iterations
     * \param int MDS projection dimmension
     * \param int predictive calulus time
     * \return void
     *
     */
	void somMds(int rows, int columns, int eHat, int mdsIteration, int mdsProjection, double eps, int maxCalcTime);

    ///Classification, grouping components

    /** \brief Validate som parameters
     *
     * \param int number of rows
	 * \param int number of columns
	 * \param int number of learning epoch
	 * \param int number of processors that will be used
     * \param int predictive calulus time
     * \return void
     *
     */
    void som(int rows, int columns, int eHat, int p, int maxCalcTime);
    /** \brief Validate mlp parameters
     *
     * \param int number of neurons in first hidden layer
     * \param int number of neurons in second hidden layer
     * \param double number of folds
     * \param bool if true then cross validation if false then k fold validation
     * \param int number of processors that will be used
     * \param int number of maximum iterations
     * \param int predictive calulus time
     * \return void
     *
     */
	void mlp(int h1pNo, int h2pNo, double qty, bool validationMethod, int p, int maxIteration, int maxCalcTime);
    /** \brief Validate decision forest
     *
     * \param double noise level the bigger the lower noise is
     * \param double relative quantity of the learning objects
     * \param double relative quantity of the testing objects
     * \param int predictive calulus time
     * \return void
     *
     */
	void decForest(double r, double dL, double dT, int maxCalcTime);
    /** \brief Validate kMeans parameters
     *
     * \param int number of max iterations
     * \param  int number of clusters
     * \param int predictive calulus time
     * \return void
     *
     */
	void kMeans(int maxIteration, int kMax, int maxCalcTime);


private:

     InitDamisService *inFile; /**< Hods reference to arff file */

     bool methodParamsValid; /**< Indicates if the parameters are valid */

     /** \brief Validates if the value is in interval
      *
      * \param double value
      * \param double lbound
      * \param double ubound
      * \param std::string value to be printed in log file
      * \param bool lbound = true inclusive
      * \param bool ubound = true inclusive
      * \return bool satisfies condition
      *
      */
     bool validateInterval(double val, double lBound, double uBound, std::string printVal, bool lowerInclusive = true, bool upperInclusive = true);
     /** \brief Validates if value is greather than
      *
      * \param double value
      * \param double lbound
      * \param std::string value to be printed in log file
      * \param bool lbound = true inclusive
      * \param bool inverse = false condition
      * \return bool satisfies condition
      *
      */
	 bool validateGreatherThan(double val, double lBound, std::string printVal, bool lowerInclusive = true, bool inverse = false);
};
#endif //!defined(VALIDATEPARAMS_H)
