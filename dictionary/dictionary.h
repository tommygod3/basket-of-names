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
    };

    template < typename T1, typename T2 >
    std::ostream & operator<<(std::ostream & os, const Dictionary<T1, T2> & Dictionary);

    template < typename T1, typename T2 >
    struct Dictionary<T1, T2>::Node
    {
        Key key;
        Item item;
        Node* next = nullptr;

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
        else
        {
            return insertRec(key, item, current->next);
        }
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
        else if (node->key == key)
        {
            return &node->item;
        }
        else
        {
            return lookupRec(key, node->next);
        }
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

        else if (current->key == key)
        {
            delete current;
            current = nullptr;
            return true;
        }
        else
        {
            return removeRec(key, current->next);
        }
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::displayEntriesRec(Node* node, std::ostream & os)
    {
        if (isLeaf(node))
            return;
        displayEntriesRec(node->next, os);
        os << node->key << ' ' << node->item << '\n';
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
        if (not isLeaf(node->next))
        {
            deepDelete(node->next);
        }
        delete node;
    }

    template < typename T1, typename T2 >
    typename Dictionary<T1, T2>::Node* Dictionary<T1, T2>::deepCopy(Node* original)
    {
        if (isLeaf(original))
            return nullptr;

        Node* node = new Node(original->key, original->item);
        if (not isLeaf(original->next))
        {
            node->next = deepCopy(original->next);
        }
        return node;
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
