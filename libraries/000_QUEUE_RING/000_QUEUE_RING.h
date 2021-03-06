//-----------------------------------------------------------------------
// PAR : Frederick Henri
//-----------------------------------------------------------------------
/*
 Implantation d'une file generique. Cette version alloue la memoire une
 unique fois!!! Cette file n'est donc pas dynamique apres sa creation.
 Afin d'utiliser les algorithmes et les foncteurs sans le probleme des
 itérateurs for(...iterator it=_qu.begin(); it!=_qu.end(); ++it){...}
 où begin & end pointent le même élément, la ringQueue instancie un item
 de plus que la taille passée au constructeur.
 */
//-----------------------------------------------------------------------
#ifndef __000_QUEUE_RING___
#define __000_QUEUE_RING___

/*
 #include <iostream.h>
 #include <stdlib.h>
 */
#include <vector>
#include <cstring>
using namespace std;
template<typename ITM>
void addMSG(ITM &item, const char *MSG){
	strncpy(&(item.MSG[strlen(item.MSG)]), MSG, 1 + strlen(MSG));
}

/*
 //-----------------------------------------------------------------------
 //							DEFINITION DE CLASSE
 //
 // Cette classe permet de creer des files de n'importe quel type.
 // Le type choisit doit permettre l'affectation (=).
 Crée un container (vector) avec 1 élément de plus que la taille demandée afin
 que end() soit différent de begin() pour autoriser un itérateur circulaire
 utilisable par algorithme for(... it=begin(); it!=end(), ++it){...}

 //-----------------------------------------------------------------------
 */
/*

 A chaque appel d'une fonction de "myIterator" _DBGRQ_ provoque un sprintf
 des variables suivies dans le membre char MSG[...] du template <class T>.
 ATTENTION:
 MSG doit bien sûr exister & être assez long pour accepter le message.
 _DBGRQ_ ne prend que les caracères 7 bits ASCII € [!,~].
 DEBUG_RQLVL 0	Rien
 DEBUG_RQLVL 1
 DEBUG_RQLVL 2	Begin End Start Stop

*/
//#define DEBUG_RQLVL 
#ifdef DEBUG_RQLVL
	#define _DBGRQ_(LVL,CHN,format,...) \
	do { \
		extern int _dbgLvlMax; \
		if (LVL<=_dbgLvlMax) { \
			sprintf(CHN,format,__VA_ARGS__); \
	    } \
	}while(0)
#else	
#define _DBGRQ_(...)
#endif

template<class T>
class ringQueue {
private:
	vector<T> *donnees; //=new vector<T>(Z+1);		// Le tableau de donnees.
	// On ne permet pas la copie (d'aucune sorte) !
	ringQueue(const ringQueue&);
	ringQueue& operator =(const ringQueue&);
	protected:
	public:
	int taille; // Taille de la file.
	int debut, fin; // Debut et fin de la file.
	int nb_elem; // Nombre d'elements de la file.
	class myIterator: public iterator<random_access_iterator_tag, T> {
		T *it;
		T *first;
		T *last;
		public:
		//
		//	beginning (starting, etc...) constructors
		//	defining first & last pointers for circular looping increments
		//
		myIterator(vector<T> *data, T *x) :
			it(x),
				first(&(*data)[0]),
				last((T*)((unsigned int)(&(*data)[0]) + (data->size() * sizeof(T))))
		{
			_DBGRQ_(9,(*it).MSG,"%s/%d,%d,%d,%d",(*it).MSG,(unsigned int)first,(unsigned int)last,data->size(),sizeof(T));
		}
		//
		//	ending (stopping, etc...) constructors
		//	TODO defining first & last pointers for circular looping decrements
		//
		myIterator(T *x) :
			it(x){
			_DBGRQ_(9,(*it).MSG,"%s/%d,%d",(*it).MSG,(unsigned int)first,(unsigned int)last);
		}
		myIterator(const myIterator &mit) :
			it(mit.it){
			_DBGRQ_(9,(*it).MSG,"%s/%c",(*it).MSG,'h');
		}
		myIterator& operator++(){
			++it;
			if (it == last)
				it = first;
			// 	possible si insert erase
			//	if(it==org->end())it=org->begin();
			_DBGRQ_(9,(*it).MSG,"%s/%c%d",(*it).MSG,'#',(unsigned int)it);
			return *this;
		}
		myIterator& operator++(int){
			it++;
			if (it == last)
				it = first;
			_DBGRQ_(9,(*it).MSG,"%s/%c",(*it).MSG,'#');
			return *this;
		}
		bool operator==(const myIterator &rhs){
			_DBGRQ_(9,(*it).MSG,"%s/%c",(*it).MSG,it==rhs.it?'=':'!');
			return it == rhs.it;
		}
		bool operator!=(const myIterator &rhs){
			_DBGRQ_(9,(*it).MSG,"%s/%c",(*it).MSG,it!=rhs.it?'!':'=');
			return it != rhs.it;
		}
		T& operator*(){
			_DBGRQ_(9,(*it).MSG,"%s/%c",(*it).MSG,'*');
			return *it;
		}
	};
	myIterator begin(){
		T *pdata = &(*donnees)[debut];
		_DBGRQ_(2,pdata->MSG,"%s/%s%d",pdata->MSG,"Begin:",getIDX(pdata));
		return myIterator(donnees, pdata);
	}
	myIterator end(){
		T *pdata = &(*donnees)[fin];
		_DBGRQ_(2,pdata->MSG,"%s/%s%d",pdata->MSG,"End:",getIDX(pdata));
		return myIterator(pdata);
	}
	//---------------------------------------------------------------------
	/*	start begin décalé de idx>0 éléments

	 PERFORMANCE il est possible de passer un label const String="Stop" pour
	 mieux debugger (par ex Deriv au lieur de Start dans l'algorithm PID) mais
	 on aura une baisse de performance à évaluer ...

	 */
	myIterator start(int idx){
		T *pdata;
		if (nb_elem == taille) {
			pdata = &(*donnees)[(debut + idx) % (taille + 1)];
		} else if (nb_elem < (debut + idx)) {
			pdata = &(*donnees)[fin];
		} else {
			pdata = &(*donnees)[(debut + idx)];
		}
		_DBGRQ_(2,pdata->MSG,"%s/%s%d",pdata->MSG,"Start:",getIDX(pdata));
		return myIterator(donnees, pdata);
	}
	//---------------------------------------------------------------------
	/*	stop à idx>0 éléments de begin ou de start
	 */
	myIterator stop(int idx){
		T *pdata;
		if (nb_elem == taille) {
			pdata = &(*donnees)[(debut + idx) % (taille + 1)];
		} else if (nb_elem < (debut + idx)) {
			pdata = &(*donnees)[fin];
		} else {
			pdata = &(*donnees)[(debut + idx)];
		}
		_DBGRQ_(2,pdata->MSG,"%s/%s%d",pdata->MSG,"Stop:",getIDX(pdata));
		return myIterator(pdata);
	}
	// CONSTRUCTEUR  explicit
	ringQueue(const int SIZE, const int DBGLVLMAX);
	inline ~ringQueue();
	T getX(int i);
	int getIDX(T *pdata);
	void clrX(int i);
	void clrAllMSG();
	int cnt();
	void enqueue(const T);
	inline void empty(){
		debut = fin = nb_elem = 0;
	}
	inline int size() const{
		return nb_elem;
	}
};
//-------------------------------------------------------
//  CONSTRUCTEUR
//
template<class T> ringQueue<T>::ringQueue(const int SIZE, const int DBGLVLMAX){
	extern int _dbgLvlMax;
	_dbgLvlMax = DBGLVLMAX;
	if (SIZE > 0) {
		//donnees = new vector<T>[size];
		//

//
		donnees = new vector<T>(SIZE + 1);

		//donnees=&data[0];
		taille = ((donnees != 0) ? SIZE : 0);
		debut = fin = nb_elem = 0;
	}
}
//-------------------------------------------------------
//  DESTRUCTEUR
//
template<class T> ringQueue<T>::~ringQueue(){
	if (taille > 0)
		delete[] donnees;
}
//-------------------------------------------------------
//  getX
//
template<class T> int ringQueue<T>::getIDX(T *pdata){
	/*
	 long int x=(long int)pdata;
	 long int y=(long int)(&(*donnees)[0]);
	 return (x-y)/sizeof(T);
	 */
	void *x = pdata;
	T *y = &(*donnees)[0];
	ptrdiff_t diff = pdata - y;
	return diff;
}
//-------------------------------------------------------
//  getX
//
template<class T> T ringQueue<T>::getX(int i){
	return (*donnees)[i];
}
//-------------------------------------------------------
//  clrX
//
template<class T> void ringQueue<T>::clrX(int i){
	(*donnees)[i].MSG[0] = 0;
}
//-------------------------------------------------------
//  clrX
//
template<class T> void ringQueue<T>::clrAllMSG(){
	for (size_t i = 0; i <= taille; i++)
		clrX(i);
}
//-------------------------------------------------------
//  NOMBRE D'ELEMENTS
//
template<class T> int ringQueue<T>::cnt(){
	return nb_elem;
}
//-------------------------------------------------------
//  MISE EN QUEUE
/*
 fin pointe le premier elt libre ou début qui sera écrasé
 */
template<class T> void ringQueue<T>::enqueue(const T element){
	//if (nb_elem == taille){debut=(fin + 1) % taille;}
	debut = (debut + taille) % (taille + 1);
	(*donnees)[debut] = element;
	if (nb_elem < taille) {
		nb_elem++;
	}
	if (nb_elem == taille) {
		fin = (debut + taille) % (taille + 1);
	}
}
/*
 //-------------------------------------------------------
 //  NEXT
 //
 uint8_t next(uint8_t I){
 uint8_t i=I+1;
 if (i==_qu.nb_elem)return 0; else return i;
 }
 //-------------------------------------------------------
 //  PREV
 //
 uint8_t prev(uint8_t I){
 uint8_t i=I-1;
 if (i<0)return _qu.nb_elem-1; else return i;
 }
 */
////////////////////////////////////////////////////////////////
#endif //__000_QUEUE_RING___
////////////////////////////////////////////////////////////////
