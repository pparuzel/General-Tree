#ifndef __GENERAL_TREE_HPP__
#define __GENERAL_TREE_HPP__

/**
 *  GeneralTree data structure
 *
 *  @author  Pawel Paruzel
 *  @version 0.2.2
 */

#include <vector>
#include <deque>

/*

    template <typename T>
    class GeneralTree
    {
        Node* _root;
        int _size;
    public:
        typedef T           value_type;
        typedef Iterator    iterator;
        typedef CIterator   const_iterator;
        typedef std::size_t size_type;

        GeneralTree();
        GeneralTree(const T& );
        GeneralTree(T&& ) noexcept;
        GeneralTree(const GeneralTree& );
        ~GeneralTree();
        GeneralTree& operator=(const GeneralTree& );
        GeneralTree& operator=(GeneralTree&& ) noexcept;
        iterator insert(const T& , const_iterator , std::size_t );
        const_iterator root() const;
        iterator root();
        const_iterator begin() const;
        iterator begin();
        const_iterator cbegin() const;
        const_iterator end() const;
        iterator end();
        const_iterator cend() const;
        const_iterator getChild(const_iterator , std::size_t );
        iterator getChild(const_iterator , std::size_t );
        int getNumberOfChildren(const_iterator );
        void erase(const_iterator );
        void clear();
        bool empty();
        std::size_t size();
    };
 */

namespace rcn {

template<typename T>
class GeneralTree
{
	/***************
	 *			   *
	 *  TREE NODE  *
	 *			   *
	 ***************/
    struct Node
    {
        T value;
        Node* parent;
        std::vector<Node*> children;

        explicit Node(T val = T(), Node* par = nullptr) : value(val), parent(par) {}
    };


    /********************
     *					*
     *  CONST ITERATOR  *
     *					*
     ********************/
    class CIterator
    {
        friend class GeneralTree;

        Node* ptr;

        explicit CIterator(Node* n) : ptr(n) {}
    public:
        CIterator() : ptr(nullptr) {}

        CIterator(const CIterator& it) : ptr(it.ptr) {}

        CIterator(CIterator&& it) noexcept : ptr(it.ptr)
        {
            it.ptr = nullptr;
        }

        CIterator& operator=(const CIterator& it)
        {
            ptr = it.ptr;
            return *this;
        }

        CIterator& operator=(CIterator&& it) noexcept
        {
            ptr = it.ptr;
            it.ptr = nullptr;
            return *this;
        }

        const T& operator*() const
        {
            return ptr->value;
        }

        CIterator& operator++()
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

        const T* operator->() const
        {
            return &ptr->value;
        }

        CIterator operator++(int)
        {
            CIterator temp = *this;
            this->operator++();
            return temp;
        }

        bool operator==(CIterator it) const
        {
            return this->ptr == it.ptr;
        }

        bool operator!=(CIterator it) const
        {
            return this->ptr != it.ptr;
        }
    };

    /************************
	 *						*
     *  NON-CONST ITERATOR  *
     *						*
     ************************/
    class Iterator
    {
        friend class GeneralTree;

        Node* ptr;

        explicit Iterator(Node* n) : ptr(n) {}
    public:
        Iterator() : ptr(nullptr) {}

        Iterator(const Iterator& it) : ptr(it.ptr) {}

        Iterator(Iterator&& it) noexcept : ptr(it.ptr)
        {
            it.ptr = nullptr;
        }

        Iterator& operator=(const Iterator& it)
        {
            ptr = it.ptr;
            return *this;
        }

        Iterator& operator=(Iterator&& it) noexcept
        {
            ptr = it.ptr;
            it.ptr = nullptr;
            return *this;
        }

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

        operator CIterator() const // NOLINT
        {
            return CIterator(ptr);
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
    void copy_successors(Node* parent_copy, Node* other)
    {
    	// Undefined Behavior if copy has
    	// children or parent_copy is nullptr
    	if (other == nullptr) { return; }
    	auto* current = other;
    	while (current != other->parent)
    	{
    		auto copy_size = parent_copy->children.size();
    		if (parent_copy->children.size() < current->children.size())
    		{
    			parent_copy->children.push_back(new Node(current->children[copy_size]->value, parent_copy));
    			++_size;
    			current = current->children[copy_size];
    			parent_copy = parent_copy->children[copy_size];
    		}
    		else
    		{
    			current = current->parent;
    			parent_copy = parent_copy->parent;
    		}

    	}
    }

    /****************
     * TREE MEMBERS *
     ****************/
    Node* _root;
    std::size_t _size;
public:
    /***************
     * TYPE TRAITS *
     ***************/
    typedef T           value_type;
    typedef Iterator    iterator;
    typedef CIterator   const_iterator;
    typedef std::size_t size_type;

    /**
     * Default constructor creates an empty
     * tree without the root assigned
     */
    GeneralTree() : _root(nullptr), _size(0) {}

    /**
     * Constructor initilizing the tree
     * with a root of some value
     *
     * @param value value assigned to root
     */
    explicit GeneralTree(const T& value) : _root(new Node(value)), _size(1) {}

    GeneralTree(const GeneralTree& tree) : _root(nullptr), _size(tree._size)
    {
        if(tree._root == nullptr) return;
        this->_root = new Node(tree._root->value, nullptr);
        copy_successors(_root, tree._root);
    }

    GeneralTree(GeneralTree&& tree) noexcept : _root(tree._root), _size(tree._size)
    {
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
        copy_successors(_root, tree._root);

        return *this;
    }

    GeneralTree& operator=(GeneralTree&& tree) noexcept
    {
        this->clear();
        this->_root = tree._root;
        this->_size = tree._size;
        tree._root = nullptr;
        tree._size = 0;

        return *this;
    }

    /**
     * Insertion method
     * To insert a value into the root, one must pass
     * three parameters: a value, an empty iterator and index 0
     * Inserting to the root twice results in an Undefined Behavior
     * If index position of the child is larger than the amount
     * of children, the value is inserted as the last element
     *
     * @param value value to insert
     * @param it_parent iterator to the parent
     * @param index position of the new child
     * @return iterator pointing at the inserted node
     */
    iterator insert(const T& value, const_iterator it_parent, std::size_t index)
    {
        _size++;
        if (it_parent.ptr == nullptr && index == 0 && _root == nullptr)
        {
            _root = new Node(value);
            return iterator(_root);
        }
        auto* node = new Node(value, it_parent.ptr);
        if (index >= it_parent.ptr->children.size())
        {
            it_parent.ptr->children.push_back(node);
            return iterator(node);
        }
        std::vector<Node*>* children = &(it_parent.ptr->children);
        children->insert(children->begin() + index, node);
        return iterator(node);
    }

    /**
     * Mutable iterator to the root of the tree
     *
     * @return iterator to root
     */
    iterator root()
    {
        return iterator(_root);
    }

    /**
     * Immutable iterator to the root of the tree
     *
     * @return const_iterator to root
     */
    const_iterator root() const
    {
        return const_iterator(_root);
    }

    /**
     * Mutable iterator to the first element in post-order
     *
     * @return iterator to left-most element of the tree
     */
    iterator begin()
    {
        Node* temp = _root;
        if(_size <= 1)
        {
            return iterator(_root);
        }
        while(!temp->children.empty())
        {
            temp = temp->children[0];
        }
        return iterator(temp);
    }

    /**
     * Immutable iterator to the first element in post-order
     *
     * @return const_iterator to left-most element of the tree
     */
    const_iterator begin() const
    {
        Node* temp = _root;
        if(_size <= 1)
        {
            return const_iterator(_root);
        }
        while(!temp->children.empty())
        {
            temp = temp->children[0];
        }
        return const_iterator(temp);
    }

    /**
     * Immutable iterator to the first element in post-order
     *
     * @return const_iterator to left-most element of the tree
     */
    const_iterator cbegin() const
    {
        Node* temp = _root;
        if(_size <= 1)
        {
            return const_iterator(_root);
        }
        while(!temp->children.empty())
        {
            temp = temp->children[0];
        }
        return const_iterator(temp);
    }

    /**
     * Mutable iterator to the element
     * following the last element
     *
     * @return iterator to past-the-end element
     */
    iterator end()
    {
        return iterator(nullptr);
    }

    /**
     * Immutable iterator to the element
     * following the last element
     *
     * @return const_iterator to past-the-end element
     */
    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    /**
     * Immutable iterator to the element
     * following the last element
     *
     * @return const_iterator to past-the-end element
     */
    const_iterator cend() const
    {
        return const_iterator(nullptr);
    }

    /**
     * Method of getting any child of parent iterator
     * If parent iterator is empty
     * then root node is returned
     * Parent with no children results in Undefined Behavior
     * Index out of bounds results in Undefined Behavior
     *
     * @param parent iterator over the child
     * @param index position of the child
     * @return const_iterator to the child
     */
    const_iterator getChild(const_iterator parent, std::size_t index) const
    {
        if (parent.ptr == nullptr) { return root(); }
        return const_iterator(parent.ptr->children[index]);
    }

    /**
     * Method of getting any child of parent iterator
     * If parent iterator is empty
     * then root node is returned
     * Parent with no children results in Undefined Behavior
     * Index out of bounds results in Undefined Behavior
     *
     * @param parent iterator over the child
     * @param index position of the child
     * @return iterator to the child
     */
    iterator getChild(const_iterator parent, std::size_t index)
    {
        if (parent.ptr == nullptr) { return root(); }
        return iterator(parent.ptr->children[index]);
    }

    /**
     * Method of getting the number of
     * the children directly connected 
     * to the parent node
     * If parent is an empty iterator
     * the return value is either 1
     * if the root exists or 0 if it does not
     *
     * @param parent iterator to the node
     * @return integer amount of children of the parent iterator
     */
    int getNumberOfChildren(const_iterator parent) const
    {
        if (parent.ptr == nullptr) { return static_cast<int>(_root != nullptr); }
        return static_cast<int>(parent.ptr->children.size());
    }

    /**
     * Method of erasing an iterator pointee
     * Empty iterator results in Undefined Behavior
     *
     * @param iter iterator pointing to a node
     */
    void erase(const_iterator iter)
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

    /**
     * Method for clearing the whole tree
     * Deallocates all nodes iteratively
     * Assigns root to nullptr
     */
    void clear()
    {
        if (_root != nullptr)
            delete_node(_root);
        _root = nullptr;
    }

    /**
     * Returns if the root is empty
     *
     * @return boolean if root is nullptr
     */
    bool empty() const
    {
        return _root == nullptr;
    }

    /**
     * Returns the size of the tree
     *
     * @return number of nodes in the tree
     */
    std::size_t size() const
    {
        return _size;
    }
};

} // END NAMESPACE RCN

#endif // __GENERAL_TREE_HPP__
