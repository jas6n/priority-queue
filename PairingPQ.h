// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>
using namespace std;

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // to initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent_node{nullptr}
            {}

            // Description: Allows access to the element at that Node's
            // position.  There are two versions, getElt() and a dereference
            // operator, use whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent_node;

    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root_ptr{nullptr}, node_count{0} {
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } , root_ptr{nullptr}, node_count{0}{

        while(start != end){
            push(*start);
            ++start;
        }

    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare }, root_ptr{nullptr}, node_count{0} {
        
        if (other.root_ptr != nullptr){ 
            deque<Node*> copy_deque;
            copy_deque.push_back(other.root_ptr);
        
            while(copy_deque.empty() == false) {
                // get next from dq
                Node* current = copy_deque.front();

                // add child and siblings to DQ
                if (current->child != nullptr){
                    copy_deque.push_back(current->child);
                }

                if (current->sibling != nullptr){ 
                    copy_deque.push_back(current->sibling);
                    }

                
                push(current->elt);
                
                copy_deque.pop_front();
            }
        }


    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        
        PairingPQ t(rhs);

        swap(node_count, t.node_count);

        swap(root_ptr, t.root_ptr);


        return *this;


    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {

        deque<Node*> destroy_deque;

        if (root_ptr != nullptr) {
            destroy_deque.push_back(root_ptr);
        }

        while(!destroy_deque.empty()) {

            Node* current = destroy_deque.front();
            destroy_deque.pop_front();


            if (current->child != nullptr){ 
                destroy_deque.push_back(current->child);
            }
            if (current->sibling != nullptr){ 
                destroy_deque.push_back(current->sibling);
            }

            delete current;
        }


    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {

        deque<Node*> node_deque;

        if (root_ptr != nullptr){ 
            node_deque.push_back(root_ptr);
        }

        root_ptr = nullptr;

        while(node_deque.empty() == false) {

            Node* next_node = node_deque.front();

            if (next_node->child != nullptr){ 
                node_deque.push_back(next_node->child);
            }

            if (next_node->sibling != nullptr){
                 node_deque.push_back(next_node->sibling);
            }

            next_node->parent_node = nullptr;
            next_node->child = nullptr;
            next_node->sibling = nullptr;

            if (root_ptr != nullptr) {
                root_ptr = meld(root_ptr, next_node);
            }
            else {
                root_ptr = next_node;
            }

            node_deque.pop_front();
        }


    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {

        if (node_count == 1) { 

            node_count -= 1;
            delete root_ptr;
            root_ptr = nullptr; 
            return;

        }

        deque<Node*> node_deque;


        for (Node* p = root_ptr->child; p != nullptr; p = p->sibling) {
            node_deque.push_back(p);
        }



        if (node_deque.size() == 1) {
            node_count -= 1;

            delete root_ptr;
            root_ptr = node_deque.front();
            root_ptr->parent_node = nullptr;

            return;
        } else {
            while (node_deque.size() > 1) {

                Node* node1 = node_deque.front();
                node_deque.pop_front();
                node1->parent_node = nullptr;
                node1->sibling = nullptr;


                Node* node2 = node_deque.front();
                node_deque.pop_front();
                node2->parent_node = nullptr;
                node2->sibling = nullptr;


                node_deque.push_back(meld(node1, node2));
            }
        }
        node_count -= 1;
        delete root_ptr;
        root_ptr = node_deque.front();


    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        
        return root_ptr->elt;


    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        
        return static_cast<size_t>(node_count);

    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        
        if (node_count == 0) { 
            return true;
        } else {
            return false;
        }


    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        

        node->elt = new_value;

        if (node != root_ptr) {

            if (this->compare(node->elt, node->parent_node->elt)){ 
                return;
            } else if (node == node->parent_node->child) {

                node->parent_node->child = node->sibling;

                node->parent_node = nullptr;
                node->sibling = nullptr;

                root_ptr = meld(node, root_ptr);
            }

            else {
                Node* new_node = node->parent_node->child;

                while (new_node->sibling != node){ 
                    new_node = new_node->sibling;
                }

                new_node->sibling = node->sibling;

                node->sibling = nullptr;
                node->parent_node = nullptr;
                
                root_ptr = meld(node, root_ptr);
            }
        }



    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node* addNode(const TYPE &val) {
        node_count += 1;
        if (root_ptr == nullptr) {
            root_ptr = new Node(val);
            return root_ptr;
        } else {
            Node* new_node = new Node(val);
            root_ptr = meld(root_ptr, new_node);
            return new_node;
        }
    } // addNode()


private:
     Node * meld(Node* node1, Node* node2) { 
        if (this->compare(node1->elt , node2->elt)) {

            if (node2->child != nullptr) {
                node1->sibling = node2->child;
            }

            node2->child = node1;
            node1->parent_node = node2;
            return node2;

        
        } 
        else {

            if (node1->child != nullptr) {
                node2->sibling = node1->child;
            }

            node1->child = node2;
            node2->parent_node = node1;
            return node1;
            
        } 
    }

    Node* root_ptr;
    int node_count;

    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif // PAIRINGPQ_H
