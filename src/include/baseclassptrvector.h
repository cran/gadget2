#ifndef baseclassptrvector_h
#define baseclassptrvector_h

class BaseClass;

/**
 * \class BaseClassPtrVector
 * \brief This class implements a dynamic vector of BaseClass values
 */
class BaseClassPtrVector {
public:
  /**
   * \brief This is the default BaseClassPtrVector constructor
   */
  BaseClassPtrVector() { size = 0; v = 0; };
  /**
   * \brief This is the BaseClassPtrVector constructor that creates a copy of an existing BaseClassPtrVector
   * \param initial is the BaseClassPtrVector to copy
   */
  BaseClassPtrVector(const BaseClassPtrVector& initial);
  /**
   * \brief This is the BaseClassPtrVector destructor
   * \note This will free all the memory allocated to all the elements of the vector
   */
  ~BaseClassPtrVector();
  /**
   * \brief This will add one new entry to the vector
   * \param value is the value that will be entered for the new entry
   */
  void resize(BaseClass* value);
  /**
   * \brief This will delete an entry from the vector
   * \param pos is the element of the vector to be deleted
   * \note This will free the memory allocated to the deleted element of the vector
   */
  void Delete(int pos);
  /**
   * \brief This will return the size of the vector
   * \return the size of the vector
   */
  int Size() const { return size; };
  /**
   * \brief This will return the value of an element of the vector
   * \param pos is the element of the vector to be returned
   * \return the value of the specified element
   */
  BaseClass*& operator [] (int pos) { return v[pos]; };
  /**
   * \brief This will return the value of an element of the vector
   * \param pos is the element of the vector to be returned
   * \return the value of the specified element
   */
  BaseClass* const& operator [] (int pos) const { return v[pos]; };
protected:
  /**
   * \brief This is the vector of BaseClass values
   */
  BaseClass** v;
  /**
   * \brief This is the size of the vector
   */
  int size;
};

#endif
