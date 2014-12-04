///////////////////////////////////////////////////////////
//  ArffFile.h
//  Implementation of the Class ArffFile
//  Created on:      30-Sau-2014 17:06:17
//  Original author: Povilas
///////////////////////////////////////////////////////////

#if !defined (ARFFFILE_H)
#define ARFFFILE_H

#include "DamisFile.h"
#include <vector>
#include <string>


 /** \brief ARFF file parsing class.
  *
  * ArffFile class is inherited from the DamisFile class.
  */

class ArffFile : public DamisFile
{

public:

     /** \brief ArffFile constructor.
      *
      * \param pref std::string Parameter indicates the file name prefix value.
      *
      */
	ArffFile(std::string pref);

    /** \brief Virtual destructor.
     *
     */
	virtual ~ArffFile();
     /** \brief Function returns vector of attribute names (from the attribute section class excluded).
      *
      * \return std::vector<std::string>
      *
      */
	std::vector<std::string> getAttributeName();
     /** \brief Function returns vector of attribute types (from the attribute section class excluded).
      *
      * \return std::vector<std::string>
      *
      */
	std::vector<std::string> getAttributeType();
	 /** \brief Function returns vector of possible classes that object may have (from the attribute section).
	 *
	 * \return std::vector<std::string>
	 */
	std::vector<std::string> getStringClass();
	 /** \brief Function returns vector of object's class values in string (from data section).
	 *
	 * \return std::vector<std::string>
	 */
	std::vector<std::string> getStringClassAttribute();
	 /** \brief Function returns vector of object's class values in int (from the data section).
	 *
	 * \return std::vector<int>
	 */
	std::vector<int> getIntClass();
    /** \brief Function returns vector of object's class values in int (from attribute section).
	 *
	 * \return std::vector<int>
	 */
	std::vector<int> getIntClassAttribute();
     /** \brief Function returns data form data section (string format without class).
	 *
	 * \return std::vector<std::vector<double>>
	 */
    std::vector<std::vector<double>> getDoubleData();
     /** \brief Function returns data in data section (string format without class).
	 *
	 * \return std::vector<std::vector<std::string>>
	 */
	std::vector<std::vector<std::string>> getStringData();
     /** \brief Function returns class attribute index.
	 *
	 * \return int
	 */
	 int getClassAttributeIndex();
	 /** \brief Function returns number of attributes (without class).
	 *
	 * \return int
	 */
     int getNumberOfAttributes();
     /** \brief Function returns number of rows/objects (without class).
	 *
	 * \return int
	 */
     int getNumberOfObjects();
	 /** \brief Function indicates if the class section is found in arff file.
	 *
	 * \return bool
	 */
	bool isClassFound();
    /** \brief Function retruns i,j th data value from the arff file data section.
     *
     * \param int i Parameter indicates the column.
     * \param int j Parameter indicates the row.
     * \return double
     *
     */
	double getDoubleDataAt(int i, int j);

protected:
    /** \brief Function reads and validates arff file. If file is valid reurns true.
	 *
	 * \return bool
	 */
	bool readArffFile();

private:

	std::vector<std::string> attributeName; /**< Attribute that holds attribute names of the arff file. */

	std::vector<std::string> attributeType;/**< Attribute that holds attribute types. */

	std::vector<int> attributeIntClass; /**< Attribute that holds object class names in int format (from attribute section). */

	std::vector<std::string> attributeStringClass; /**< Attribute that holds object class names in string format (from attribute section). */

	std::vector<int> objIntClass; /**< Attribute that holds object class names in int format. */

	std::vector<std::string> objStringClass; /**< Attribute that holds object class names in string format. */

    bool classFound; /**< Is class attribute found. */

	int classAttributeIndex; /**< ttribute that holds class index. */

    int noOfAtt; /**< Number of features in data section of the arff file. */

    int noOfObj; /**< Number of objects in data section of the arff file. */

	std::vector<std::vector<std::string>> stringData; /**< Attribute that holds raw data (without class) in string format. */

	std::vector<std::vector<double>> doubleData; /**< Attribute that holds raw data (without class) in double format. */
};
#endif //!defined (ARFFFILE_H)
