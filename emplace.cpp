#ifndef DLL_HPP
#define DLL_HPP

#include <iostream>
#include <limits>

template <typename T>
class DLL {
    private:
        struct Node {
            T val;
            Node* next;
            Node* prev;

            Node(T _val = T(), Node* n = nullptr, Node* p = nullptr) : val{_val}, next{n}, prev{p} {}
            template <typename... Types>
            Node(Types&& ... args, Node* n = nullptr, Node* p = nullptr ) : val(std::forward<T>(args ...)), next{n}, prev{p} {}
        }; 

        Node* head;
        Node* tail;
        size_t size = 0;

        void copy(DLL& l, DLL& r);
    public:
        DLL(Node* h = nullptr, Node* t = nullptr);
        DLL(size_t count, const T& _val);
        DLL(std::initializer_list<T> init);
        DLL(const DLL& other);
        DLL(DLL&& other) noexcept;
        ~DLL();
        
        DLL& operator=(const DLL& other);
        DLL& operator=(DLL&& other) noexcept;
        DLL& operator=(std::initializer_list<T> init);  
        void assign(size_t count, const T& _val);
        void assign(std::initializer_list<T> init);
       
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
       
        bool empty () const noexcept { return head == nullptr; }
        size_t get_size() const noexcept { return size; }
        size_t max_size() const noexcept { return std::numeric_limits<size_t>::max(); } 
        Node* get_head()const noexcept { return head; }
        Node* get_tail()const noexcept { return tail; }


        void clear() noexcept;
        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();
        void push_front(const T& value);
        void push_front(T&& value);
        void pop_front();
        void resize(size_t count );
        void resize(size_t count, const T& value);
        void swap(DLL& other ) noexcept;
        void insert(size_t pos, const T& value);
        void erase(size_t pos);
        template<typename... Types>
        void emplace_back(Types... args);
        template<typename... Types>
        void emplace_front(Types... args);
        template<typename... Types>
        void emplace(size_t pos, Types... args);
        
        void merge(DLL& other);
        size_t remove(const T& value);
        void reverse();
        void unique();

        template <typename U>
        friend bool operator==(const DLL<U>& l,const DLL<U>& r);
        template <typename U>
        friend bool operator!=(const DLL<U>& l,const DLL<U>& r) { return !(l == r); }
        template <typename U>
        friend bool operator<(const DLL<U>& l,const DLL<U>& r);
        template <typename U>
        friend bool operator<=(const DLL<U>& l,const DLL<U>& r) { return !(r < l); }
        template <typename U>
        friend bool operator>(const DLL<U>& l,const DLL<U>& r) { return r < l; }
        template <typename U>
        friend bool operator>=(const DLL<U>& l,const DLL<U>& r) { return !(l < r); }
};

template <typename T>
void DLL<T>::clear() noexcept {
    while (head) {
        auto* tmp = head;
        head = head->next;
        delete tmp;
    }
    tail = nullptr;
    size = 0;
}

template <typename T>
void DLL<T>::copy(DLL& l, DLL& r) {
    l.clear();   

    auto* tmp = r.head;
    while (tmp) {
        l.push_back(tmp->val);
        tmp = tmp->next;
    }  
}

template <typename T>
void DLL<T>::push_back(const T& value) {
    ++size;
    if (empty()) {
        head = tail = new Node(value);
        return;
    }
    tail->next = new Node(value,nullptr,tail); 
    tail = tail->next;
     
}

template <typename T>
void DLL<T>::push_back(T&& value) {
    ++size;
    if (empty()) {
        head = tail = new Node(std::move(value));
        return;
    }
    tail->next = new Node(std::move(value),nullptr,tail); 
    tail = tail->next; 
}

template <typename T>
void DLL<T>::pop_back() {
    if (empty()) throw std::logic_error("Can't pop from empty list.");
    if (head == tail) { 
        delete head;
        head = nullptr;
        tail = nullptr;
        size = 0;
        return;
    }
    auto* tmp = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete tmp;
    --size;
}

template <typename T>
void DLL<T>::push_front(const T& value) {
    ++size;
    if (empty()) {
        head = tail = new Node(value);
        return;
    }
    head->prev = new Node(std::move(value),head,nullptr);
    head = head->prev;
}

template <typename T>
void DLL<T>::push_front(T&& value) {
    ++size;
    if (empty()) {
        head = tail = new Node(std::move(value));
        return;
    }
    head->prev = new Node(std::move(value),head,nullptr);
    head = head->prev;
}

template <typename T>
void DLL<T>::pop_front() {
    if (empty()) throw std::logic_error("Can't pop from empty list.");
    if (head == tail) {
        delete head;
        head = tail= nullptr;
        size = 0;
        return;
    }
    auto* tmp = head;
    head = head->next;
    delete tmp;
    head->prev = nullptr;
    --size;
}

template <typename T>
void DLL<T>::resize(size_t count) {
    if (count == size) return;
    
    if (size > count) {
        for (size_t i{}; i < size - count; ++i) 
            pop_back();
    }

    else if (size < count) {
        for (size_t i{}; i < count - size; ++i) 
            push_back(T());
    }
}

template <typename T>
void DLL<T>::resize(size_t count, const T& value) {
    if (count == size) return;
    
    if (size > count) {
        for (size_t i{}; i < size - count; ++i) 
            pop_back();
    }

    else if (size < count) {
        for (size_t i{}; i < count - size; ++i) 
            push_back(value);
    }
}

template <typename T>
void DLL<T>::swap(DLL& other) noexcept {
    std::swap(head,other.head);
    std::swap(tail,other.tail);
    std::swap(size,other.size);
}

template <typename T>
void DLL<T>::insert(size_t pos, const T& value) {
    if (pos == 0) this->push_front(std::move(value));
    else if (pos == size) this->push_back(std::move(value));
    else if (pos > size) throw std::logic_error("Position is not in range.");
    else {
        auto* tmp = head;
        while (pos) {
            tmp = tmp->next;
            --pos;
        }
        tmp->prev->next = new Node(std::move(value), tmp, tmp->prev);
        tmp->prev = tmp->prev->next; 
        ++size;
    }
}

template <typename T>
void DLL<T>::erase(size_t pos) {
    if (pos >= size) throw std::logic_error("Position is out of range.");
    else if (pos == 0) this->pop_front();
    else if (pos == size - 1) this->pop_back();
    else {
        auto* tmp = head;
        while (pos) {
            tmp = tmp->next;
            --pos;
        }
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        delete tmp;
        --size;
    }
}

template <typename T>
template<typename... Types>
void DLL<T>::emplace(size_t pos, Types... args) {
    if (pos == 0) this->emplace_front(std::forward<T>((args) ...));
    else if (pos == size) this->emplace_back(std::forward<T>((args) ...));
    else if (pos > size) throw std::logic_error("Position is not in range.");
    else {
        auto* tmp = head;
        while (pos) {
            tmp = tmp->next;
            --pos;
        }

        tmp->prev->next = new Node(std::forward<T>((args) ...), tmp, tmp->prev);
        tmp->prev = tmp->prev->next; 
        ++size;
    }
}

template <typename T>
template<typename... Types>
void DLL<T>::emplace_back(Types... args) {
    ++size;
    if (empty()) {
        head = tail = new Node(std::forward<T>((args)...));
        return;
    }
    tail->next = new Node(std::forward<T>((args)...),nullptr,tail); 
    tail = tail->next; 
}

template <typename T>
template<typename... Types>
void DLL<T>::emplace_front(Types... args) {
    ++size;
    if (empty()) {
        head = tail = new Node(std::forward<T>((args)...));
        return;
    }
    head->prev = new Node(std::forward<T>((args)...),head,nullptr);
    head = head->prev;
}

template <typename T>
DLL<T>::DLL(Node* h , Node* t) : head{h}, tail{t} {
    auto* tmp = head;
    while (tmp) {
        ++size;
        tmp = tmp->next;
    }
}

template <typename T>
DLL<T>::DLL(size_t count, const T& _val) {
    size = count;
    if (size == 0) head = tail = nullptr;
    else {
        head  = new Node(_val);
        auto* tmp = head;
    
        for (size_t i{}; i < count - 1; ++i) {
            tmp->next = new Node(_val,nullptr,tmp);
            tmp = tmp->next;
        }
        tail = tmp;
    }
}

template <typename T>
DLL<T>::DLL(std::initializer_list<T> init) {
    if (init.size() == 0) {
        head = tail = nullptr;
        size = 0;
    }
    else {
        Node dummy;
        auto* tmp = &dummy;
    
        for (auto _val : init) {
            tmp->next = new Node(_val,nullptr,tmp);
            tmp = tmp->next;
        }
        tail = tmp;
        head = dummy.next;
        head->prev = nullptr;
        size = init.size();
    }
}

template <typename T>
DLL<T>::DLL(const DLL& other) {
    copy(*this,other);
}

template <typename T>
DLL<T>::DLL(DLL&& other) noexcept : head {other.head}, tail{other.tail}, size{other.size} {
    other.head = other.tail = nullptr;
    other.size = 0;
}

template <typename T>
DLL<T>::~DLL() {
    clear();
}

template <typename T>
DLL<T>& DLL<T>::operator=(const DLL& other) {
    if (this != &other) {
        clear();
        copy(*this,other);
    }
    return *this;
}

template <typename T>
DLL<T>& DLL<T>::operator=(DLL&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        size = other.size;
        other.head = other.tail = nullptr;
        other.size = 0;
    }
    return *this;
}

template <typename T>
DLL<T>& DLL<T>::operator=(std::initializer_list<T> init) {
    clear();
    if (init.size() == 0) {
        head = tail = nullptr;
        size = 0;
    }
    else {
        Node dummy;
        auto* tmp = &dummy;
    
        for (auto _val : init) {
            tmp->next = new Node(_val,nullptr,tmp);
            tmp = tmp->next;
        }
        tail = tmp;
        head = dummy.next;
        head->prev = nullptr;
        size = init.size();
    }
    return *this;
}

template <typename T>
void DLL<T>::assign(size_t count, const T& _val) {
    DLL<T> tmp(count,_val);
    swap(tmp); 
}

template <typename T>
void DLL<T>::assign( std::initializer_list<T> init) {
    DLL<T> tmp(init);
    swap(tmp);
}

template <typename T>
T& DLL<T>::front() {
    if (empty()) throw std::logic_error("Can't get empty list's first element.");
    return head->val;
}

template <typename T>
const T& DLL<T>::front() const {
    if (empty()) throw std::logic_error("Can't get empty list's first element.");
    return head->val;
}

template <typename T>
T& DLL<T>::back() {
    if (empty()) throw std::logic_error("Can't get empty list's last element.");
    return tail->val;
}

template <typename T>
const T& DLL<T>::back() const {
    if (empty()) throw std::logic_error("Can't get empty list's last element.");
    return tail->val;
}

template <typename T>
bool operator==(const DLL<T>& l,const DLL<T>& r) {
    if (l.get_size() != r.get_size()) return false;
   
    auto* tmp1 = l.get_head();
    auto* tmp2 = r.get_head(); 
    
    while (tmp1) {
        if(tmp1->val != tmp2->val) return false;
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }
    return true;
}

template <typename T>
bool operator<(const DLL<T>& l,const DLL<T>& r) {    
    auto* tmp1 = l.get_head();
    auto* tmp2 = r.get_head(); 
    
    while (tmp1 && tmp2) {
        if(tmp1->val < tmp2->val) return true;
        else if(tmp1->val > tmp2->val) return false;
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }
    if (l.get_size() < r.get_size()) return true;
    return false;
}

template <typename T>
void DLL<T>::merge(DLL& other) {
    if (this == &other) return;
    if (head == nullptr) swap(other);
    size += other.size;
    auto* tmp1 = head;

    while (other.head) {
        auto* tmp2 = other.head;
        if (tmp1 && tmp2->val >= tmp1->val) {
            tmp1 = tmp1->next;
            continue;
        }
        
        other.head = other.head->next;
        if (other.head != nullptr) other.head->prev = nullptr;
        
        if (tmp1 == nullptr) {
            tmp2->prev = tail;
            tail->next = tmp2;
            tail = other.tail;
            other.head= other.tail = nullptr;
            other.size = 0;
            return;
        }
        else if (tmp1 == head) {
            tmp2->next = tmp1;
            tmp2->prev = tmp1->prev;
            tmp1->prev = tmp2; 
            head = tmp2;
        }
        else { 
            tmp2->next = tmp1;
            tmp2->prev = tmp1->prev;
            tmp1->prev->next = tmp2;
            tmp1->prev = tmp2; 
        }
    }
    other.head= other.tail = nullptr;
    other.size = 0;
}

template <typename T>
size_t DLL<T>::remove(const T& value) {
    if (head == nullptr) return 0;
    
    size_t s = 0;
    auto* tmp = head;
    while (tmp) {
        if (tmp->val == value) {
            if (tmp == head) { 
                tmp = tmp->next;
                pop_front();
            }
            else if (tmp == tail){
                tmp = tmp->next;
                pop_back();
            }
            else {
                tmp->next->prev = tmp->prev;
                tmp->prev->next = tmp->next;
                auto* tmp2 = tmp;
                tmp = tmp->next;
                delete tmp2;
                --size;
            }

            ++s;
        }
        else tmp = tmp->next;
    }
    return s;
}

template <typename T>
void DLL<T>::reverse() {
    if (empty() || (head == tail)) return;
    
    auto* tmp = head;
    while (tmp) {
        std::swap(tmp->next,tmp->prev);
        tmp = tmp->prev;
    }   
    std::swap(head,tail);
}

template <typename T>
void DLL<T>::unique() {
    if (empty() || head == tail) return;
    
    auto* tmp = head;
    while (tmp != tail) {
        if (tmp->val == tmp->next->val) {
            auto* tmp2 = tmp->next;
            if (tmp2 == tail) { 
                pop_back();
                continue;
            }
            tmp2->next->prev = tmp;
            tmp->next = tmp2->next;
            delete tmp2;
            --size;
        }
        else tmp = tmp->next;
    }
}

#endif