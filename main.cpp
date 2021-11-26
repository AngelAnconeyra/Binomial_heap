#include <iostream>
#include <fstream>
#include <list>
using namespace std;

template <class T>
class NodoB
{
public:
    int m_Grado;
    T m_Dato;
    NodoB<T> *m_Padre;
    list<NodoB<T> *> m_Sons;
    bool m_Color;

    NodoB(T d)
    {
        m_Dato = d;
        m_Grado = 0;
        m_Padre = 0;
        m_Color = 0;
    }
};

template <class T>
class Binomial_Heap
{
private:
    list<NodoB<T> *> m_Roots;
    NodoB<T> *m_pMin;

private:
    void Compactar();                         // O(log(n))
    NodoB<T> *Unir(NodoB<T> *p, NodoB<T> *q); // O(1)
    void PrintRaices(typename list<NodoB<T> *>::iterator it, typename list<NodoB<T> *>::iterator end, ofstream &archivo);
    void PrintArbol(NodoB<T> *, ofstream &archivo);

public:
    Binomial_Heap(){};
    ~Binomial_Heap(){};
    /**************************************/
    void Insert(T d);                      // O(1))
    void Extrac_Min();                     // O(log(n))
    void Delete(NodoB<T> *e);              // O(log(n))
    void Decrease_Key(NodoB<T> *e, T val); // O(1)
    NodoB<T> *GetMin();                    // O(1)
    void Show_Dot(string filename);
};

template <class T>
void Binomial_Heap<T>::Insert(T d)
{
    NodoB<T> *pNew = new NodoB<T>(d);
    m_Roots.push_front(pNew);
    if (d < m_pMin->m_Dato)
    {
        m_pMin = pNew;
    }
    //cout<<"insertando "<<pNew->m_Dato<<endl;
    Compactar();
}

template <class T>
void Binomial_Heap<T>::Extrac_Min() // O(log(n))
{
    typename list< NodoB< T>  * >::iterator it = m_pMin->m_Sons.begin();
    m_pMin->m_Dato=100000;
    NodoB<T> * pNew = m_pMin;
    for(; it != m_pMin->m_Sons.end(); it++)
        m_Roots.push_front(*it);
        if((*(it))->m_Dato<m_pMin->m_Dato){
            m_pMin=(*it);
        }
        Compactar();
    m_Roots.remove(pNew); // O(log(n))
}

template <class T>
void Binomial_Heap<T>::Delete(NodoB<T> *e) // O(log(n))
{
    Decrease_Key(e,m_pMin->m_Dato - 1);
    m_pMin = e;
    Extrac_Min();
}

template <class T>
void Binomial_Heap<T>::Decrease_Key(NodoB<T> *e, T val) // O(1)
{
    e->m_Dato = val;
    while(e->m_Padre && e->m_Padre->m_Dato  >  e->m_Dato)
    {
        swap( e->m_Dato,e->m_Padre->m_Dato);
        e = e->m_Padre;
    }
}

template <class T>
NodoB<T> *Binomial_Heap<T>::GetMin() // O(1)
{
    return m_pMin;
}

// Log(n)
template <class T>
void Binomial_Heap<T>::Compactar() // O(log(n))
{
    typename list<NodoB<T> *>::iterator it,it2;
    it = m_Roots.begin();
    it2 = m_Roots.end();
    for (; it!=it2; it++)
    {
        if (next(it, 1) != m_Roots.end())
        {
            if ((*it)->m_Grado == (*(next(it, 1)))->m_Grado)
            {
                //cout << "Hice una union de " << (*(it))->m_Dato << " con " << (*(next(it, 1)))->m_Dato << endl;
                Unir((*(it)), (*(next(it, 1))));
                it2=m_Roots.end();
                it=m_Roots.begin();
            }
        }
    }
}

// O(1)
template <class T>
NodoB<T> *Binomial_Heap<T>::Unir(NodoB<T> *p, NodoB<T> *q) // O(1)
{
    if (p->m_Dato < q->m_Dato)
    {
        p->m_Grado += 1;
        p->m_Sons.push_front(q);
        q->m_Padre = p;
        m_Roots.remove(q);
        //q = NULL;
        return p;
    }
    q->m_Grado += 1;
    q->m_Sons.push_front(p);
    //p = NULL;
    p->m_Padre = q;
    m_Roots.remove(p);
    return q;
}

template <class T>
void Binomial_Heap<T>::Show_Dot(string filename)
{
    ofstream archivo;
    archivo.open(filename.c_str(), ios::out);
    archivo << "digraph binomialHeap {" << endl;
    archivo << "label= \"Binomial Heap\";" << endl;
    archivo << "node [shape = record];" << endl;
    typename list<NodoB<T> *>::iterator it = m_Roots.begin();
    PrintRaices(it, m_Roots.end(), archivo);
    archivo << "}" << endl;
    system("dot -Tjpg -O binomial.dot");
    system("open binomial.dot.jpg");
}

template <class T>
void Binomial_Heap<T>::PrintRaices(typename list<NodoB<T> *>::iterator it, typename list<NodoB<T> *>::iterator end, ofstream &archivo)
{
    if (it == end)
    {
        return;
    }
    archivo<<"subgraph "<<(*it)->m_Dato<<"{ label = "<< (*it)->m_Dato<<" ;"<<endl;
    archivo << (*it)->m_Dato << "[label= " << (*it)->m_Dato << "];" << endl;
    PrintArbol(*it,archivo);
    if (((next(it, 1))) != end)
    {
        archivo<<"{rank=same; "<<(*it)->m_Dato<<"; "<<(*(next(it, 1)))->m_Dato<<"}";
        archivo << (*it)->m_Dato << " -> " << (*(next(it, 1)))->m_Dato << endl;
    }
    archivo<<"}"<<endl;
    PrintRaices((++it), end, archivo);
}

template<typename T>
void Binomial_Heap<T>::PrintArbol(NodoB<T> * nodo,ofstream &archivo){
    if(!nodo)
        return ;
    typename list<NodoB<T> *>::iterator it=nodo->m_Sons.begin();
    //cout<<endl<<"Mostrando hijos de "<<nodo->m_Dato<<endl;
    for(;it!= nodo->m_Sons.end();it++){
        cout<<((*it)->m_Dato)<<"   ";
        archivo << nodo->m_Dato <<" -> "<< (*it)->m_Dato<<";"<< endl;
        PrintArbol((*it),archivo);
    }
}

int main()
{
    Binomial_Heap<int> bh;
    bh.Insert(3);
    bh.Insert(30);
    bh.Insert(4);
    bh.Insert(5);
    bh.Insert(6);
    bh.Insert(7);
    bh.Insert(2);
    bh.Insert(4);
    bh.Insert(6);
    bh.Insert(7);
    bh.Insert(2);
    bh.Insert(4);
    bh.Extrac_Min();
    bh.Show_Dot("binomial.dot");
    cout<<endl;
    return 1;
}
