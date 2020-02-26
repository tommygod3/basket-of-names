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

            // Member functions no longer use recursion to avoid stack overflow
            Item* lookup(Key);

            bool insert(Key, Item);

            bool remove(Key);

            template < typename Function >
            bool removeIf(Function);

        private:
            Node* root = nullptr;
            static bool isLeaf(Node*);

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
    typename Dictionary<T1, T2>::Item* Dictionary<T1, T2>::lookup(Key key)
    {
        // current is a pointer to the root pointer.
        // This is useful for incrementing our current pointer
        // without changing the data that is in the tree
        // but while retaining a way to change the data if needed
        Node** current = &root;
        while (!isLeaf(*current))
        {
            if ((*current)->key == key)
            {
                return &(*current)->item;
            }
            current = &(*current)->next;
        }
        return nullptr;
    }

    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::insert(Key key, Item item)
    {
        Node** current = &root;
        while (!isLeaf(*current))
        {
            if ((*current)->key == key)
            {
                (*current)->item = item;
                return false;
            }
            current = &(*current)->next;
        }
        *current = new Node(key, item);
        return true;
    }

    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::remove(Key key)
    {
        Node** current = &root;
        while (!isLeaf(*current))
        {
            if ((*current)->key == key)
            {
                Node* next = (*current)->next;
                delete *current;    
                *current = next;
                return true;
            }
            current = &(*current)->next;
        }
        return false;
    }

    template < typename T1, typename T2 >
    bool Dictionary<T1, T2>::isLeaf(Node* node)
    {
        return node == nullptr;
    }

    template < typename T1, typename T2 >
    void Dictionary<T1, T2>::deepDelete(Node* node)
    {
        Node** current = &node;
        while (!isLeaf(*current))
        {
            Node* next = (*current)->next;
            delete *current;
            *current = next;
        }
    }

    template < typename T1, typename T2 >
    typename Dictionary<T1, T2>::Node* Dictionary<T1, T2>::deepCopy(Node* original)//TODO
    {
        if (isLeaf(original))
            return nullptr;

        Node* node = new Node(original->key, original->item);

        Node** current = &original;
        Node** currentNew = &node;
        while (!isLeaf((*current)->next))
        {
            Node* next = (*current)->next;
            (*currentNew)->next = new Node(next->key, next->item);
            current = &(*current)->next;
            currentNew = &(*currentNew)->next;
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
        Node** current = &root;
        bool deleted = false;
        while (!isLeaf(*current))
        {
            if (f((*current)->key))
            {
                Node* next = (*current)->next;
                delete *current;    
                *current = next;
                deleted = true;
            }
            current = &(*current)->next;
        }
        return deleted;
    }

}
