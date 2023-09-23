#ifndef _list_hpp
#define _list_hpp
//Kod preuzet sa vezbi za sinhronu promenu konteksta

template <typename T>
class List{
private:
    struct Elem{
        T* data;
        Elem* next;

        Elem(T* data, Elem* next): data(data), next(next){}
    };
    Elem* head, *tail;

public:
    List() : head(0), tail(0){}

    List(const List<T> &) = delete;
    List<T> &operator = (const List<T> &) = delete;

    void addFirst(T* data){ //dadaje element na pocetak liste
        Elem* elem = new Elem(data, head);
        head = elem;
        if(!tail) {tail = head;}
    }

    void addLast(T* data){ //dodaje element na kraj liste
        Elem* elem = new Elem(data, 0);
        if(tail){
            tail->next = elem;
            tail = elem;
        }
        else{
            head = tail = elem;
        }
    }

    T* removeFirst(){ //uzima element sa pocetka liste
        if(!head) {return 0;}

        Elem* elem = head;
        head = head->next;
        if(!head) {tail = 0;}

        T* ret = elem->data;
        delete elem;
        return ret;
    }

    T* peekFirst(){ //pogleda element sa pocetka liste
        if(!head){ return 0; }
        return head->data;
    }

    T* removeLast(){ //uzima element sa kraja liste
        if(!head){ return 0; }

        Elem* prev = 0;
        for(Elem* curr = head; curr != tail; curr = curr->next){
            prev = curr;
        }

        Elem* elem = tail;
        if(prev){ prev->next = 0; }
        else { head = 0; }
        tail = prev;

        T* ret = elem->data;
        delete elem;
        return ret;
    }

    T* peekLast(){ //pogleda element sa kraja liste
        if(!tail) { return 0; }
        return tail->data;
    }
};

#endif //_list_hpp