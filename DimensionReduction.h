///////////////////////////////////////////////////////////
//  DimensionReduction.h
//  Implementation of the Class DimensionReduction
//  Created on:      05-Vas-2014 17:36:47
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(DIMENSIONREDUCTION_H)
#define DIMENSIONREDUCTION_H

#include "CallCalculus.h"

 /** \brief Class that implements constructing parameter set for dimmension reduction cluster algoritms
  */
 class DimensionReduction : public CallCalculus
{

public:
    /** \brief Class constructor.
     *
     * \param int number of processors.
     * \param int predictive calculation time in seconds.
     * \param InitDamisService* pointer to the InitDamisService object.
     *
     */
	DimensionReduction(int p, int maxCalcTime, InitDamisService* initFile);
    /** \brief Class destructor
     */
	virtual ~DimensionReduction();
    /** \brief Runs PCA
     *
     * \param bool true means dimmension false - relative disperssion
     * \param double  either dimmension or dispersion
     * \return void
     *
     */
	void runPCA(bool projType, double d);
    /** \brief Runs SMACOFMDS
     *
     * \param int dimmension of the projection
     * \param int max number of iterations
     * \param double difference between two subsequent iterations
     * \param bool apply Zeidel modification
     * \return void
     *
     */
	void runSMACOFMDS(int d, int maxIteration, double eps, bool zeidel);
    /** \brief Runs DMA
     *
     * \param int dimmension of the projection
     * \param int max number of iterations
     * \param double difference between two subsequent iterations
     * \param double relative number of neighbours
     * \return void
     *
     */
	void runDMA(int d, int maxIteration, double eps, double neighbour);
    /** \brief Runs RELATIVEMDS
     *
     * \param int dimmension of the projection
     * \param int max number of iterations
     * \param double difference between two subsequent iterations
     * \param double relative number of base vectors
     * \param int (0 - random, 1 - PCA (1 dimmension), 2 - according to highest dispersion)
     * \return void
     *
     */
	void runRELATIVEMDS(int d, int maxIteration, double eps, double noOfBaseVectors, int selStrategy);
    /** \brief RunsSAMMAN
     *
     * \param int dimmension of the projection
     * \param int max number of iterations
     * \param double relative size of raining set
     * \param int neurons in hidden layer
     * \param double learning rate
     * \return void
     *
     */
	void runSAMANN(int d, int maxIteration, double mTrain, int nNeurons, double eta);
    /** \brief Runs SOMMDS
     *
     * \param int number of rows of the resulting matrix
     * \param int number of columns of resulting matix
     * \param int number of MDS iterations
     * \param double difference between two subsequent iterations
     * \param int MDS projection dimmension
     * \return void
     *
     */
	void runSOMMDS(int rows, int columns, int eHat, int mdsIteration, double eps, int mdsProjection);
};
#endif //!defined(DIMENSIONREDUCTION_H)
