%begin %{
#define WIN32_LEAN_AND_MEAN
#pragma warning(disable : 4312)
#pragma warning(disable : 4311)
#pragma warning(disable : 4267)
%}

#ifdef SWIGRUBY
#define SUPPORT_DIRECTORS
#endif


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
%typemap(in,checkfn="SWIG_isptrtype") SWIGTYPE* VDISOWN{  
        if (SWIGEX_Lua_Ownership(L, $input) != 1)
                SWIG_fail_arg("$symname", $argnum, "The value must have ownership!");
        if (!SWIG_IsOK(SWIG_ConvertPtr(L,$input,(void**)&$1,$descriptor,SWIG_POINTER_DISOWN))){
    SWIG_fail_ptr("$symname",$argnum,$descriptor);
    }
}

%typemap(check) SWIGTYPE* NONNULL{
	if($1 == NULL)
		SWIG_fail_ptr("$symname",$argnum,$descriptor);
}

//unsupported
%define PROP_R(getter, prop)
%enddef
%define PROP_RW(setter, getter, prop)
%enddef
#elif defined(SWIGRUBY)
//find in typemaps\swigtype.swg
%typemap(in) SWIGTYPE* VDISOWN (int res = 0, swig_owntype own){
  res = SWIG_ConvertPtrAndOwn($input, %as_voidptrptr(&$1), $descriptor, SWIG_POINTER_DISOWN | %convertptr_flags, &own);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res,"$type", $symname, $argnum);
  }
  if (own == 0 || own == SWIG_RubyRemoveTracking)//no ownership
        SWIG_exception_fail(SWIG_ValueError, Ruby_Format_TypeError("The value must have ownership!", "$type","$symname", $argnum, $input));
}
%typemap(check) SWIGTYPE* NONNULL{
	if($1 == NULL)
		SWIG_exception_fail(SWIG_ValueError, "Expected a non-null pointer! Arg #$argnum in $symname");
}

%define PROP_R(getter, prop)
%rename(#prop) getter;
%enddef
%define PROP_RW(getter, setter, prop)
PROP_R(getter, prop)
%rename(#prop"=") setter;
%enddef
#endif
