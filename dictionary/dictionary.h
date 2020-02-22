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

            bool remove(Key);

            template < typename Function >
            bool removeIf(Function);

        private:
            Node* root = nullptr;
            static bool isLeaf(Node*);

            static Item* lookupRec(Key, Node*);
            static bool insertRec(Key, Item, Node* &);

            static bool removeRec(Key, Node* &);

            template < typename Function >
            bool removeIfRec(Node* & current, Function f);

            static void deepDelete(Node*);

            static Node* deepCopy(Node*);
    };

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
            Node* next = current->next;
            delete current;
            current = next;
            return true;
        }
        else
        {
            return removeRec(key, current->next);
        }
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
    template < typename Function >
    bool Dictionary<T1, T2>::removeIf(Function f)
    {
        return removeIfRec(root, f);
    }

    template < typename T1, typename T2 >
    template < typename Function >
    bool Dictionary<T1, T2>::removeIfRec(Node* & current, Function f)
    {
        // Key not in Tree
        if (isLeaf(current))
            return false;

        bool nextDeleted = removeIfRec(current->next, f);
        if (f(current->key))
        {
            return remove(current->key);
        }
        return nextDeleted;
    }

}
