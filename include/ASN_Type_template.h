#ifndef _ASN_TYPE_TEMPLATE_
#define _ASN_TYPE_TEMPLATE_

#include <vector>

template< class T > class ASN_OBJECT_LIST
{

public:

UINT Add(T* _object);
const T* Get(UINT id);
const T* GetByName(const char *_name);
UINT GetSize();
void Del(UINT id);

protected:

 std::vector< T* > internal_List;

};

#endif
