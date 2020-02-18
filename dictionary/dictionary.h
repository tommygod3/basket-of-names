#include <iostream>
#include <cassert>

namespace Containers
{
    template < typename T1, typename T2 >
    class Dictionary
    {
        public:
            using Key = T1;
            using Item = T2;
        private:
            struct Node;

        template < typename TF1, typename TF2 >
        friend std::ostream & operator<<(std::ostream & os, const Dictionary<TF1, TF2> & Dictionary);

        public:
            // Default Constructor
            Dictionary() = default;
            // Copy Constructor
            Dictionary(const Dictionary<T1, T2> &);
            // Copy Assignment Operator
            Dictionary<T1, T2> & operator=(const Dictionary<T1, T2> &);
            // Move Constructor
            Dictionary(Dictionary<T1, T2> &&);
            // Move Assignment Operator
            Dictionary<T1, T2> & operator=(Dictionary<T1, T2> &&);
            // Destructor
            ~Dictionary();

            Item* lookup(Key);

            bool insert(Key, Item);

            void displayTree();

            bool remove(Key);

        private:
            Node* root = nullptr;
            static bool isLeaf(Node*);

            static Item* lookupRec(Key, Node*);
            static bool insertRec(Key, Item, Node* &);

            static void displayEntriesRec(Node* node, std::ostream & os);
            void displayTreeRec(Node* node, int depth = 0);

            static bool removeRec(Key, Node* &);

            static Node* detachMinimumNode(Node* &);

            static void deepDelete(Node*);

            static Node* deepCopy(Node*);

            // The Node argument here must be an internal node and so must it's left child
            static void rotateRight(Node* &);
            // The Node argument here must be an internal node and so must it's right child
            static void rotateLeft(Node* &);
    };

    template < typename T1, typename T2 >
    std::ostream & operator<<(std::ostream & os, const Dictionary<T1, T2> & Dictionary);

    template < typename T1, typename T2 >
    struct Dictionary<T1, T2>::Node
    {
        Key key;
        Item item;
        Node* leftChild = nullptr;
        Node* rightChild = nullptr;

        Node(Key keyIn, Item itemIn)
        {
            key = keyIn;
            item = itemIn;
        }
    };

    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::insert(Key key, Item item)
    {
        return insertRec(key, item, root);
    }

    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::insertRec(Key key, Item item, Node* & current)
    {
        if (isLeaf(current))
        {
            current = new Node(key, item);
            return true;
        }
        else if (current->key == key)
        {
            current->item = item;
            return false;
        }
        else if (key < current->key)
        {
            return insertRec(key, item, current->leftChild);
        }
        else if (key > current-> key)
        {
            return insertRec(key, item, current->rightChild);
        }
        assert(false);
    }

    template < typename T1, typename T2 >
    typename Dictionary<T1, T2>::Item* Dictionary<T1, T2>::lookup(Key key)
    {
        return lookupRec(key, root);
    }

    template < typename T1, typename T2 >
    typename Dictionary<T1, T2>::Item* Dictionary<T1, T2>::lookupRec(Key key, Node* node)
    {
        if (isLeaf(node))
        {
            return nullptr;
        }
        if (node->key == key)
        {
            return &node->item;
        }
        if (key < node->key)
        {
            return lookupRec(key, node->leftChild);
        }
        else if (key > node->key)
        {
            return lookupRec(key, node->rightChild);
        }
        assert(false);
    }

    // TODO: Would it not be easier to make a "lookupNode" function which lookups and returns whole node?
    // It would simplify a few functions.
    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::remove(Key key)
    {
        return removeRec(key, root);
    }

    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::removeRec(Key key, Node* & current)
    {
        // Key not in Tree
        if (isLeaf(current))
            return false;

        if (current->key == key)
        {
            if (isLeaf(current->leftChild) and isLeaf(current->rightChild))
            {
                delete current;
                current = nullptr;
            }
            else if (isLeaf(current->leftChild) != isLeaf(current->rightChild))
            {
                Node* child;
                if (current->leftChild)
                {
                    child = current->leftChild;
                }
                else
                {
                    child = current->rightChild;
                }
                
                delete current;
                current = child;
            }
            else if (current->leftChild and current->rightChild)
            {
                Dictionary<T1, T2>::Node* newNode = detachMinimumNode(current->rightChild);
                newNode->leftChild = current->leftChild;
                newNode->rightChild = current->rightChild;

                delete current;
                current = newNode;
            }
            return true;
        }
        else if (key < current->key)
        {
            return removeRec(key, current->leftChild);
        }
        else if (key > current-> key)
        {
            return removeRec(key, current->rightChild);
        }
        assert(false);
    }

    template < typename T1, typename T2 >
    typename Dictionary<T1, T2>::Node* Dictionary<T1, T2>::detachMinimumNode(Node* & node)
    {
        if (isLeaf(node->leftChild))
        {
            Dictionary<T1, T2>::Node* nodeCopy = new Node(node->key, node->item);
            removeRec(node->key, node);
            return nodeCopy;
        }
        else
        {
            return detachMinimumNode(node->leftChild);
        }
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::displayEntriesRec(Node* node, std::ostream & os)
    {
        if (isLeaf(node))
            return;
        displayEntriesRec(node->leftChild, os);
        os << node->key << ' ' << node->item << '\n';
        displayEntriesRec(node->rightChild, os);
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::displayTree()
    {
        displayTreeRec(root);
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::displayTreeRec(Node* node, int depth)
    {
        if (isLeaf(node))
        {
            std::cout << std::string(depth, '.') << '*' << '\n';
            return;
        }
        std::cout << std::string(depth, '.') << node->item << '\n';
        int nextDepth = ++depth;
        displayTreeRec(node->leftChild, nextDepth);
        displayTreeRec(node->rightChild, nextDepth);
    }

    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::isLeaf(Node* node)
    {
        return node == nullptr;
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::deepDelete(Node* node)
    {
        if (isLeaf(node))
            delete node;
            return;
        if (not isLeaf(node->leftChild))
        {
            deepDelete(node->leftChild);
        }
        if (not isLeaf(node->rightChild))
        {
            deepDelete(node->rightChild);
        }
        delete node;
    }

    template < typename T1, typename T2 >
    typename Dictionary<T1, T2>::Node* Dictionary<T1, T2>::deepCopy(Node* original)
    {
        if (isLeaf(original))
            return nullptr;

        Node* node = new Node(original->key, original->item);
        if (not isLeaf(original->leftChild))
        {
            node->leftChild = deepCopy(original->leftChild);
        }
        if (not isLeaf(original->rightChild))
        {
            node->rightChild = deepCopy(original->rightChild);
        }
        return node;
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::rotateRight(Node* & localRoot)
    {
        Node* b = localRoot;
        assert(not isLeaf(b));
        Node* a = b->leftChild;
        assert(not isLeaf(a));
        Node * beta = a->rightChild;

        localRoot = a;
        a->rightChild = b;
        b->leftChild = beta;
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::rotateLeft(Node* & localRoot)
    {
        Node* a = localRoot;
        assert(not isLeaf(a));
        Node* b = a->rightChild;
        assert(not isLeaf(b));
        Node * beta = b->leftChild;

        localRoot = b;
        b->leftChild = a;
        a->rightChild = beta;
    }

    template < typename T1, typename T2 >
    Dictionary<T1, T2>::Dictionary(const Dictionary & original)
    {
        this->root = deepCopy(original.root);
    }

    template < typename T1, typename T2 >
    Dictionary<T1, T2> & Dictionary<T1, T2>::operator=(const Dictionary & original)
    {
        if (this == &original)
            return *this;
        deepDelete(this->root);
        this->root = deepCopy(original.root);
        return *this;
    }

    template < typename T1, typename T2 >
    Dictionary<T1, T2>::Dictionary(Dictionary<T1, T2> && original)
    {
        this->root = original.root;
        original.root = nullptr;
    }

    template < typename T1, typename T2 >
    Dictionary<T1, T2> & Dictionary<T1, T2>::operator=(Dictionary<T1, T2> && original)
    {
        if (this == &original)
            return *this;
        deepDelete(this->root);
        this->root = original.root;
        original.root = nullptr;
        return *this;
    }

    template < typename T1, typename T2 >
    Dictionary<T1, T2>::~Dictionary()
    {
        deepDelete(root);
    }

    template < typename T1, typename T2 >
    std::ostream & operator<<(std::ostream & os, const Dictionary<T1, T2> & Dictionary)
    {
        Dictionary.displayEntriesRec(Dictionary.root, os);
        return os;
    }
}
