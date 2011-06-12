%begin %{
#define WIN32_LEAN_AND_MEAN

#include "../OUEDefs.h"
inline void* operator new(size_t count){
	return OneU::GetAllocator().alloc(count);
}
inline void operator delete(void* p, size_t){
	OneU::GetAllocator().dealloc(p);
}
%}

#ifdef SWIGLUA
%runtime %{
int SWIGEX_Lua_Ownership(lua_State* L, int index){
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
	if (SWIGEX_Lua_Ownership(L, $input) != 1)
		SWIG_fail_arg("$symname", $argnum, "The value must have ownership!");
	if (!SWIG_IsOK(SWIG_ConvertPtr(L,$input,(void**)&$1,$descriptor,SWIG_POINTER_DISOWN))){
    SWIG_fail_ptr("$symname",$argnum,$descriptor);
    }
%}
//%typemap(in,checkfn="SWIG_isptrtype") SWIGTYPE* VOWN,SWIGTYPE VOWN[]
//%{
////	SWIGEX_Lua_Ownership(L, $input) = 1;
//	if (!SWIG_IsOK(SWIG_ConvertPtr(L,$input,(void**)&$1,$descriptor,0))){
//    SWIG_fail_ptr("$symname",$argnum,$descriptor);
//    }
//%}
#else
%typemap(in,checkfn="SWIG_isptrtype") SWIGTYPE* VDISOWN,SWIGTYPE VDISOWN[] = SWIGTYPE* DISOWN;
#endif