%begin %{
#include "../OUEDefs.h"
inline void* operator new(size_t count){
	return OneU::GetAllocator().alloc(count);
}
inline void operator delete(void* p, size_t){
	OneU::GetAllocator().dealloc(p);
}
%}

%runtime %{
int SWIGEX_Lua_Disown(lua_State* L, int index){
  swig_lua_userdata* usr;
  if (lua_isnil(L,index)) return -1;
  usr=(swig_lua_userdata*)lua_touserdata(L,index);  /* get data */
  if (usr)
  {
    return usr->own;
  }
  return -1;
}
%}
%typemap(in,checkfn="SWIG_isptrtype") SWIGTYPE* VDISOWN,SWIGTYPE VDISOWN[]
%{  
	if (SWIGEX_Lua_Disown(L, $input) != 1)
		SWIG_fail_arg("$symname", $argnum, "The value must have ownership!");
	if (!SWIG_IsOK(SWIG_ConvertPtr(L,$input,(void**)&$1,$descriptor,SWIG_POINTER_DISOWN))){
    SWIG_fail_ptr("$symname",$argnum,$descriptor);
    }
%}
