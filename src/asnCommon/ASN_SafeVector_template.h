#ifndef _ASN_SAFEVECTOR_
#define _ASN_SAFEVECTOR_

#include <vector>

template< class T > class ASN_SafeVector
{
public:

UINT AddItem( T* param_value){

    for( UINT i=0; i<internal_List.size(); ++i){

	if(internal_List[i]==NULL){
			internal_List[i] = param_value;
			return (i);
		}
	}

	internal_List.push_back(param_value);
	return internal_List.size()-1;
};

/// полный размер
int GetSize(){
    return internal_List.size();
};

/// число ненулевых элементов
int GetCount(){
    int cnt=0;
    for( UINT i=0; i<internal_List.size(); ++i) if(internal_List[i]) cnt++;
    return cnt;
};

const T* GetItem(UINT id){
    if ( internal_List.size()==0 ) return NULL;
    if( id > (UINT)internal_List.size() || internal_List[id] == NULL ) return NULL;
    return internal_List[id];
};

void DelItem(UINT id){

    if( id > (UINT)internal_List.size() || internal_List[id] == NULL ) return;
    internal_List[id]=NULL;

};

protected:

 std::vector< T* > internal_List;

};

//typedef ASN_List< std::string > ASN_StringList;

#endif
