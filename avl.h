//
// Created by Aleksandra Dmitrieva on 10/18/19.
//
/*avl.h*/


#include <iostream>
#include <algorithm>  // std::max
#include <cstdlib>    // std::abs
#include <stack>
#include <vector>
#include <cassert>

using namespace std;

template<typename TKey, typename TValue>
class avltree
{
private:
    struct NODE
    {
        TKey   Key;
        TValue Value;
        int    Height;
        NODE*  Left;
        NODE*  Right;
    };

    NODE* Root;  // pointer to root node of tree (nullptr if empty)
    int   Size;  // # of nodes in the tree (0 if empty)


    void _inorder(NODE* cur)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder(cur->Left);
            cout << "(" << cur->Key << "," << cur->Value << "," << cur->Height << ")" << " ";
            _inorder(cur->Right);
        }
    }


    void _inorder_keys(NODE* cur, std::vector<TKey>& V)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder_keys(cur->Left, V);
            V.push_back(cur->Key);
            _inorder_keys(cur->Right, V);
        }
    }

    void _inorder_values(NODE* cur, std::vector<TValue>& V)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder_values(cur->Left, V);
            V.push_back(cur->Value);
            _inorder_values(cur->Right, V);
        }
    }

    void _inorder_heights(NODE* cur, std::vector<int>& V)
    {
        if (cur == nullptr)
            return;
        else
        {
            _inorder_heights(cur->Left, V);
            V.push_back(cur->Height);
            _inorder_heights(cur->Right, V);
        }
    }

    //
    // _copytree:
    //
    // Given a tree, inserts the (key, value) pairs into *this* tree.
    //
    void _copytree(NODE* cur)
    {
        if (cur == nullptr)
            return;
        else
        {
            insert(cur->Key, cur->Value);

            _copytree(cur->Left);
            _copytree(cur->Right);
        }
    }

    // _height
    // Helper function to compute NODE heights

    //Return the height of the NODE
    int height(NODE* N)
    {
        return (N == nullptr? -1 : N->Height);
    }

    void rotateHeight(NODE* N)
    {
        int hL = height(N->Left);
        int hR = height(N->Right);
        N->Height = 1+ max (hL, hR);
    }

    //
    // _RightRotate
    // _LeftRotate
    //
    // Rotates the tree around the node N, where Parent is N's parent.
    //
    void _RightRotate(NODE* Parent, NODE* N)
    {
        assert(N != nullptr);  // pre-conditions: N and L must exist
        assert(N->Left != nullptr);

        NODE* L = N->Left;
        NODE* B = L->Right;

        N->Left = B;
        L->Right = N;

        if (Parent == NULL)
            Root = L;
        else if(L->Key < Parent->Key)
            Parent->Left = L;
        else
            Parent->Right = L;


        rotateHeight(N);
        rotateHeight(L);
    }

    void _LeftRotate(NODE* Parent, NODE* N)
    {
        assert(N != nullptr);  // pre-conditions: N and R must exist
        assert(N->Right != nullptr);

        NODE* R = N->Right;
        NODE* B = R->Left;

        N->Right = B;
        R->Left = N;

        if (Parent == NULL)
            Root = R;
        else if (R->Key < Parent->Key )
            Parent->Left = R;
        else
            Parent->Right = R;

        rotateHeight(N);
        rotateHeight(R);
    }

    // Rotate Cases

    int cases( NODE* N, int hL, int hR)
    {
        int hLChild, hRChild;

        //IF tree is LEFT heavy
        if (hL - hR > 1){

            hLChild = height(N->Left->Left);
            hRChild = height(N->Left->Right);

            // Left subtree is LEFT heavy
            if (hLChild > hRChild){
                return 1;
            }else
            {
                return 2;
            }
        }
        //IF tree is Right heavy
        else if ( hR - hL > 1 ) {

            hLChild = height(N->Right->Left);
            hRChild = height(N->Right->Right);

            //Right subtree is RIGHT heavy
            if (hRChild > hLChild) {
                return 4;
            } else {
                return 3;
            }
        }
    }


    void _RotateFix(NODE* Parent, NODE* &N )
    {
        assert(N != nullptr);

        int hL =height(N->Left);
        int hR = height(N->Right);

        assert(std::abs(hL - hR) > 1);
        int Case = cases( N, hL, hR);

        if ( Case == 1 || Case == 2 )
        {
            if (Case == 1)
            {
                _RightRotate(Parent, N);
            }
            else
            {
                _LeftRotate(N, N->Left);
                _RightRotate(Parent, N);
            }
        }
        else
        {
            if (Case == 3)
            {
                _RightRotate(N, N->Right);
                _LeftRotate(Parent, N);
            }
            else
            {
                _LeftRotate(Parent,N);
            }
        }
    }

    // Destructor helper Function:
    void DESTRUCTOR(NODE* cur) {
        if(cur)
        {
            DESTRUCTOR(cur->Left);
            DESTRUCTOR(cur->Right);
            delete cur;
        }
    }

    //_clearNodes Helper function
    void _clearNodes(NODE* node )
    {
        if (node)
        {
            _clearNodes(node->Left);
            _clearNodes(node->Right);
            free(node);
            Root = nullptr;
        }
    }


    // Helper function to find how far from the root is key we are looking for
    int _findkeyLevel(NODE* Root, int key, int level )
    {
        //Check if Root in empty
        if (Root  == nullptr)
            return -1;

        //return level when root is found
        if (Root -> Key == key)
            return level;

        // Go through the tree recursevly and find the key incrementing level when going to deeper children
        int left = _findkeyLevel(Root->Left, key, level +1);
        int right = _findkeyLevel(Root->Right, key, level +1);
        return (left != -1) ? left: right;
    }

public:
    //
    // default constructor:
    //
    // Creates an empty tree.
    //
    avltree()
    {
        Root = nullptr;
        Size = 0;
    }

    //
    // copy constructor:
    //
    avltree(avltree& other)
    {
        Root = nullptr;
        Size = 0;

        _copytree(other.Root);
    }

    // destructor:
    // Frees the memory.

    virtual ~avltree()
    {
        DESTRUCTOR(Root);
    }

    // clear()
    // Clear function to empty the tree and free the memory
    void clear()
    {
        Size = 0;
        _clearNodes(Root);
    }

    //distance(Tkey k1, Tkey k2)
    //Distance functions returns the path from lenght between 2 keys in the tree
    // K1 = K2 returns 0, K1 or K1 not in the tree returns -1;

    // Find the key in the three
    NODE *findNODE(NODE* N, int k1, int k2, int &d1,
                       int &d2, int &dist, int lvl)
    {
        // Base case
        if (N == NULL) return NULL;

        // If either key1 or key2 matches with root's key, report
        // the presence by returning root

        if (N->Key == k1)
        {
            d1 = lvl;
            return N;
        }
        if (N->Key == k2)
        {
            d2 = lvl;
            return N;
        }

        // find Key1 and Key2 in left and right subtrees
        NODE *leftCca  = findNODE(N->Left, k1, k2,
                                       d1, d2, dist, lvl+1);
        NODE *rightCca = findNODE(N->Right, k1, k2,
                                       d1, d2, dist, lvl+1);


        // Closest Common ancestor if one key is present in once subtree and other is
        // present in other

        if (leftCca && rightCca)
        {
            dist = d1 + d2 - 2*lvl;
            return N;
        }

        // Otherwise check if left subtree or right subtree
        // is Closest Common ancestor
        return (leftCca != NULL)? leftCca: rightCca;
    }

    int distance(TKey k1, TKey k2)
    {
        //If entered the same key
        if (k1 == k2){
            return 0;
        }

        int distance;
        int d1 = -1, d2 = -1;

        // Find closest common ancestor
        NODE *cca = findNODE(Root, k1, k2, d1, d2,
                                 distance, 1);
        if (d1 != -1 && d2 != -1)
        {
            return distance;
        }

        // If key 1 is ancestor of key 2 we set Key 1 as a root to find the distance
        if (d1 != -1)
        {
            distance = _findkeyLevel(cca, k2, 0 );
            return distance;
        }
        // If key 2 is ancestor of key 2 we set Key 1 as a root to find the distance

        if (d2 != -1)
        {
            distance = _findkeyLevel(cca, k1, 0);
            return distance;
        }

        return -1;
    }


    // size:
    //
    // Returns the # of nodes in the tree, 0 if empty.
    //
    int size()
    {
        return Size;
    }

    //
    // height
    //
    // Returns the height of tree; the height of an empty tree is
    // defined as -1.
    //
    int height()
    {
        if (Root == nullptr)
            return -1;
        else
            return Root->Height;
    }

    //
    // search:
    //
    // Searches the tree for the given key, returning a pointer to the
    // value if found and nullptr if not.  We return a pointer so the caller
    // may update the underlying value if needed.
    //

    TValue* search(TKey key)
    {
        NODE* cur = Root;
        while (cur != nullptr)
        {
            if (key == cur->Key)  // already in tree
            {
                return &cur->Value;
            }

            if (key < cur->Key)  // search left:
            {
                cur = cur->Left;
            }
            else
            {
                cur = cur->Right;
            }
        }//while

        // if get here, not found
        return nullptr;
    }

    //
    // insert
    //
    // Inserts the given (key, value) pair into the tree; if the key has
    // already been inserted then the function returns without changing
    // the tree.
    //
    void insert(TKey key, TValue value)
    {
        NODE* prev = nullptr;
        NODE* cur = Root;

        //
        // stack the nodes we visit so we can walk back up
        // the search path later, adjusting heights:
        //
        stack<NODE*> nodes;

        //
        // 1. Search to see if tree already contains key:
        //
        while (cur != nullptr)
        {
            if (key == cur->Key)  // already in tree
                return;
            nodes.push(cur);  // stack so we can return later:

            if (key < cur->Key)  // search left:
            {
                prev = cur;
                cur = cur->Left;
            }
            else
            {
                prev = cur;
                cur = cur->Right;
            }
        }//while

        //
        // 2. if we get here, key is not in tree, so allocate
        // a new node to insert:
        //

        NODE* newNode;
        newNode = new NODE();
        newNode->Key = key;
        newNode->Value = value;
        newNode->Height = 0;  // leaf node -> sub-tree of height 0:
        newNode->Left = nullptr;
        newNode->Right = nullptr;

        //
        // 3. link in the new node:
        //
        // NOTE: cur is null, and prev denotes node where
        // we fell out of the tree.  if prev is null, then
        // the tree is empty and the Root pointer needs
        // to be updated.
        //
        if (prev == nullptr)
            Root = newNode;
        else if (key < prev->Key)
            prev->Left = newNode;
        else
            prev->Right = newNode;

        //
        // 4. update size:
        //
        Size++;
        //
        // 5. walk back up tree using stack and update heights.
        //
        while (!nodes.empty())
        {
            cur = nodes.top();
            nodes.pop();

            int hL, hR, hCur;

            hL = height(cur->Left);
            hR = height(cur->Right);

            if (std::abs(hL-hR) > 1 )
            {
                NODE* Parent;
                if (nodes.empty())
                    _RotateFix(nullptr, cur);
                else
                    _RotateFix(nodes.top(), cur);

                break; // We updated all nessesary heights
            }
            else
            {
                hCur = 1 + std::max(hL, hR);
            }

            if (cur->Height == hCur)  // didn't change, so no need to go further:
                break;
            else  // height changed, update and keep going:
                cur->Height = hCur;
        }//while

        //
        // done!
        //
        return;
    }

    //
    // inorder:
    //
    // Performs an inorder traversal of the tree, outputting
    // the keys to the console.
    //
    void inorder()
    {
        cout << "Inorder: ";

        _inorder(Root);

        cout << endl;
    }

    std::vector<TKey> inorder_keys()
    {
        std::vector<TKey>  V;

        _inorder_keys(Root, V);

        return V;
    }

    std::vector<TValue> inorder_values()
    {
        std::vector<TValue>  V;

        _inorder_values(Root, V);

        return V;
    }

    std::vector<int> inorder_heights()
    {
        std::vector<int>  V;

       _inorder_heights(Root, V);

        return V;
    }

};

