
Class 17 -- SPLAY Trees

35 Splay Trees
Splay trees are Binary_Search_Trees on which ``splaying'' operations are 
performed.
A node in a Binary_Search_Tree is "splayed" when it is moved to the root of
the tree by one or more "rotations".

Whenever a node is accessed (Find, Insert, Remove, etc.), it is splayed, 
thereby making it the root. 

In addition to moving the accessed node to the root, the height of the 
tree may be shortened.

Splay trees guarantee amortized O(m lg n) performance for any long 
sequence of m consecutive operations in a tree having n nodes.

Some operations might be as bad as O(n), but over a long enough 
sequence of operations, the average performance per operation will be O(lg n).

Splay tree rotations have the following names:
    Zig:Rotate the node about its parent (left or right). These are called 
Zig_Left and Zig_Right and are shown in Figure 35.1 below.
The resulting tree has the same height as the original tree.

    Zig_Zig:Rotate the parent about the grandparent (left or right), then 
rotate the node about its parent in the same direction. These are called
Zig_Zig_Left or Zig_Zig_Right and are shown in Figure 35.2.
The resulting tree has the same height as the original tree.

  Zig_Zag:Rotate the node about its parent (left or right), then rotate the 
node about its grandparent in the other direction. These are called 
Zig_Left_Zag_Right and Zig_Right_Zag_Left and are shown in figure 35.3.
The resulting tree has lower height than the orignal tree.

DEFINITION: A Single Rotation is either of the two Zig rotations.

DEFINITION: A Double Rotation is any one of the two Zig Zag or 
any one of the two Zig Zig rotations. 

 The basic idea is to do rotations until the accessed node is at the root.



35.1 Rules for Splaying 

Here are the rules for splaying. Denote the node being accessed as X, the 
parent of X as P and the grandparent of X as G:

Zig: In this case, P is the root of the tree. 
     1. Zig_Left( X ) if X is a left child. 
     2. Zig_Right( X ) if X is a right child.

Zig­Zag: P is not the root of the tree. X and P are opposite types of 
children (left or right). The path from X to G is "crooked".
     1. Zig_Right_Zag_Left if X is a right child and P is a left child. 
     2. Zig_Left_Zag_Right if X is a left child and P is a right child.

Zig­Zig: P is not the root of the tree. X and P are the same type of 
children (left or right). X, P and G are in a straight line.
     1. Zig_Zig_Left if X and P are both right children. 
     2. Zig_Zig_Right if X and P are both left children.


Figure 35.1: Splay Tree Zig Rotations of Node X 



Figure 35.2: Splay Tree Zig_Zig Rotations of Node X 

Figure 35.3: Splay Tree Zig_Zag Rotations of Node X 
35.2 Implementation of Splay Trees




template <class Etype>
class Splay_Node : public Tree_Node<Etype>
{
  protected: 
    Splay_Node * Parent;

    void Single_Rotate(); 
    void Double_Rotate(); 
    void Zig_Left(Splay_Node * const); 
    void Zig_Zig_Left(Splay_Node * const); 
    void Zig_Zig_Right(Splay_Node * const); 
    void Zig_Left_Zag_Right(Splay_Node * const); 
    void Zig_Right_Zag_Left(Splay_Node * const); 
  public: 
    Splay_Node(Etype E = 0, Splay_Node * L = NULL, 
               Splay_Node * R = NULL, Splay_Node * P = NULL) 
         : Tree—Node<Etype> (E,L,R), Parent(P) { }
    friend class Splay_Tree<Etype>; 
};

The Left and Right pointers are inherited from Tree Node.

A pointer to Parent is kept by each Splay Node. This allows the path
along which splaying is to be done to be easily determined. 

Note that the constructor for Splay Node calls the constructor for 
Tree Node in the initialization. 


template <class Etype> 
class Splay_Tree : public Binary_Search_Tree<Etype> 
{
  protected: 
    Splay_Node<Etype> * Copy (Splay_Node<Etype> *); 
    Splay_Node<Etype> * Insert (const Etype &, Splay_Node<Etype> * &); 
    void Remove (const Etype &, Splay_Node<Etype> * &);
    int Splay (Splay_Node<Etype> * const); 

  public: 
    Splay_Tree () : Binary_Search_Tree<Etype> ( )  { }
    const Splay_Tree & operator = (const Splay_Tree &); 
    virtual int Find(const Etype &); 
    virtual int Find_Min(); 
    virtual int Find_Max(); 
    virtual void Insert (const Etype &); 
    virtual void Remove (const Etype &); 
}; 

The Splay Tree constructor constructs the base class Binary_Search_Tree. 

Insert and Remove member functions are overloaded. For each, there is a 
protected version and a public version. The public version calls the protected version. 


35.2.1 The Splay Function

template <class Etype> 
int Splay_Tree<Etype>:: Splay(Splay_Node<Etype> * const Current) 
{
  Splay_Node<Etype> * The_Parent; 
  if (Current == NULL) 
    return 0; 

  while ( (The_Parent = Current­>Parent) != NULL) 
    if (The_Parent­>Parent) 
      Current­>Double_Rotate(); 
    else 
      Current­>Single_Rotate(); 

  return 1; 
}

Splay returns a boolean. It returns 0 if the node to be splayed is NULL, 1 otherwise. 

The reason for this is so that tree operations which return a boolean can 
return the result of the splay. For example, see Find_Min and Find_Max, below.

Splay works up the tree from a node, doing splay single­ or 
double­rotations until the node has no parent (i.e., it's the root). 

If there is a "grandparent", do a double­rotation, otherwise do a single 
rotation. Stop the while loop when the parent of node is NULL. 

Since Root, Left, and Right are inherited from Binary_Search_Tree and 
are Tree_Node instances, they must be cast to Splay Node when used in 
Splay Tree member functions. 



35.2.2 The Find Function

template <class Etype> 
int Splay_Tree<Etype>:: Find (const Etype & X) 
{ 
  Splay_Node<Etype> * Last_On_Path; 
  Splay_Node<Etype> * Current = (Splay_Node<Etype> *) Root; 

  if (Current == NULL) 
    return 0; 

  while (Current != NULL) 
  { 
    Last_On_Path = Current; 
    if (X < Current­>Element) 
      Current = (Splay_Node<Etype> *)(Current­>Left); 
    else if (X > Current­>Element) 
      Current = (Splay_Node<Etype> *)(Current­>Right); 
    else 
    { // item is found 
      Splay(Current); 
      Root = Last_Find = Current; 
      return 1;
    }
  }
  Splay(Last_On_Path); // Not found; still splay, however. 
  Root = Last_On_Path; 
  return 0; 
}

Splaying is done every time a Find operation is done. The node splayed is 
the node found. If no node is found, then the last node on the search path is splayed. 

Last_On_Path is used only when X is not found. It points to the last node 
on the search path. 


35.2.3 The Find_Min and Find_Max Functions 

Find_Min and Find_Max override the corresponding Binary_Search_Tree 
functions. Moreover, they call the Binary_Search_Tree functions. 

Find_Min and Find_Max return the int from Splay. 

There is always a minimum and a maximum node in any non­NULL binary 
search tree. Therefore, 0 is returned only when the tree is empty. 

template <class Etype> 
int Splay_Tree<Etype>::Find_Min() 
{
  Root = Last_Find = Binary_Search_Tree<Etype>::Find_Min(Root); 
  return Splay((Splay_Node<Etype> *)(Root)); 
}


template <class Etype> 
int Splay_Tree<Etype>::Find_Max() 
{
  Root = Last_Find = Binary_Search_Tree<Etype>::Find_Max(Root); 
  return Splay((Splay_Node<Etype> *)(Root)); 
}



35.2.4 The Remove Function 

Every operation on a splay tree causes a node to be splayed. 
The Remove function therefore splays on the node to be removed, before removing it. 

The Remove member functions (one protected, one public) are 
overloaded AND the public version overrides the Binary_Search_Tree version. 

template <class Etype> 
void Splay_Tree<Etype>::Remove(const Etype & X) 
{
  Splay_Tree<Etype>::Remove(X (Splay_Node<Etype> *) Root); 
}

Here is pseudo­code for the protected version of Remove 

template <class Etype> 
void Splay_Tree<Etype>::Remove(const Etype & X, Splay_node<Etype> * & T) 
{
  Splay the node with Element of X. Do this by "Find"ing it.

  Let TL and TR be the left­ and right­subtrees of the X node. 
  If TL is not NULL 
  {
    Find the max node in TL, splay it to the root of the subtree. 
    Reorganize the tree, making TL-max the root. 
    Make TR be the right subtree of TL-max.
  } 
  else
    Make TR the root

  Delete the X node.
}

