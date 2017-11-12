#ifndef __GENERAL_TREE_HPP__
#define __GENERAL_TREE_HPP__

#include <vector>
#include <deque>

/*

    template <typename T>
    class GeneralTree
    {
        struct Node;
        class Iterator;

        Node* _root;
        int _size;
    public:
        typedef T value_type;
        typedef Iterator iterator;
        typedef std::size_t size_type;

        GeneralTree();
        GeneralTree(const T& );
        GeneralTree(T&& );
        GeneralTree(const GeneralTree& );
        ~GeneralTree();
        GeneralTree& operator=(const GeneralTree& );
        Iterator insert(const T& , Iterator , std::size_t );
        Iterator root();
        Iterator begin();
        Iterator end();
        Iterator getChild(Iterator , std::size_t );
        int getNumberOfChildren(Iterator );
        void erase(Iterator );
        void clear(Iterator );
        bool empty();
        std::size_t size();
    };
 */

namespace rcn {

template<typename T>
class GeneralTree
{
    struct Node
    {
        T value;
        Node* parent;
        std::vector<Node*> children;

        explicit Node(T val = T(), Node* par = nullptr) : value(val), parent(par) {}
    };

    class Iterator
    {
        friend class GeneralTree;

        Node* ptr;
    public:
        explicit Iterator(Node* n = nullptr) : ptr(n) {}

        T& operator*()
        {
            return ptr->value;
        }

        Iterator& operator++()
        {
            std::vector<Node*>& vec = ptr->parent->children;
            unsigned long vecsize = ptr->parent != nullptr ? vec.size() : 0;
            unsigned long index;
            for(index = 0; index < vecsize; index++)
            {
                if(vec[index] == ptr)
                {
                    index++;
                    break;
                }
            }
            if(index < vecsize)
            {
                ptr = ptr->parent->children[index];
                while(!ptr->children.empty())
                {
                    ptr = ptr->children[0];
                }
            }
            else
            {
                ptr = ptr->parent;
            }
            return *this;
        }

        T* operator->() const
        {
            return &ptr->value;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            this->operator++();
            return temp;
        }

        bool operator==(Iterator it) const
        {
            return this->ptr == it.ptr;
        }

        bool operator!=(Iterator it) const
        {
            return this->ptr != it.ptr;
        }
    };
    void delete_node(Node* node)
    {
        if (node == nullptr) { return; }
        std::deque<Node*> stack;
        Node* current = node;
        if (node == _root)
        {
            _root = nullptr;
        }
        stack.push_back(current);
        while (!stack.empty())
        {
            if (!current->children.empty())
            {
                current = current->children.back();
                stack.push_back(current);
            }
            else
            {
                delete current;
                --_size;
                stack.pop_back();
                if (!stack.empty())
                {
                    current = stack.back();
                    current->children.pop_back();
                }
            }
        }
    }
    void copy_node(T val, Node* other, Node* parent)
    {
        auto* temp = new Node(val, parent);
        parent->children.push_back(temp);
        for (Node* n : other->children)
        {
            copy_node(n->value, n, temp);
        }
    }

    Node* _root;
    std::size_t _size;
public:
    typedef T           value_type;
    typedef Iterator    iterator;
    typedef std::size_t size_type;

    GeneralTree() : _root(nullptr), _size(0) {}

    explicit GeneralTree(const T& value) : _root(new Node(value)), _size(1) {}

    GeneralTree(const GeneralTree& tree) : _root(nullptr), _size(tree._size)
    {
        if(tree._root == nullptr) return;
        this->_root = new Node(tree._root->value, nullptr);
        for(Node* n : tree._root->children)
        {
            copy_node(n->value, n, this->_root);
        }
    }

    GeneralTree(GeneralTree&& tree) noexcept : _root(tree._root), _size(tree._size)
    {
        if(tree._root == nullptr) return;
        this->_root = tree._root;
        tree._root = nullptr;
        tree._size = 0;
    }

    ~GeneralTree()
    {
        this->clear();
    }

    GeneralTree& operator=(const GeneralTree& tree)
    {
        this->clear();
        if(tree._root == nullptr) return *this;
        this->_size = tree._size;
        this->_root = new Node(tree._root->value, nullptr);
        for(Node* n : tree._root->children)
        {
            copy_node(n->value, n, this->_root);
        }

        return *this;
    }

    GeneralTree& operator=(GeneralTree&& tree) noexcept
    {
        this->_root = tree._root;
        this->_size = tree._size;
        tree._root = nullptr;
        tree._size = 0;

        return *this;
    }

    Iterator insert(const T& value, Iterator it_parent, std::size_t index)
    {
        _size++;
        if (it_parent.ptr == nullptr && index == 0 && _root == nullptr)
        {
            _root = new Node(value);
            return Iterator(_root);
        }
        auto* node = new Node(value, it_parent.ptr);
        if (index >= it_parent.ptr->children.size())
        {
            it_parent.ptr->children.push_back(node);
            return Iterator(node);
        }
        std::vector<Node*>* children = &(it_parent.ptr->children);
        children->insert(children->begin() + index, node);
        return Iterator(node);
    }

    Iterator root() const
    {
        return Iterator(_root);
    }

    Iterator begin() const
    {
        Node* temp = _root;
        if(_size <= 1)
        {
            return Iterator(_root);
        }
        while(!temp->children.empty())
        {
            temp = temp->children[0];
        }
        return Iterator(temp);
    }

    Iterator end() const
    {
        return Iterator(nullptr);
    }

    Iterator getChild(Iterator parent, std::size_t index) const
    {
        return Iterator(parent.ptr->children[index]);
    }

    int getNumberOfChildren(Iterator parent) const
    {
        return static_cast<int>(parent.ptr->children.size());
    }

    void erase(Iterator iter)
    {
        if (iter.ptr == _root && _root != nullptr)
        {
            delete_node(iter.ptr);
            _root = nullptr;
            return;
        }
        std::vector<Node*>& parent_children = iter.ptr->parent->children;
        int pos = 0;
        for(Node* p : parent_children)
        {
            if(p == iter.ptr) break;
            pos++;
        }
        parent_children.erase(parent_children.begin()+pos);
        delete_node(iter.ptr);
    }

    void clear()
    {
        if (_root != nullptr)
            delete_node(_root);
        _root = nullptr;
    }

    bool empty() const
    {
        return _root == nullptr;
    }

    std::size_t size() const
    {
        return _size;
    }
};

} // END NAMESPACE RCN

#endif // __GENERAL_TREE_HPP__
