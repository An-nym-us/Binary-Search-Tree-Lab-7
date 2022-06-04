/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Jonathan Gunderson and Sulav Dahal
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
#include <stdio.h>
class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
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

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
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
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

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
   bool isRightChild(BNode * pNode) const {
      if(pNode->pRight == nullptr && pNode->pParent->pRight == pNode)
         return true;
      
      return false;
   }
   bool isLeftChild( BNode * pNode) const {
      if(pNode->pLeft == nullptr && pNode->pParent->pLeft == pNode)
         return true;
      
      return false;
   }

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set; 
public:
   // constructors and assignment
   iterator(BNode * p = nullptr) : pNode(p)
   {

   }
   iterator(const iterator & rhs): pNode(nullptr)
   {
      *this = rhs;
   }
   iterator & operator = (const iterator & rhs)
   {
      pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      iterator old = *this;
      
//      if(pNode->pLeft != NULL)
//         pNode = pNode-> pLeft;
      
      return old;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      return *this;;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
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
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   BNode* currentNode = root;
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
            std::pair<iterator, bool> pairReturn( newNode, true);
            return pairReturn;

         }
         else
         {
            currentNode->pRight = newNode;
            newNode->pParent = root;
            numElements += 1;
            found = true;
            std::pair<iterator, bool> pairReturn( newNode, true);
            return pairReturn;
         }
      }
      else
      {
         if (t == currentNode->data)
         {
            found = true;
            std::pair<iterator, bool> pairReturn( currentNode, false);
            return pairReturn;
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
   
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
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
            newNode->pParent = root;
            numElements += 1;
            found = true;
            std::pair<iterator, bool> pairReturn( newNode, true);
            return pairReturn;

         }
         else
         {
            BNode* newNode = new BNode(std::move(t));
            currentNode->pRight = newNode;
            newNode->pParent = currentNode;
            numElements += 1;
            found = true;
            std::pair<iterator, bool> pairReturn( newNode, true);
            return pairReturn;
         }
      }
      else
      {
         if (t == currentNode->data)
         {
            found = true;
            std::pair<iterator, bool> pairReturn( currentNode, false);
            return pairReturn;
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
   
   std::pair<iterator, bool> pairReturn(end(), false);
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{
   
   if(it.pNode == nullptr)
      return end();
   // Case No Child
   if(it.pNode->pRight == NULL && it.pNode->pLeft == NULL)
      {
         BNode * temp = nullptr;
         if(it.pNode->pParent != NULL && it.pNode->pParent->pRight == it.pNode)
         {
            it.pNode->pParent->pRight = NULL;
         }
         if(it.pNode->pParent != NULL && it.pNode->pParent->pLeft == it.pNode)
         {
            ++it;
            temp = it.pNode->pLeft;
            it.pNode->pLeft = NULL;
         }
         numElements--;
         delete temp;
         return it;
   }
   // Case One Child
   if(it.pNode->pRight == NULL && it.pNode->pLeft != NULL)
   {
      BNode * temp = nullptr;
      it.pNode->pLeft->pParent = it.pNode->pParent;
      
      if(it.pNode->pParent != NULL && it.pNode->pParent->pRight == it.pNode)
      {
         temp = it.pNode;
         it.pNode->pParent->pRight = it.pNode->pLeft;
      }
      
      if(it.pNode->pParent != NULL && it.pNode->pParent->pLeft == it.pNode)
      {
         temp = it.pNode;
         it.pNode->pParent->pLeft = it.pNode->pLeft;
      }
      
      ++it;
      numElements--;
      delete temp;
      return it;
   }
   // has Right Child
   if(it.pNode->pLeft == NULL && it.pNode->pRight != NULL)
   {
      BNode * temp = nullptr;
      it.pNode->pRight->pParent = it.pNode->pParent;
      
      if(it.pNode->pParent != NULL && it.pNode->pParent->pRight == it.pNode)
      {
         temp = it.pNode;
         it.pNode->pParent->pRight = it.pNode->pRight;
      }
      
      if(it.pNode->pParent != NULL && it.pNode->pParent->pLeft == it.pNode)
      {
         temp = it.pNode;
         it.pNode->pParent->pLeft = it.pNode->pRight;
      }
      
      ++it;
      numElements--;
      delete temp;
      return it;
   }

   
   // Case TWO CHILD
   
   if(it.pNode->pLeft != NULL && it.pNode->pRight != NULL)
   {
      BNode * tempNode = it.pNode->pRight;
      while(tempNode->pLeft != NULL)
      {
         tempNode = tempNode->pLeft;
      }

      BNode * deleteNode = it.pNode;

      if(tempNode->pRight!= NULL)
      {
         tempNode->pRight->pParent = tempNode->pParent;
         tempNode->pParent->pLeft = tempNode->pRight;
      }
      
      it.pNode->pParent->pLeft = tempNode;
      it.pNode->pParent->pLeft->pParent = deleteNode->pParent;
      
      // LEFT CHILD
      it.pNode->pParent->pLeft->pLeft = deleteNode->pLeft;
      it.pNode->pParent->pLeft->pLeft->pParent = it.pNode->pParent->pLeft;
      
      // RIGHT CHILD
      
      it.pNode->pParent->pLeft->pRight = deleteNode->pRight;
      it.pNode->pParent->pLeft->pRight->pParent = it.pNode->pParent->pLeft;
      
   

      numElements--;
      ++it;
      delete deleteNode;
      if(it.pNode>= tempNode)
        it.pNode = tempNode;
      
      return it;
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

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   if(empty())
      return end();
   BNode * currentNode = root;
   
   while(currentNode->pLeft)
   {
      currentNode = currentNode->pLeft;
   }
   return iterator(currentNode);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   BNode * currentNode = root;
   
   while(currentNode != NULL)
   {
      if(currentNode->data == t)
         return iterator(currentNode);
      
      else if( t < currentNode->data)
         currentNode = currentNode->pLeft;
      else
         currentNode = currentNode->pRight;
   }
   return end();
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
   pLeft= pNode;
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



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
//   if(pNode->isRightChild(pNode))
//   {
//      BNode *
//   }
   if(pNode == NULL)
   {
      pNode = nullptr;
   }

   else if(pNode->pRight != NULL)
   {
      pNode = pNode->pRight;
      while(pNode->pLeft)
      {
         pNode = pNode->pLeft;
      }
 
   }
   
   else if(pNode->pRight == NULL && pNode->pParent->pLeft == pNode)
      pNode = pNode->pParent;
   
   else if(pNode->pRight == NULL && pNode->pParent->pRight == pNode)
   {
      while(pNode->pParent && pNode->pParent->pRight == pNode)
      {
         pNode = pNode->pParent;
      }
    
         pNode = pNode->pParent;
   }
   else {
      pNode = nullptr;
   }
      
   return *this;  
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   return *this;

}


} // namespace custom


