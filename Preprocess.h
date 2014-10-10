///////////////////////////////////////////////////////////
//  Preprocess.h
//  Implementation of the Class Preprocess
//  Created on:      05-Vas-2014 17:36:45
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined(PREPROCESS_H)
#define PREPROCESS_H

#include "ServeRequest.h"
#include <string>

 /** \brief Implements all components described in Preprocessing gropup
  */
class Preprocess : public ServeRequest
{

public:
    /** \brief Constructor
     *
     * \param InitDamisService* object which data mus be preprocessed
     */

	Preprocess(InitDamisService*);
     /** \brief Normalize data either by mean and std or by mapping to interval
      *
      * \param bool = true then normalize by mean and std, else map to interval
      * \param double either mean or lbound value
      * \param double either std of ubound value
      * \return void
      *
      */
	void normData(bool, double, double);
     /** \brief Function that performs data transpose
      * \return void
      *
      */
	void transposeData();
     /** \brief Function that performs data cleaning
      * \return void
      *
      */
	void cleanData();
     /** \brief Function that performs data splitting
      *
      * \param bool perform shufling
      * \param double relative number of object in first set
      * \param double relative number of objects in second set
      * \return void
      *
      */
	void splitData(bool, double, double);
     /** \brief Function that performs data filtering
      *
      * \param  bool data part to return outliers or not
      * \param double z value
      * \param int attribute index acording to wich perform data filtering
      * \return  void
      *
      */
	void filterData(bool, double, int);

    DamisFile *altOutFile;/**< Variable that holds alternative output path if needed (by default NULL) Only for methods that return two output files of data*/

    /** \brief Destructor
     */
	virtual ~Preprocess();

private:

    std::vector<std::string> writeClass; /**< Variable that holds object classes to be written in output file */

};
#endif //!defined(PREPROCESS_H)
