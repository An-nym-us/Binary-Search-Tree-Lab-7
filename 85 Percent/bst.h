/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *        ____     _______        __
 *      .' __ '.  |  _____|   _  / /
 *      | (__) |  | |____    (_)/ /
 *      .`____'.  '_.____''.   / / _
 *     | (____) | | \____) |  / / (_)
 *     `.______.'  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair
 
class TestBST;        // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   class set;
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;
   friend class custom::set;
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Access
   //

   const T* find(const T& t) const;
   T* find(const T& t);

   // 
   // Insert
   //

   bool insert(const T&  t, bool keepUnique = false);
   bool insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   void clear() noexcept;
   bool erase(const T& t);
   // 
   // Status
   //
   

   bool   empty() const noexcept { return numElements <=0 ? true: false; }
   size_t size()  const noexcept { return numElements;   }
   
private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode(): pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(0), isRed(NULL)
   {
  
   }
   BNode(const T &  t):pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(t), isRed(NULL)
   {

   }
   BNode(T && t): pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(std::move(t)), isRed(NULL)
   {
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);
   
   void clear(BNode  * &pThis);
   
   void assign(BNode *&pDest, const BNode *pSrc);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return true; }
   bool isLeftChild( BNode * pNode) const { return true; }
//
   BNode* findMinNode();

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST(): numElements(0), root(NULL)
{
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) : numElements(0), root(nullptr)
{
    // Allocate the Node
      *this = rhs;

}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs): numElements(0), root(NULL)
{
   root = rhs.root;
   numElements = rhs.numElements;
   
   rhs.root = nullptr;
   rhs.numElements = 0;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{

}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   root->assign(root, rhs.root);
   numElements = rhs.numElements;
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   clear();
   
   swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   BNode * tempRoot = rhs.root;
   rhs.root = root;
   root = tempRoot;
   
   size_t tempElements = rhs.numElements;
   rhs.numElements = numElements;
   numElements = tempElements;
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
bool BST <T> :: insert(const T & t, bool keepUnique)
{
   BNode * currentNode = root;
   bool found = false;
   while (!found && currentNode != nullptr)
   {
      if (keepUnique == false)
      {
         BNode* newNode = new BNode(t);
         if (newNode->data < currentNode->data)
         {

            currentNode->pLeft = newNode;
            newNode->pParent = root;
            numElements += 1;
            found = true;
            return true;

         }
         else
         {
            currentNode->pRight = newNode;
            newNode->pParent = root;
            numElements += 1;
            found = true;
            return true;
         }
      }

      else
      {
         if (t == currentNode->data)
         {
            found = true;
            return false;
         }
         else if (t < currentNode->data)
         {
            if (currentNode->pLeft != nullptr)
            {
               currentNode = currentNode->pLeft;
            }
         }
         else
         {
            if (currentNode->pRight != nullptr)
            {
               currentNode = currentNode->pRight;
            }
         }
      }


   }
   return false;
}

template <typename T>
bool BST <T> ::insert(T && t, bool keepUnique)
{
   BNode* currentNode = root;
   
   bool found = false;


   while (!found && currentNode != nullptr)
   {
      if (keepUnique == false)
      {
         if (t < currentNode->data)
         {
            BNode* newNode = new BNode(std::move(t));
            currentNode->pLeft = newNode;
            newNode->pParent = currentNode;
            numElements += 1;

            found = true;
            return true;
         }
         else
         {
            BNode* newNode = new BNode(std::move(t));
            currentNode->pRight = newNode;
            newNode->pParent = currentNode;
            numElements += 1;

            found = true;
            return true;
         }
      }
      else
      {
         if (t == currentNode->data)
         {
            found = true;
            return false;
         }
         else if (t < currentNode->data)
         {
            if (currentNode->pLeft != nullptr)
            {
               currentNode = currentNode->pLeft;
            }
         }
         else
         {
            if (currentNode->pRight != nullptr)
            {
               currentNode = currentNode->pRight;
            }
         }
      }

   }
   return false;
}

/****************************************************
 * BST :: FIND
 * Return a pointer to the value if it exists, nullptr otherwise
 ****************************************************/
template <typename T>
const T* BST<T>::find(const T& t) const
{
   BNode * currentNode = root;
   
   while(currentNode != nullptr)
   {
     if(currentNode->data == t)
     {
        return &currentNode->data;
     } else if (t < currentNode->data)
        currentNode = currentNode->pLeft;
      else
         currentNode = currentNode->pRight;
   }
   return nullptr;
}

/****************************************************
 * BST :: FIND
 * Return a pointer to the value if it exists, nullptr otherwise
 ****************************************************/
template <typename T>
T* BST<T>::find(const T& t)
{
   BNode * currentNode = root;
   
   while(currentNode != nullptr)
   {
     if(currentNode->data == t)
     {
        return &currentNode->data;
     } else if (t < currentNode->data)
        currentNode = currentNode->pLeft;
      else
         currentNode = currentNode->pRight;
   }
   return nullptr;
  
}


/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   root->clear(root);
   numElements = 0;
   root= nullptr;
}


/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the value
 ************************************************/
template <typename T>
bool BST <T> ::erase(const T& t)
{
   BNode * currentNode = root;
   
   while(currentNode != nullptr)
   {
      if(currentNode->data == t)
      {
         // Case No Child
         if(currentNode->pRight == nullptr && currentNode->pLeft == nullptr)
         {
            if(currentNode->pParent != nullptr && currentNode->pParent->pRight == currentNode)
            {
               currentNode->pParent->pRight = nullptr;
            }
            
            if(currentNode->pParent != nullptr && currentNode->pParent->pLeft == currentNode)
            {
               currentNode->pParent->pLeft = nullptr;
            }
            numElements--;
            delete currentNode;
            return true;
         }
         
         // Case One Child
      if(currentNode->pRight == NULL && currentNode->pLeft != NULL)
      {
         currentNode->pLeft->pParent = currentNode->pParent;
         
         if(currentNode->pParent != NULL && currentNode->pParent->pRight == currentNode)
         {
            currentNode->pParent->pRight = currentNode->pLeft;
         }
         if(currentNode->pParent != NULL && currentNode->pParent->pLeft == currentNode)
         {
            currentNode->pParent->pLeft = currentNode->pLeft;
         }
         numElements--;
         delete currentNode;
         return true;
      }
         
      if(currentNode->pLeft == NULL && currentNode->pRight != NULL)
         {
            currentNode->pRight->pParent = currentNode->pParent;
            
            if(currentNode->pParent != NULL && currentNode->pParent->pRight == currentNode)
            {
               currentNode->pParent->pRight = currentNode->pRight;
            }
            if(currentNode->pParent != NULL && currentNode->pParent->pLeft == currentNode)
            {
               currentNode->pParent->pLeft = currentNode->pRight;
            }
            numElements--;
            delete currentNode;
            return true;
         }
         
         
         // Case Two Childs
         if(currentNode->pLeft!= nullptr && currentNode->pRight != nullptr)
         {
            BNode * tempNode = currentNode->pRight;
            while(tempNode->pLeft != NULL)
            {
               tempNode = tempNode->pLeft;
            }

            BNode * deleteNode = currentNode;

            if(tempNode->pRight!= NULL)
            {
               tempNode->pRight->pParent = tempNode->pParent;
               tempNode->pParent->pLeft = tempNode->pRight;
            }
            
            currentNode->pParent->pLeft = tempNode;
            currentNode->pParent->pLeft->pParent = deleteNode->pParent;
            // LEFT CHILD
            currentNode->pParent->pLeft->pLeft = deleteNode->pLeft;
            currentNode->pParent->pLeft->pLeft->pParent = currentNode->pParent->pLeft;
            
            // RIGHT CHILD
            
            currentNode->pParent->pLeft->pRight = deleteNode->pRight;
            currentNode->pParent->pLeft->pRight->pParent = currentNode->pParent->pLeft;
            
         
   
            numElements--;
            delete deleteNode;
            return true;
            
         }
  
   
      } else if (t < currentNode->data)
      {
         currentNode = currentNode->pLeft;
      }
      else {
         currentNode= currentNode->pRight;
      }
   }
   return false;
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   pLeft = new BNode(t);
   
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    pLeft = new BNode(std::move(t));
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    pRight = new BNode(t);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    pRight = new BNode(std::move(t));
}

template <typename T>
void BST<T>:: BNode :: clear(BNode *&pThis)
{
   if(pThis== nullptr)
      return;
   
   clear(pThis->pLeft);
   clear(pThis->pRight);
   delete pThis;
   pThis = nullptr;

}

template <typename T>
void BST <T>:: BNode:: assign(BNode * &pDest, const BNode *pSrc)
{
   if(pSrc == nullptr)
   {
      clear(pDest);
      return;
   }
   
   if(pDest== nullptr)
      pDest = new BNode(pSrc->data);
   else
      pDest->data = pSrc->data;
   
   assign(pDest->pLeft, pSrc->pLeft); // L
   if(pSrc->pLeft)
      pDest->pLeft->pParent = pDest;
   
   assign(pDest->pRight, pSrc->pRight); // R
   if(pSrc->pRight)
      pDest->pRight->pParent = pDest;
}


} // namespace custom
//template <typename T>
//BST<T>:: BNode:: findMinNode()
//{
//
//}

