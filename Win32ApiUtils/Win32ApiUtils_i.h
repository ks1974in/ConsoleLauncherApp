

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 08:44:07 2038
 */
/* Compiler settings for Win32ApiUtils.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Win32ApiUtils_i_h__
#define __Win32ApiUtils_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IProcessManager_FWD_DEFINED__
#define __IProcessManager_FWD_DEFINED__
typedef interface IProcessManager IProcessManager;

#endif 	/* __IProcessManager_FWD_DEFINED__ */


#ifndef __ProcessManager_FWD_DEFINED__
#define __ProcessManager_FWD_DEFINED__

#ifdef __cplusplus
typedef class ProcessManager ProcessManager;
#else
typedef struct ProcessManager ProcessManager;
#endif /* __cplusplus */

#endif 	/* __ProcessManager_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IProcessManager_INTERFACE_DEFINED__
#define __IProcessManager_INTERFACE_DEFINED__

/* interface IProcessManager */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IProcessManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d97b964c-4493-4f6c-9936-215af55e65be")
    IProcessManager : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LaunchProcessAsActiveUser( 
            /* [in] */ BSTR processName,
            /* [retval][out] */ LONG *dwProcessId) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IProcessManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProcessManager * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProcessManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProcessManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IProcessManager * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IProcessManager * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IProcessManager * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IProcessManager * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LaunchProcessAsActiveUser )( 
            IProcessManager * This,
            /* [in] */ BSTR processName,
            /* [retval][out] */ LONG *dwProcessId);
        
        END_INTERFACE
    } IProcessManagerVtbl;

    interface IProcessManager
    {
        CONST_VTBL struct IProcessManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProcessManager_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IProcessManager_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IProcessManager_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IProcessManager_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IProcessManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IProcessManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IProcessManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IProcessManager_LaunchProcessAsActiveUser(This,processName,dwProcessId)	\
    ( (This)->lpVtbl -> LaunchProcessAsActiveUser(This,processName,dwProcessId) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IProcessManager_INTERFACE_DEFINED__ */



#ifndef __Win32ApiUtilsLib_LIBRARY_DEFINED__
#define __Win32ApiUtilsLib_LIBRARY_DEFINED__

/* library Win32ApiUtilsLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Win32ApiUtilsLib;

EXTERN_C const CLSID CLSID_ProcessManager;

#ifdef __cplusplus

class DECLSPEC_UUID("e07bbe6a-bbf4-4edc-bae2-92b6521cf194")
ProcessManager;
#endif
#endif /* __Win32ApiUtilsLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


