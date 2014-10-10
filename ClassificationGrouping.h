///////////////////////////////////////////////////////////
//  ClassificationGrouping.h
//  Implementation of the Class ClassificationGrouping
//  Created on:      05-Vas-2014 17:36:50
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(CLASSIFICATIONGROUPING_H)
#define CLASSIFICATIONGROUPING_H

#include "CallCalculus.h"

/** \brief Class that implements classification and grouping cluster algorithms.
 *
 */
class ClassificationGrouping : public CallCalculus
{

public:
    /** \brief Class constructor
	*
	* \param int number of procesors to be used by the calculus
    * \param int predictive time used by the calulus
    * \param InitDamisService* pointer to the InitDamisService object
	*/
	ClassificationGrouping(int p, int maxCalcTime, InitDamisService* initFile);
    /** \brief Class destructor
	*
	*/
	virtual ~ClassificationGrouping();
    /** \brief Method runs SOM
	*
	* \param int number of rows
	* \param int number of columns
	* \param int number of learning epoch
	* \return void
	*/
	void runSOM(int rows, int columns, int eHat);
    /**
	*Method runs MLP
	*/
    /** \brief Method runs MLP
     *
     * \param int number of neurons in first hidden layer
     * \param int number of neurons in second hidden layer
     * \param double number of folds
     * \param int number of maximum iterations
     * \param bool if true then cross validation if false then k fold validation
     * \return void
     *
     */
	void runMLP(int h1pNo, int h2pNo, double qty, int maxIteration, bool validationMethod);

    /** \brief Runs decision forest
     *
     * \param double noise level the bigger the lower noise is
     * \param double relative quantity of the learning objects
     * \param double relative quantity of the testing objects
     * \param int number of trees in the forest
     * \return void
     *
     */
	void runDecForest(double q, double dL, double dT, int nTree);

    /** \brief Runs k-Means clustering
     *
     * \param int number of max iterations
     * \param  int number of clusters
     * \return void
     *
     */
	void runKMEANS(int maxIteration, int kMax);
};
#endif //!defined(CLASSIFICATIONGROUPING_H)
