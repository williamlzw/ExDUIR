

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.xx.xxxx */
/* at a redacted point in time
 */
/* Compiler settings for ../../edge_embedded_browser/client/win/current/webview2experimental.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.xx.xxxx 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __webview2experimental_h__
#define __webview2experimental_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICoreWebView2Experimental_FWD_DEFINED__
#define __ICoreWebView2Experimental_FWD_DEFINED__
typedef interface ICoreWebView2Experimental ICoreWebView2Experimental;

#endif 	/* __ICoreWebView2Experimental_FWD_DEFINED__ */


#ifndef __ICoreWebView2Experimental2_FWD_DEFINED__
#define __ICoreWebView2Experimental2_FWD_DEFINED__
typedef interface ICoreWebView2Experimental2 ICoreWebView2Experimental2;

#endif 	/* __ICoreWebView2Experimental2_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalBytesReceivedChangedEventHandler ICoreWebView2ExperimentalBytesReceivedChangedEventHandler;

#endif 	/* __ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalCompositionControllerInterop_FWD_DEFINED__
#define __ICoreWebView2ExperimentalCompositionControllerInterop_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalCompositionControllerInterop ICoreWebView2ExperimentalCompositionControllerInterop;

#endif 	/* __ICoreWebView2ExperimentalCompositionControllerInterop_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalCompositionController3_FWD_DEFINED__
#define __ICoreWebView2ExperimentalCompositionController3_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalCompositionController3 ICoreWebView2ExperimentalCompositionController3;

#endif 	/* __ICoreWebView2ExperimentalCompositionController3_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalCompositionController4_FWD_DEFINED__
#define __ICoreWebView2ExperimentalCompositionController4_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalCompositionController4 ICoreWebView2ExperimentalCompositionController4;

#endif 	/* __ICoreWebView2ExperimentalCompositionController4_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalController_FWD_DEFINED__
#define __ICoreWebView2ExperimentalController_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalController ICoreWebView2ExperimentalController;

#endif 	/* __ICoreWebView2ExperimentalController_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalDownloadOperation_FWD_DEFINED__
#define __ICoreWebView2ExperimentalDownloadOperation_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalDownloadOperation ICoreWebView2ExperimentalDownloadOperation;

#endif 	/* __ICoreWebView2ExperimentalDownloadOperation_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalDownloadStartingEventArgs_FWD_DEFINED__
#define __ICoreWebView2ExperimentalDownloadStartingEventArgs_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalDownloadStartingEventArgs ICoreWebView2ExperimentalDownloadStartingEventArgs;

#endif 	/* __ICoreWebView2ExperimentalDownloadStartingEventArgs_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalDownloadStartingEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalDownloadStartingEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalDownloadStartingEventHandler ICoreWebView2ExperimentalDownloadStartingEventHandler;

#endif 	/* __ICoreWebView2ExperimentalDownloadStartingEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEnvironment2_FWD_DEFINED__
#define __ICoreWebView2ExperimentalEnvironment2_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalEnvironment2 ICoreWebView2ExperimentalEnvironment2;

#endif 	/* __ICoreWebView2ExperimentalEnvironment2_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEnvironment3_FWD_DEFINED__
#define __ICoreWebView2ExperimentalEnvironment3_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalEnvironment3 ICoreWebView2ExperimentalEnvironment3;

#endif 	/* __ICoreWebView2ExperimentalEnvironment3_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEnvironmentInterop_FWD_DEFINED__
#define __ICoreWebView2ExperimentalEnvironmentInterop_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalEnvironmentInterop ICoreWebView2ExperimentalEnvironmentInterop;

#endif 	/* __ICoreWebView2ExperimentalEnvironmentInterop_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler;

#endif 	/* __ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrame_FWD_DEFINED__
#define __ICoreWebView2ExperimentalFrame_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalFrame ICoreWebView2ExperimentalFrame;

#endif 	/* __ICoreWebView2ExperimentalFrame_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameCreatedEventArgs_FWD_DEFINED__
#define __ICoreWebView2ExperimentalFrameCreatedEventArgs_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalFrameCreatedEventArgs ICoreWebView2ExperimentalFrameCreatedEventArgs;

#endif 	/* __ICoreWebView2ExperimentalFrameCreatedEventArgs_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameCreatedEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalFrameCreatedEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalFrameCreatedEventHandler ICoreWebView2ExperimentalFrameCreatedEventHandler;

#endif 	/* __ICoreWebView2ExperimentalFrameCreatedEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameDestroyedEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalFrameDestroyedEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalFrameDestroyedEventHandler ICoreWebView2ExperimentalFrameDestroyedEventHandler;

#endif 	/* __ICoreWebView2ExperimentalFrameDestroyedEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameNameChangedEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalFrameNameChangedEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalFrameNameChangedEventHandler ICoreWebView2ExperimentalFrameNameChangedEventHandler;

#endif 	/* __ICoreWebView2ExperimentalFrameNameChangedEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler;

#endif 	/* __ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings_FWD_DEFINED__
#define __ICoreWebView2ExperimentalSettings_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalSettings ICoreWebView2ExperimentalSettings;

#endif 	/* __ICoreWebView2ExperimentalSettings_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings2_FWD_DEFINED__
#define __ICoreWebView2ExperimentalSettings2_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalSettings2 ICoreWebView2ExperimentalSettings2;

#endif 	/* __ICoreWebView2ExperimentalSettings2_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings3_FWD_DEFINED__
#define __ICoreWebView2ExperimentalSettings3_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalSettings3 ICoreWebView2ExperimentalSettings3;

#endif 	/* __ICoreWebView2ExperimentalSettings3_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings4_FWD_DEFINED__
#define __ICoreWebView2ExperimentalSettings4_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalSettings4 ICoreWebView2ExperimentalSettings4;

#endif 	/* __ICoreWebView2ExperimentalSettings4_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalStateChangedEventHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalStateChangedEventHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalStateChangedEventHandler ICoreWebView2ExperimentalStateChangedEventHandler;

#endif 	/* __ICoreWebView2ExperimentalStateChangedEventHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_FWD_DEFINED__
#define __ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler;

#endif 	/* __ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_FWD_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalUpdateRuntimeResult_FWD_DEFINED__
#define __ICoreWebView2ExperimentalUpdateRuntimeResult_FWD_DEFINED__
typedef interface ICoreWebView2ExperimentalUpdateRuntimeResult ICoreWebView2ExperimentalUpdateRuntimeResult;

#endif 	/* __ICoreWebView2ExperimentalUpdateRuntimeResult_FWD_DEFINED__ */


/* header files for imported files */
#include "webview2.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __WebView2Experimental_LIBRARY_DEFINED__
#define __WebView2Experimental_LIBRARY_DEFINED__

/* library WebView2Experimental */
/* [version][uuid] */ 




























typedef struct COREWEBVIEW2_MATRIX_4X4
    {
    FLOAT _11;
    FLOAT _12;
    FLOAT _13;
    FLOAT _14;
    FLOAT _21;
    FLOAT _22;
    FLOAT _23;
    FLOAT _24;
    FLOAT _31;
    FLOAT _32;
    FLOAT _33;
    FLOAT _34;
    FLOAT _41;
    FLOAT _42;
    FLOAT _43;
    FLOAT _44;
    } 	COREWEBVIEW2_MATRIX_4X4;

typedef /* [v1_enum] */ 
enum COREWEBVIEW2_UPDATE_RUNTIME_STATUS
    {
        COREWEBVIEW2_UPDATE_RUNTIME_STATUS_LATEST_VERSION_INSTALLED	= 0,
        COREWEBVIEW2_UPDATE_RUNTIME_STATUS_UPDATE_ALREADY_RUNNING	= ( COREWEBVIEW2_UPDATE_RUNTIME_STATUS_LATEST_VERSION_INSTALLED + 1 ) ,
        COREWEBVIEW2_UPDATE_RUNTIME_STATUS_BLOCKED_BY_POLICY	= ( COREWEBVIEW2_UPDATE_RUNTIME_STATUS_UPDATE_ALREADY_RUNNING + 1 ) ,
        COREWEBVIEW2_UPDATE_RUNTIME_STATUS_FAILED	= ( COREWEBVIEW2_UPDATE_RUNTIME_STATUS_BLOCKED_BY_POLICY + 1 ) 
    } 	COREWEBVIEW2_UPDATE_RUNTIME_STATUS;

typedef 
enum COREWEBVIEW2_DOWNLOAD_STATE
    {
        COREWEBVIEW2_DOWNLOAD_STATE_IN_PROGRESS	= 0,
        COREWEBVIEW2_DOWNLOAD_STATE_INTERRUPTED	= ( COREWEBVIEW2_DOWNLOAD_STATE_IN_PROGRESS + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_STATE_COMPLETED	= ( COREWEBVIEW2_DOWNLOAD_STATE_INTERRUPTED + 1 ) 
    } 	COREWEBVIEW2_DOWNLOAD_STATE;

typedef 
enum COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON
    {
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NONE	= 0,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_FAILED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NONE + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_ACCESS_DENIED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_FAILED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_NO_SPACE	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_ACCESS_DENIED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_NAME_TOO_LONG	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_NO_SPACE + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_TOO_LARGE	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_NAME_TOO_LONG + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_MALICIOUS	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_TOO_LARGE + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_TRANSIENT_ERROR	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_MALICIOUS + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_BLOCKED_BY_POLICY	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_TRANSIENT_ERROR + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_SECURITY_CHECK_FAILED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_BLOCKED_BY_POLICY + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_TOO_SHORT	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_SECURITY_CHECK_FAILED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_HASH_MISMATCH	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_TOO_SHORT + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_FAILED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_FILE_HASH_MISMATCH + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_TIMEOUT	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_FAILED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_DISCONNECTED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_TIMEOUT + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_SERVER_DOWN	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_DISCONNECTED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_SERVER_DOWN + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_FAILED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_NETWORK_INVALID_REQUEST + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_NO_RANGE	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_FAILED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_BAD_CONTENT	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_NO_RANGE + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_UNAUTHORIZED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_BAD_CONTENT + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_CERTIFICATE_PROBLEM	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_UNAUTHORIZED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_FORBIDDEN	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_CERTIFICATE_PROBLEM + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_UNEXPECTED_RESPONSE	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_FORBIDDEN + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_CONTENT_LENGTH_MISMATCH	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_UNEXPECTED_RESPONSE + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_CROSS_ORIGIN_REDIRECT	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_CONTENT_LENGTH_MISMATCH + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_USER_CANCELED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_SERVER_CROSS_ORIGIN_REDIRECT + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_USER_SHUTDOWN	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_USER_CANCELED + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_USER_PAUSED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_USER_SHUTDOWN + 1 ) ,
        COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_DOWNLOAD_PROCESS_CRASHED	= ( COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON_USER_PAUSED + 1 ) 
    } 	COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON;


EXTERN_C const IID LIBID_WebView2Experimental;

#ifndef __ICoreWebView2Experimental_INTERFACE_DEFINED__
#define __ICoreWebView2Experimental_INTERFACE_DEFINED__

/* interface ICoreWebView2Experimental */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2Experimental = {0xa3d0c16a,0x9ba9,0x11eb,{0xa8,0xb3,0x02,0x42,0xac,0x13,0x00,0x03}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a3d0c16a-9ba9-11eb-a8b3-0242ac130003")
    ICoreWebView2Experimental : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE add_FrameCreated( 
            /* [in] */ ICoreWebView2ExperimentalFrameCreatedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_FrameCreated( 
            /* [in] */ EventRegistrationToken token) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2Experimental * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2Experimental * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2Experimental * This);
        
        HRESULT ( STDMETHODCALLTYPE *add_FrameCreated )( 
            ICoreWebView2Experimental * This,
            /* [in] */ ICoreWebView2ExperimentalFrameCreatedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_FrameCreated )( 
            ICoreWebView2Experimental * This,
            /* [in] */ EventRegistrationToken token);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalVtbl;

    interface ICoreWebView2Experimental
    {
        CONST_VTBL struct ICoreWebView2ExperimentalVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2Experimental_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2Experimental_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2Experimental_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2Experimental_add_FrameCreated(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_FrameCreated(This,eventHandler,token) ) 

#define ICoreWebView2Experimental_remove_FrameCreated(This,token)	\
    ( (This)->lpVtbl -> remove_FrameCreated(This,token) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2Experimental_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2Experimental2_INTERFACE_DEFINED__
#define __ICoreWebView2Experimental2_INTERFACE_DEFINED__

/* interface ICoreWebView2Experimental2 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2Experimental2 = {0x84e547a6,0x2cf7,0x4966,{0xa4,0x4e,0x3d,0xd1,0x0b,0xE6,0x9a,0x7f}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("84e547a6-2cf7-4966-a44e-3dd10bE69a7f")
    ICoreWebView2Experimental2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE add_DownloadStarting( 
            /* [in] */ ICoreWebView2ExperimentalDownloadStartingEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_DownloadStarting( 
            /* [in] */ EventRegistrationToken token) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2Experimental2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2Experimental2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2Experimental2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2Experimental2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *add_DownloadStarting )( 
            ICoreWebView2Experimental2 * This,
            /* [in] */ ICoreWebView2ExperimentalDownloadStartingEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_DownloadStarting )( 
            ICoreWebView2Experimental2 * This,
            /* [in] */ EventRegistrationToken token);
        
        END_INTERFACE
    } ICoreWebView2Experimental2Vtbl;

    interface ICoreWebView2Experimental2
    {
        CONST_VTBL struct ICoreWebView2Experimental2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2Experimental2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2Experimental2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2Experimental2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2Experimental2_add_DownloadStarting(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_DownloadStarting(This,eventHandler,token) ) 

#define ICoreWebView2Experimental2_remove_DownloadStarting(This,token)	\
    ( (This)->lpVtbl -> remove_DownloadStarting(This,token) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2Experimental2_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalBytesReceivedChangedEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalBytesReceivedChangedEventHandler = {0x828e8ab6,0xd94c,0x4264,{0x9c,0xef,0x52,0x17,0x17,0x0d,0x62,0x51}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("828e8ab6-d94c-4264-9cef-5217170d6251")
    ICoreWebView2ExperimentalBytesReceivedChangedEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2ExperimentalDownloadOperation *sender,
            /* [in] */ IUnknown *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalBytesReceivedChangedEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalBytesReceivedChangedEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalBytesReceivedChangedEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalBytesReceivedChangedEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalBytesReceivedChangedEventHandler * This,
            /* [in] */ ICoreWebView2ExperimentalDownloadOperation *sender,
            /* [in] */ IUnknown *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalBytesReceivedChangedEventHandlerVtbl;

    interface ICoreWebView2ExperimentalBytesReceivedChangedEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalBytesReceivedChangedEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalBytesReceivedChangedEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalCompositionControllerInterop_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalCompositionControllerInterop_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalCompositionControllerInterop */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalCompositionControllerInterop = {0x4B60F2C9,0x88BB,0x42F4,{0x9C,0x4F,0x3C,0x0D,0x0E,0xD1,0x70,0x72}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4B60F2C9-88BB-42F4-9C4F-3C0D0ED17072")
    ICoreWebView2ExperimentalCompositionControllerInterop : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_UIAProvider( 
            /* [retval][out] */ IUnknown **provider) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RootVisualTarget( 
            /* [retval][out] */ IUnknown **target) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_RootVisualTarget( 
            /* [in] */ IUnknown *target) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalCompositionControllerInteropVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalCompositionControllerInterop * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalCompositionControllerInterop * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalCompositionControllerInterop * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_UIAProvider )( 
            ICoreWebView2ExperimentalCompositionControllerInterop * This,
            /* [retval][out] */ IUnknown **provider);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RootVisualTarget )( 
            ICoreWebView2ExperimentalCompositionControllerInterop * This,
            /* [retval][out] */ IUnknown **target);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_RootVisualTarget )( 
            ICoreWebView2ExperimentalCompositionControllerInterop * This,
            /* [in] */ IUnknown *target);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalCompositionControllerInteropVtbl;

    interface ICoreWebView2ExperimentalCompositionControllerInterop
    {
        CONST_VTBL struct ICoreWebView2ExperimentalCompositionControllerInteropVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalCompositionControllerInterop_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalCompositionControllerInterop_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalCompositionControllerInterop_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalCompositionControllerInterop_get_UIAProvider(This,provider)	\
    ( (This)->lpVtbl -> get_UIAProvider(This,provider) ) 

#define ICoreWebView2ExperimentalCompositionControllerInterop_get_RootVisualTarget(This,target)	\
    ( (This)->lpVtbl -> get_RootVisualTarget(This,target) ) 

#define ICoreWebView2ExperimentalCompositionControllerInterop_put_RootVisualTarget(This,target)	\
    ( (This)->lpVtbl -> put_RootVisualTarget(This,target) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalCompositionControllerInterop_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalCompositionController3_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalCompositionController3_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalCompositionController3 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalCompositionController3 = {0xb134916b,0xa104,0x4d2a,{0x95,0x67,0xc2,0xfd,0x53,0x71,0x43,0x50}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b134916b-a104-4d2a-9567-c2fd53714350")
    ICoreWebView2ExperimentalCompositionController3 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DragEnter( 
            /* [in] */ IDataObject *dataObject,
            /* [in] */ DWORD keyState,
            /* [in] */ POINT point,
            /* [retval][out] */ DWORD *effect) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DragLeave( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DragOver( 
            /* [in] */ DWORD keyState,
            /* [in] */ POINT point,
            /* [retval][out] */ DWORD *effect) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Drop( 
            /* [in] */ IDataObject *dataObject,
            /* [in] */ DWORD keyState,
            /* [in] */ POINT point,
            /* [retval][out] */ DWORD *effect) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalCompositionController3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalCompositionController3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalCompositionController3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalCompositionController3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *DragEnter )( 
            ICoreWebView2ExperimentalCompositionController3 * This,
            /* [in] */ IDataObject *dataObject,
            /* [in] */ DWORD keyState,
            /* [in] */ POINT point,
            /* [retval][out] */ DWORD *effect);
        
        HRESULT ( STDMETHODCALLTYPE *DragLeave )( 
            ICoreWebView2ExperimentalCompositionController3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *DragOver )( 
            ICoreWebView2ExperimentalCompositionController3 * This,
            /* [in] */ DWORD keyState,
            /* [in] */ POINT point,
            /* [retval][out] */ DWORD *effect);
        
        HRESULT ( STDMETHODCALLTYPE *Drop )( 
            ICoreWebView2ExperimentalCompositionController3 * This,
            /* [in] */ IDataObject *dataObject,
            /* [in] */ DWORD keyState,
            /* [in] */ POINT point,
            /* [retval][out] */ DWORD *effect);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalCompositionController3Vtbl;

    interface ICoreWebView2ExperimentalCompositionController3
    {
        CONST_VTBL struct ICoreWebView2ExperimentalCompositionController3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalCompositionController3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalCompositionController3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalCompositionController3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalCompositionController3_DragEnter(This,dataObject,keyState,point,effect)	\
    ( (This)->lpVtbl -> DragEnter(This,dataObject,keyState,point,effect) ) 

#define ICoreWebView2ExperimentalCompositionController3_DragLeave(This)	\
    ( (This)->lpVtbl -> DragLeave(This) ) 

#define ICoreWebView2ExperimentalCompositionController3_DragOver(This,keyState,point,effect)	\
    ( (This)->lpVtbl -> DragOver(This,keyState,point,effect) ) 

#define ICoreWebView2ExperimentalCompositionController3_Drop(This,dataObject,keyState,point,effect)	\
    ( (This)->lpVtbl -> Drop(This,dataObject,keyState,point,effect) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalCompositionController3_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalCompositionController4_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalCompositionController4_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalCompositionController4 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalCompositionController4 = {0xe6041d7f,0x18ac,0x4654,{0xa0,0x4e,0x8b,0x3f,0x81,0x25,0x1c,0x33}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e6041d7f-18ac-4654-a04e-8b3f81251c33")
    ICoreWebView2ExperimentalCompositionController4 : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_UIAProvider( 
            /* [retval][out] */ IUnknown **provider) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateCoreWebView2PointerInfoFromPointerId( 
            /* [in] */ UINT pointerId,
            /* [in] */ HWND parentWindow,
            /* [in] */ struct COREWEBVIEW2_MATRIX_4X4 transform,
            /* [retval][out] */ ICoreWebView2PointerInfo **pointerInfo) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalCompositionController4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalCompositionController4 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalCompositionController4 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalCompositionController4 * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_UIAProvider )( 
            ICoreWebView2ExperimentalCompositionController4 * This,
            /* [retval][out] */ IUnknown **provider);
        
        HRESULT ( STDMETHODCALLTYPE *CreateCoreWebView2PointerInfoFromPointerId )( 
            ICoreWebView2ExperimentalCompositionController4 * This,
            /* [in] */ UINT pointerId,
            /* [in] */ HWND parentWindow,
            /* [in] */ struct COREWEBVIEW2_MATRIX_4X4 transform,
            /* [retval][out] */ ICoreWebView2PointerInfo **pointerInfo);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalCompositionController4Vtbl;

    interface ICoreWebView2ExperimentalCompositionController4
    {
        CONST_VTBL struct ICoreWebView2ExperimentalCompositionController4Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalCompositionController4_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalCompositionController4_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalCompositionController4_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalCompositionController4_get_UIAProvider(This,provider)	\
    ( (This)->lpVtbl -> get_UIAProvider(This,provider) ) 

#define ICoreWebView2ExperimentalCompositionController4_CreateCoreWebView2PointerInfoFromPointerId(This,pointerId,parentWindow,transform,pointerInfo)	\
    ( (This)->lpVtbl -> CreateCoreWebView2PointerInfoFromPointerId(This,pointerId,parentWindow,transform,pointerInfo) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalCompositionController4_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalController_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalController_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalController */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalController = {0x3413543f,0x7a0e,0x4b56,{0x98,0xc1,0x7a,0xf3,0xa3,0x74,0x16,0xca}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3413543f-7a0e-4b56-98c1-7af3a37416ca")
    ICoreWebView2ExperimentalController : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RasterizationScale( 
            /* [retval][out] */ double *scale) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_RasterizationScale( 
            /* [in] */ double scale) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ShouldDetectMonitorScaleChanges( 
            /* [retval][out] */ BOOL *value) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ShouldDetectMonitorScaleChanges( 
            /* [in] */ BOOL value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_RasterizationScaleChanged( 
            /* [in] */ ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_RasterizationScaleChanged( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BoundsMode( 
            /* [retval][out] */ COREWEBVIEW2_BOUNDS_MODE *boundsMode) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BoundsMode( 
            /* [in] */ COREWEBVIEW2_BOUNDS_MODE boundsMode) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalControllerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalController * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalController * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalController * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_RasterizationScale )( 
            ICoreWebView2ExperimentalController * This,
            /* [retval][out] */ double *scale);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_RasterizationScale )( 
            ICoreWebView2ExperimentalController * This,
            /* [in] */ double scale);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShouldDetectMonitorScaleChanges )( 
            ICoreWebView2ExperimentalController * This,
            /* [retval][out] */ BOOL *value);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShouldDetectMonitorScaleChanges )( 
            ICoreWebView2ExperimentalController * This,
            /* [in] */ BOOL value);
        
        HRESULT ( STDMETHODCALLTYPE *add_RasterizationScaleChanged )( 
            ICoreWebView2ExperimentalController * This,
            /* [in] */ ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_RasterizationScaleChanged )( 
            ICoreWebView2ExperimentalController * This,
            /* [in] */ EventRegistrationToken token);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BoundsMode )( 
            ICoreWebView2ExperimentalController * This,
            /* [retval][out] */ COREWEBVIEW2_BOUNDS_MODE *boundsMode);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_BoundsMode )( 
            ICoreWebView2ExperimentalController * This,
            /* [in] */ COREWEBVIEW2_BOUNDS_MODE boundsMode);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalControllerVtbl;

    interface ICoreWebView2ExperimentalController
    {
        CONST_VTBL struct ICoreWebView2ExperimentalControllerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalController_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalController_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalController_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalController_get_RasterizationScale(This,scale)	\
    ( (This)->lpVtbl -> get_RasterizationScale(This,scale) ) 

#define ICoreWebView2ExperimentalController_put_RasterizationScale(This,scale)	\
    ( (This)->lpVtbl -> put_RasterizationScale(This,scale) ) 

#define ICoreWebView2ExperimentalController_get_ShouldDetectMonitorScaleChanges(This,value)	\
    ( (This)->lpVtbl -> get_ShouldDetectMonitorScaleChanges(This,value) ) 

#define ICoreWebView2ExperimentalController_put_ShouldDetectMonitorScaleChanges(This,value)	\
    ( (This)->lpVtbl -> put_ShouldDetectMonitorScaleChanges(This,value) ) 

#define ICoreWebView2ExperimentalController_add_RasterizationScaleChanged(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_RasterizationScaleChanged(This,eventHandler,token) ) 

#define ICoreWebView2ExperimentalController_remove_RasterizationScaleChanged(This,token)	\
    ( (This)->lpVtbl -> remove_RasterizationScaleChanged(This,token) ) 

#define ICoreWebView2ExperimentalController_get_BoundsMode(This,boundsMode)	\
    ( (This)->lpVtbl -> get_BoundsMode(This,boundsMode) ) 

#define ICoreWebView2ExperimentalController_put_BoundsMode(This,boundsMode)	\
    ( (This)->lpVtbl -> put_BoundsMode(This,boundsMode) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalController_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalDownloadOperation_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalDownloadOperation_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalDownloadOperation */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalDownloadOperation = {0x3d6b6cf2,0xafe1,0x44c7,{0xa9,0x95,0xc6,0x51,0x17,0x71,0x43,0x36}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3d6b6cf2-afe1-44c7-a995-c65117714336")
    ICoreWebView2ExperimentalDownloadOperation : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE add_BytesReceivedChanged( 
            /* [in] */ ICoreWebView2ExperimentalBytesReceivedChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_BytesReceivedChanged( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_EstimatedEndTimeChanged( 
            /* [in] */ ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_EstimatedEndTimeChanged( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_StateChanged( 
            /* [in] */ ICoreWebView2ExperimentalStateChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_StateChanged( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Uri( 
            /* [retval][out] */ LPWSTR *uri) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ContentDisposition( 
            /* [retval][out] */ LPWSTR *contentDisposition) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MimeType( 
            /* [retval][out] */ LPWSTR *mimeType) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_TotalBytesToReceive( 
            /* [retval][out] */ INT64 *totalBytesToReceive) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_BytesReceived( 
            /* [retval][out] */ INT64 *bytesReceived) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_EstimatedEndTime( 
            /* [retval][out] */ LPWSTR *estimatedEndTime) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ResultFilePath( 
            /* [retval][out] */ LPWSTR *resultFilePath) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_State( 
            /* [retval][out] */ COREWEBVIEW2_DOWNLOAD_STATE *downloadState) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InterruptReason( 
            /* [retval][out] */ COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON *interruptReason) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Resume( void) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_CanResume( 
            /* [retval][out] */ BOOL *canResume) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalDownloadOperationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalDownloadOperation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalDownloadOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *add_BytesReceivedChanged )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [in] */ ICoreWebView2ExperimentalBytesReceivedChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_BytesReceivedChanged )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [in] */ EventRegistrationToken token);
        
        HRESULT ( STDMETHODCALLTYPE *add_EstimatedEndTimeChanged )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [in] */ ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_EstimatedEndTimeChanged )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [in] */ EventRegistrationToken token);
        
        HRESULT ( STDMETHODCALLTYPE *add_StateChanged )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [in] */ ICoreWebView2ExperimentalStateChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_StateChanged )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [in] */ EventRegistrationToken token);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Uri )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ LPWSTR *uri);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ContentDisposition )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ LPWSTR *contentDisposition);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_MimeType )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ LPWSTR *mimeType);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_TotalBytesToReceive )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ INT64 *totalBytesToReceive);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_BytesReceived )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ INT64 *bytesReceived);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_EstimatedEndTime )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ LPWSTR *estimatedEndTime);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ResultFilePath )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ LPWSTR *resultFilePath);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_State )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ COREWEBVIEW2_DOWNLOAD_STATE *downloadState);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_InterruptReason )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ COREWEBVIEW2_DOWNLOAD_INTERRUPT_REASON *interruptReason);
        
        HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            ICoreWebView2ExperimentalDownloadOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *Pause )( 
            ICoreWebView2ExperimentalDownloadOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *Resume )( 
            ICoreWebView2ExperimentalDownloadOperation * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_CanResume )( 
            ICoreWebView2ExperimentalDownloadOperation * This,
            /* [retval][out] */ BOOL *canResume);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalDownloadOperationVtbl;

    interface ICoreWebView2ExperimentalDownloadOperation
    {
        CONST_VTBL struct ICoreWebView2ExperimentalDownloadOperationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalDownloadOperation_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalDownloadOperation_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalDownloadOperation_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalDownloadOperation_add_BytesReceivedChanged(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_BytesReceivedChanged(This,eventHandler,token) ) 

#define ICoreWebView2ExperimentalDownloadOperation_remove_BytesReceivedChanged(This,token)	\
    ( (This)->lpVtbl -> remove_BytesReceivedChanged(This,token) ) 

#define ICoreWebView2ExperimentalDownloadOperation_add_EstimatedEndTimeChanged(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_EstimatedEndTimeChanged(This,eventHandler,token) ) 

#define ICoreWebView2ExperimentalDownloadOperation_remove_EstimatedEndTimeChanged(This,token)	\
    ( (This)->lpVtbl -> remove_EstimatedEndTimeChanged(This,token) ) 

#define ICoreWebView2ExperimentalDownloadOperation_add_StateChanged(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_StateChanged(This,eventHandler,token) ) 

#define ICoreWebView2ExperimentalDownloadOperation_remove_StateChanged(This,token)	\
    ( (This)->lpVtbl -> remove_StateChanged(This,token) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_Uri(This,uri)	\
    ( (This)->lpVtbl -> get_Uri(This,uri) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_ContentDisposition(This,contentDisposition)	\
    ( (This)->lpVtbl -> get_ContentDisposition(This,contentDisposition) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_MimeType(This,mimeType)	\
    ( (This)->lpVtbl -> get_MimeType(This,mimeType) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_TotalBytesToReceive(This,totalBytesToReceive)	\
    ( (This)->lpVtbl -> get_TotalBytesToReceive(This,totalBytesToReceive) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_BytesReceived(This,bytesReceived)	\
    ( (This)->lpVtbl -> get_BytesReceived(This,bytesReceived) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_EstimatedEndTime(This,estimatedEndTime)	\
    ( (This)->lpVtbl -> get_EstimatedEndTime(This,estimatedEndTime) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_ResultFilePath(This,resultFilePath)	\
    ( (This)->lpVtbl -> get_ResultFilePath(This,resultFilePath) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_State(This,downloadState)	\
    ( (This)->lpVtbl -> get_State(This,downloadState) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_InterruptReason(This,interruptReason)	\
    ( (This)->lpVtbl -> get_InterruptReason(This,interruptReason) ) 

#define ICoreWebView2ExperimentalDownloadOperation_Cancel(This)	\
    ( (This)->lpVtbl -> Cancel(This) ) 

#define ICoreWebView2ExperimentalDownloadOperation_Pause(This)	\
    ( (This)->lpVtbl -> Pause(This) ) 

#define ICoreWebView2ExperimentalDownloadOperation_Resume(This)	\
    ( (This)->lpVtbl -> Resume(This) ) 

#define ICoreWebView2ExperimentalDownloadOperation_get_CanResume(This,canResume)	\
    ( (This)->lpVtbl -> get_CanResume(This,canResume) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalDownloadOperation_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalDownloadStartingEventArgs_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalDownloadStartingEventArgs_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalDownloadStartingEventArgs */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalDownloadStartingEventArgs = {0xe99bbe21,0x43e9,0x4544,{0xa7,0x32,0x28,0x27,0x64,0xea,0xfa,0x60}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e99bbe21-43e9-4544-a732-282764eafa60")
    ICoreWebView2ExperimentalDownloadStartingEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DownloadOperation( 
            /* [retval][out] */ ICoreWebView2ExperimentalDownloadOperation **downloadOperation) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Cancel( 
            /* [retval][out] */ BOOL *cancel) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Cancel( 
            /* [in] */ BOOL cancel) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ResultFilePath( 
            /* [retval][out] */ LPWSTR *resultFilePath) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ResultFilePath( 
            /* [in] */ LPCWSTR resultFilePath) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Handled( 
            /* [retval][out] */ BOOL *handled) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Handled( 
            /* [in] */ BOOL handled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDeferral( 
            /* [retval][out] */ ICoreWebView2Deferral **deferral) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalDownloadStartingEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_DownloadOperation )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [retval][out] */ ICoreWebView2ExperimentalDownloadOperation **downloadOperation);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cancel )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [retval][out] */ BOOL *cancel);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Cancel )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [in] */ BOOL cancel);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ResultFilePath )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [retval][out] */ LPWSTR *resultFilePath);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_ResultFilePath )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [in] */ LPCWSTR resultFilePath);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Handled )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [retval][out] */ BOOL *handled);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_Handled )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [in] */ BOOL handled);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeferral )( 
            ICoreWebView2ExperimentalDownloadStartingEventArgs * This,
            /* [retval][out] */ ICoreWebView2Deferral **deferral);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalDownloadStartingEventArgsVtbl;

    interface ICoreWebView2ExperimentalDownloadStartingEventArgs
    {
        CONST_VTBL struct ICoreWebView2ExperimentalDownloadStartingEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalDownloadStartingEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalDownloadStartingEventArgs_get_DownloadOperation(This,downloadOperation)	\
    ( (This)->lpVtbl -> get_DownloadOperation(This,downloadOperation) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_get_Cancel(This,cancel)	\
    ( (This)->lpVtbl -> get_Cancel(This,cancel) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_put_Cancel(This,cancel)	\
    ( (This)->lpVtbl -> put_Cancel(This,cancel) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_get_ResultFilePath(This,resultFilePath)	\
    ( (This)->lpVtbl -> get_ResultFilePath(This,resultFilePath) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_put_ResultFilePath(This,resultFilePath)	\
    ( (This)->lpVtbl -> put_ResultFilePath(This,resultFilePath) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_get_Handled(This,handled)	\
    ( (This)->lpVtbl -> get_Handled(This,handled) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_put_Handled(This,handled)	\
    ( (This)->lpVtbl -> put_Handled(This,handled) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventArgs_GetDeferral(This,deferral)	\
    ( (This)->lpVtbl -> GetDeferral(This,deferral) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalDownloadStartingEventArgs_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalDownloadStartingEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalDownloadStartingEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalDownloadStartingEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalDownloadStartingEventHandler = {0xefedc989,0xc396,0x41ca,{0x83,0xf7,0x07,0xf8,0x45,0xa5,0x57,0x24}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("efedc989-c396-41ca-83f7-07f845a55724")
    ICoreWebView2ExperimentalDownloadStartingEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2 *sender,
            /* [in] */ ICoreWebView2ExperimentalDownloadStartingEventArgs *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalDownloadStartingEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalDownloadStartingEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalDownloadStartingEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalDownloadStartingEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalDownloadStartingEventHandler * This,
            /* [in] */ ICoreWebView2 *sender,
            /* [in] */ ICoreWebView2ExperimentalDownloadStartingEventArgs *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalDownloadStartingEventHandlerVtbl;

    interface ICoreWebView2ExperimentalDownloadStartingEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalDownloadStartingEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalDownloadStartingEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalDownloadStartingEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalDownloadStartingEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalDownloadStartingEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEnvironment2_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalEnvironment2_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalEnvironment2 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalEnvironment2 = {0x37b54fd4,0x1ad9,0x4c1f,{0xbd,0x14,0x9d,0xab,0xa9,0xfd,0xeb,0x26}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("37b54fd4-1ad9-4c1f-bd14-9daba9fdeb26")
    ICoreWebView2ExperimentalEnvironment2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetProviderForHwnd( 
            /* [in] */ HWND hwnd,
            /* [retval][out] */ IUnknown **provider) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalEnvironment2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalEnvironment2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalEnvironment2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalEnvironment2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetProviderForHwnd )( 
            ICoreWebView2ExperimentalEnvironment2 * This,
            /* [in] */ HWND hwnd,
            /* [retval][out] */ IUnknown **provider);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalEnvironment2Vtbl;

    interface ICoreWebView2ExperimentalEnvironment2
    {
        CONST_VTBL struct ICoreWebView2ExperimentalEnvironment2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalEnvironment2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalEnvironment2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalEnvironment2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalEnvironment2_GetProviderForHwnd(This,hwnd,provider)	\
    ( (This)->lpVtbl -> GetProviderForHwnd(This,hwnd,provider) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalEnvironment2_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEnvironment3_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalEnvironment3_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalEnvironment3 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalEnvironment3 = {0x9A2BE885,0x7F0B,0x4B26,{0xB6,0xDD,0xC9,0x69,0xBA,0xA0,0x0B,0xF1}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9A2BE885-7F0B-4B26-B6DD-C969BAA00BF1")
    ICoreWebView2ExperimentalEnvironment3 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE UpdateRuntime( 
            /* [in] */ ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler *handler) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalEnvironment3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalEnvironment3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalEnvironment3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalEnvironment3 * This);
        
        HRESULT ( STDMETHODCALLTYPE *UpdateRuntime )( 
            ICoreWebView2ExperimentalEnvironment3 * This,
            /* [in] */ ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler *handler);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalEnvironment3Vtbl;

    interface ICoreWebView2ExperimentalEnvironment3
    {
        CONST_VTBL struct ICoreWebView2ExperimentalEnvironment3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalEnvironment3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalEnvironment3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalEnvironment3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalEnvironment3_UpdateRuntime(This,handler)	\
    ( (This)->lpVtbl -> UpdateRuntime(This,handler) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalEnvironment3_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEnvironmentInterop_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalEnvironmentInterop_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalEnvironmentInterop */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalEnvironmentInterop = {0x79455D4F,0xD28D,0x4C3F,{0xA7,0x13,0x13,0xCA,0x39,0x3B,0xD2,0xE4}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("79455D4F-D28D-4C3F-A713-13CA393BD2E4")
    ICoreWebView2ExperimentalEnvironmentInterop : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetProviderForHwnd( 
            /* [in] */ HWND hwnd,
            /* [retval][out] */ IUnknown **provider) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalEnvironmentInteropVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalEnvironmentInterop * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalEnvironmentInterop * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalEnvironmentInterop * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetProviderForHwnd )( 
            ICoreWebView2ExperimentalEnvironmentInterop * This,
            /* [in] */ HWND hwnd,
            /* [retval][out] */ IUnknown **provider);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalEnvironmentInteropVtbl;

    interface ICoreWebView2ExperimentalEnvironmentInterop
    {
        CONST_VTBL struct ICoreWebView2ExperimentalEnvironmentInteropVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalEnvironmentInterop_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalEnvironmentInterop_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalEnvironmentInterop_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalEnvironmentInterop_GetProviderForHwnd(This,hwnd,provider)	\
    ( (This)->lpVtbl -> GetProviderForHwnd(This,hwnd,provider) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalEnvironmentInterop_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler = {0x28f0d425,0x93fe,0x4e63,{0x9f,0x8d,0x2a,0xee,0xc6,0xd3,0xba,0x1e}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("28f0d425-93fe-4e63-9f8d-2aeec6d3ba1e")
    ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2ExperimentalDownloadOperation *sender,
            /* [in] */ IUnknown *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler * This,
            /* [in] */ ICoreWebView2ExperimentalDownloadOperation *sender,
            /* [in] */ IUnknown *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandlerVtbl;

    interface ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalEstimatedEndTimeChangedEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrame_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalFrame_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalFrame */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalFrame = {0xf1131a5e,0x9ba9,0x11eb,{0xa8,0xb3,0x02,0x42,0xac,0x13,0x00,0x03}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f1131a5e-9ba9-11eb-a8b3-0242ac130003")
    ICoreWebView2ExperimentalFrame : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ LPWSTR *name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_NameChanged( 
            /* [in] */ ICoreWebView2ExperimentalFrameNameChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_NameChanged( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddHostObjectToScriptWithOrigins( 
            /* [in] */ LPCWSTR name,
            /* [in] */ VARIANT *object,
            /* [in] */ UINT32 originsCount,
            /* [size_is][in] */ LPCWSTR *origins) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveHostObjectFromScript( 
            /* [in] */ LPCWSTR name) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_Destroyed( 
            /* [in] */ ICoreWebView2ExperimentalFrameDestroyedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_Destroyed( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE IsDestroyed( 
            /* [retval][out] */ BOOL *destroyed) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalFrame * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [retval][out] */ LPWSTR *name);
        
        HRESULT ( STDMETHODCALLTYPE *add_NameChanged )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [in] */ ICoreWebView2ExperimentalFrameNameChangedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_NameChanged )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [in] */ EventRegistrationToken token);
        
        HRESULT ( STDMETHODCALLTYPE *AddHostObjectToScriptWithOrigins )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [in] */ LPCWSTR name,
            /* [in] */ VARIANT *object,
            /* [in] */ UINT32 originsCount,
            /* [size_is][in] */ LPCWSTR *origins);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveHostObjectFromScript )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [in] */ LPCWSTR name);
        
        HRESULT ( STDMETHODCALLTYPE *add_Destroyed )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [in] */ ICoreWebView2ExperimentalFrameDestroyedEventHandler *eventHandler,
            /* [out] */ EventRegistrationToken *token);
        
        HRESULT ( STDMETHODCALLTYPE *remove_Destroyed )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [in] */ EventRegistrationToken token);
        
        HRESULT ( STDMETHODCALLTYPE *IsDestroyed )( 
            ICoreWebView2ExperimentalFrame * This,
            /* [retval][out] */ BOOL *destroyed);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalFrameVtbl;

    interface ICoreWebView2ExperimentalFrame
    {
        CONST_VTBL struct ICoreWebView2ExperimentalFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalFrame_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalFrame_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalFrame_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalFrame_get_Name(This,name)	\
    ( (This)->lpVtbl -> get_Name(This,name) ) 

#define ICoreWebView2ExperimentalFrame_add_NameChanged(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_NameChanged(This,eventHandler,token) ) 

#define ICoreWebView2ExperimentalFrame_remove_NameChanged(This,token)	\
    ( (This)->lpVtbl -> remove_NameChanged(This,token) ) 

#define ICoreWebView2ExperimentalFrame_AddHostObjectToScriptWithOrigins(This,name,object,originsCount,origins)	\
    ( (This)->lpVtbl -> AddHostObjectToScriptWithOrigins(This,name,object,originsCount,origins) ) 

#define ICoreWebView2ExperimentalFrame_RemoveHostObjectFromScript(This,name)	\
    ( (This)->lpVtbl -> RemoveHostObjectFromScript(This,name) ) 

#define ICoreWebView2ExperimentalFrame_add_Destroyed(This,eventHandler,token)	\
    ( (This)->lpVtbl -> add_Destroyed(This,eventHandler,token) ) 

#define ICoreWebView2ExperimentalFrame_remove_Destroyed(This,token)	\
    ( (This)->lpVtbl -> remove_Destroyed(This,token) ) 

#define ICoreWebView2ExperimentalFrame_IsDestroyed(This,destroyed)	\
    ( (This)->lpVtbl -> IsDestroyed(This,destroyed) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalFrame_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameCreatedEventArgs_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalFrameCreatedEventArgs_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalFrameCreatedEventArgs */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalFrameCreatedEventArgs = {0x4d6e7b5e,0x9baa,0x11eb,{0xa8,0xb3,0x02,0x42,0xac,0x13,0x00,0x03}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4d6e7b5e-9baa-11eb-a8b3-0242ac130003")
    ICoreWebView2ExperimentalFrameCreatedEventArgs : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Frame( 
            /* [retval][out] */ ICoreWebView2ExperimentalFrame **frame) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalFrameCreatedEventArgsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalFrameCreatedEventArgs * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalFrameCreatedEventArgs * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalFrameCreatedEventArgs * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Frame )( 
            ICoreWebView2ExperimentalFrameCreatedEventArgs * This,
            /* [retval][out] */ ICoreWebView2ExperimentalFrame **frame);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalFrameCreatedEventArgsVtbl;

    interface ICoreWebView2ExperimentalFrameCreatedEventArgs
    {
        CONST_VTBL struct ICoreWebView2ExperimentalFrameCreatedEventArgsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalFrameCreatedEventArgs_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalFrameCreatedEventArgs_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalFrameCreatedEventArgs_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalFrameCreatedEventArgs_get_Frame(This,frame)	\
    ( (This)->lpVtbl -> get_Frame(This,frame) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalFrameCreatedEventArgs_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameCreatedEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalFrameCreatedEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalFrameCreatedEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalFrameCreatedEventHandler = {0x38059770,0x9baa,0x11eb,{0xa8,0xb3,0x02,0x42,0xac,0x13,0x00,0x03}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("38059770-9baa-11eb-a8b3-0242ac130003")
    ICoreWebView2ExperimentalFrameCreatedEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2 *sender,
            /* [in] */ ICoreWebView2ExperimentalFrameCreatedEventArgs *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalFrameCreatedEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalFrameCreatedEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalFrameCreatedEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalFrameCreatedEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalFrameCreatedEventHandler * This,
            /* [in] */ ICoreWebView2 *sender,
            /* [in] */ ICoreWebView2ExperimentalFrameCreatedEventArgs *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalFrameCreatedEventHandlerVtbl;

    interface ICoreWebView2ExperimentalFrameCreatedEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalFrameCreatedEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalFrameCreatedEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalFrameCreatedEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalFrameCreatedEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalFrameCreatedEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalFrameCreatedEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameDestroyedEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalFrameDestroyedEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalFrameDestroyedEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalFrameDestroyedEventHandler = {0x59dd7b4c,0x9baa,0x11eb,{0xa8,0xb3,0x02,0x42,0xac,0x13,0x00,0x03}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("59dd7b4c-9baa-11eb-a8b3-0242ac130003")
    ICoreWebView2ExperimentalFrameDestroyedEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2ExperimentalFrame *sender,
            /* [in] */ IUnknown *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalFrameDestroyedEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalFrameDestroyedEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalFrameDestroyedEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalFrameDestroyedEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalFrameDestroyedEventHandler * This,
            /* [in] */ ICoreWebView2ExperimentalFrame *sender,
            /* [in] */ IUnknown *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalFrameDestroyedEventHandlerVtbl;

    interface ICoreWebView2ExperimentalFrameDestroyedEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalFrameDestroyedEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalFrameDestroyedEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalFrameDestroyedEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalFrameDestroyedEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalFrameDestroyedEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalFrameDestroyedEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalFrameNameChangedEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalFrameNameChangedEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalFrameNameChangedEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalFrameNameChangedEventHandler = {0x435c7dc8,0x9baa,0x11eb,{0xa8,0xb3,0x02,0x42,0xac,0x13,0x00,0x03}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("435c7dc8-9baa-11eb-a8b3-0242ac130003")
    ICoreWebView2ExperimentalFrameNameChangedEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2ExperimentalFrame *sender,
            /* [in] */ IUnknown *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalFrameNameChangedEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalFrameNameChangedEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalFrameNameChangedEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalFrameNameChangedEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalFrameNameChangedEventHandler * This,
            /* [in] */ ICoreWebView2ExperimentalFrame *sender,
            /* [in] */ IUnknown *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalFrameNameChangedEventHandlerVtbl;

    interface ICoreWebView2ExperimentalFrameNameChangedEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalFrameNameChangedEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalFrameNameChangedEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalFrameNameChangedEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalFrameNameChangedEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalFrameNameChangedEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalFrameNameChangedEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler = {0x51560be0,0x0ad4,0x4157,{0x90,0xcf,0xe6,0xda,0xd6,0x38,0xd8,0xaa}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51560be0-0ad4-4157-90cf-e6dad638d8aa")
    ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2ExperimentalController *sender,
            /* [in] */ IUnknown *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalRasterizationScaleChangedEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler * This,
            /* [in] */ ICoreWebView2ExperimentalController *sender,
            /* [in] */ IUnknown *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalRasterizationScaleChangedEventHandlerVtbl;

    interface ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalRasterizationScaleChangedEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalRasterizationScaleChangedEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalSettings_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalSettings */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalSettings = {0x684cbeef,0x47ba,0x4d4a,{0x99,0xf4,0x97,0x61,0x13,0xf9,0xf1,0x0a}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("684cbeef-47ba-4d4a-99f4-976113f9f10a")
    ICoreWebView2ExperimentalSettings : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_UserAgent( 
            /* [retval][out] */ LPWSTR *userAgent) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_UserAgent( 
            /* [in] */ LPCWSTR userAgent) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalSettingsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalSettings * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalSettings * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalSettings * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_UserAgent )( 
            ICoreWebView2ExperimentalSettings * This,
            /* [retval][out] */ LPWSTR *userAgent);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_UserAgent )( 
            ICoreWebView2ExperimentalSettings * This,
            /* [in] */ LPCWSTR userAgent);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalSettingsVtbl;

    interface ICoreWebView2ExperimentalSettings
    {
        CONST_VTBL struct ICoreWebView2ExperimentalSettingsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalSettings_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalSettings_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalSettings_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalSettings_get_UserAgent(This,userAgent)	\
    ( (This)->lpVtbl -> get_UserAgent(This,userAgent) ) 

#define ICoreWebView2ExperimentalSettings_put_UserAgent(This,userAgent)	\
    ( (This)->lpVtbl -> put_UserAgent(This,userAgent) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalSettings_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings2_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalSettings2_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalSettings2 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalSettings2 = {0x6cc748d4,0x51c5,0x415c,{0xac,0x97,0x77,0xa1,0x5f,0xfe,0xd9,0x39}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6cc748d4-51c5-415c-ac97-77a15ffed939")
    ICoreWebView2ExperimentalSettings2 : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AreBrowserAcceleratorKeysEnabled( 
            /* [retval][out] */ BOOL *areBrowserAcceleratorKeysEnabled) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AreBrowserAcceleratorKeysEnabled( 
            /* [in] */ BOOL areBrowserAcceleratorKeysEnabled) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalSettings2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalSettings2 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalSettings2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalSettings2 * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_AreBrowserAcceleratorKeysEnabled )( 
            ICoreWebView2ExperimentalSettings2 * This,
            /* [retval][out] */ BOOL *areBrowserAcceleratorKeysEnabled);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_AreBrowserAcceleratorKeysEnabled )( 
            ICoreWebView2ExperimentalSettings2 * This,
            /* [in] */ BOOL areBrowserAcceleratorKeysEnabled);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalSettings2Vtbl;

    interface ICoreWebView2ExperimentalSettings2
    {
        CONST_VTBL struct ICoreWebView2ExperimentalSettings2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalSettings2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalSettings2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalSettings2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalSettings2_get_AreBrowserAcceleratorKeysEnabled(This,areBrowserAcceleratorKeysEnabled)	\
    ( (This)->lpVtbl -> get_AreBrowserAcceleratorKeysEnabled(This,areBrowserAcceleratorKeysEnabled) ) 

#define ICoreWebView2ExperimentalSettings2_put_AreBrowserAcceleratorKeysEnabled(This,areBrowserAcceleratorKeysEnabled)	\
    ( (This)->lpVtbl -> put_AreBrowserAcceleratorKeysEnabled(This,areBrowserAcceleratorKeysEnabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalSettings2_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings3_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalSettings3_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalSettings3 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalSettings3 = {0xb3d0d3fa,0x19b4,0x46be,{0x80,0x08,0xb4,0x9e,0xb1,0x53,0x32,0x60}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b3d0d3fa-19b4-46be-8008-b49eb1533260")
    ICoreWebView2ExperimentalSettings3 : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPasswordAutofillEnabled( 
            /* [retval][out] */ BOOL *isPasswordAutofillEnabled) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPasswordAutofillEnabled( 
            /* [in] */ BOOL isPasswordAutofillEnabled) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsGeneralAutofillEnabled( 
            /* [retval][out] */ BOOL *isGeneralAutofillEnabled) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsGeneralAutofillEnabled( 
            /* [in] */ BOOL isGeneralAutofillEnabled) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalSettings3Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalSettings3 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalSettings3 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalSettings3 * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPasswordAutofillEnabled )( 
            ICoreWebView2ExperimentalSettings3 * This,
            /* [retval][out] */ BOOL *isPasswordAutofillEnabled);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPasswordAutofillEnabled )( 
            ICoreWebView2ExperimentalSettings3 * This,
            /* [in] */ BOOL isPasswordAutofillEnabled);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsGeneralAutofillEnabled )( 
            ICoreWebView2ExperimentalSettings3 * This,
            /* [retval][out] */ BOOL *isGeneralAutofillEnabled);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsGeneralAutofillEnabled )( 
            ICoreWebView2ExperimentalSettings3 * This,
            /* [in] */ BOOL isGeneralAutofillEnabled);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalSettings3Vtbl;

    interface ICoreWebView2ExperimentalSettings3
    {
        CONST_VTBL struct ICoreWebView2ExperimentalSettings3Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalSettings3_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalSettings3_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalSettings3_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalSettings3_get_IsPasswordAutofillEnabled(This,isPasswordAutofillEnabled)	\
    ( (This)->lpVtbl -> get_IsPasswordAutofillEnabled(This,isPasswordAutofillEnabled) ) 

#define ICoreWebView2ExperimentalSettings3_put_IsPasswordAutofillEnabled(This,isPasswordAutofillEnabled)	\
    ( (This)->lpVtbl -> put_IsPasswordAutofillEnabled(This,isPasswordAutofillEnabled) ) 

#define ICoreWebView2ExperimentalSettings3_get_IsGeneralAutofillEnabled(This,isGeneralAutofillEnabled)	\
    ( (This)->lpVtbl -> get_IsGeneralAutofillEnabled(This,isGeneralAutofillEnabled) ) 

#define ICoreWebView2ExperimentalSettings3_put_IsGeneralAutofillEnabled(This,isGeneralAutofillEnabled)	\
    ( (This)->lpVtbl -> put_IsGeneralAutofillEnabled(This,isGeneralAutofillEnabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalSettings3_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalSettings4_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalSettings4_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalSettings4 */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalSettings4 = {0x19f47d09,0xd208,0x4feb,{0x9e,0x19,0xa3,0xaf,0xd2,0xfe,0x13,0xca}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("19f47d09-d208-4feb-9e19-a3afd2fe13ca")
    ICoreWebView2ExperimentalSettings4 : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsPinchZoomEnabled( 
            /* [retval][out] */ BOOL *enabled) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_IsPinchZoomEnabled( 
            /* [in] */ BOOL enabled) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalSettings4Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalSettings4 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalSettings4 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalSettings4 * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsPinchZoomEnabled )( 
            ICoreWebView2ExperimentalSettings4 * This,
            /* [retval][out] */ BOOL *enabled);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsPinchZoomEnabled )( 
            ICoreWebView2ExperimentalSettings4 * This,
            /* [in] */ BOOL enabled);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalSettings4Vtbl;

    interface ICoreWebView2ExperimentalSettings4
    {
        CONST_VTBL struct ICoreWebView2ExperimentalSettings4Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalSettings4_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalSettings4_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalSettings4_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalSettings4_get_IsPinchZoomEnabled(This,enabled)	\
    ( (This)->lpVtbl -> get_IsPinchZoomEnabled(This,enabled) ) 

#define ICoreWebView2ExperimentalSettings4_put_IsPinchZoomEnabled(This,enabled)	\
    ( (This)->lpVtbl -> put_IsPinchZoomEnabled(This,enabled) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalSettings4_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalStateChangedEventHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalStateChangedEventHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalStateChangedEventHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalStateChangedEventHandler = {0x81336594,0x7ede,0x4ba9,{0xbf,0x71,0xac,0xf0,0xa9,0x5b,0x58,0xdd}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("81336594-7ede-4ba9-bf71-acf0a95b58dd")
    ICoreWebView2ExperimentalStateChangedEventHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ ICoreWebView2ExperimentalDownloadOperation *sender,
            /* [in] */ IUnknown *args) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalStateChangedEventHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalStateChangedEventHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalStateChangedEventHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalStateChangedEventHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalStateChangedEventHandler * This,
            /* [in] */ ICoreWebView2ExperimentalDownloadOperation *sender,
            /* [in] */ IUnknown *args);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalStateChangedEventHandlerVtbl;

    interface ICoreWebView2ExperimentalStateChangedEventHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalStateChangedEventHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalStateChangedEventHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalStateChangedEventHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalStateChangedEventHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalStateChangedEventHandler_Invoke(This,sender,args)	\
    ( (This)->lpVtbl -> Invoke(This,sender,args) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalStateChangedEventHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler = {0xF1D2D722,0x3721,0x499C,{0x87,0xF5,0x4C,0x40,0x52,0x60,0x69,0x7A}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F1D2D722-3721-499C-87F5-4C405260697A")
    ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ HRESULT errorCode,
            /* [in] */ ICoreWebView2ExperimentalUpdateRuntimeResult *result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalUpdateRuntimeCompletedHandlerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler * This);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler * This,
            /* [in] */ HRESULT errorCode,
            /* [in] */ ICoreWebView2ExperimentalUpdateRuntimeResult *result);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalUpdateRuntimeCompletedHandlerVtbl;

    interface ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler
    {
        CONST_VTBL struct ICoreWebView2ExperimentalUpdateRuntimeCompletedHandlerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_Invoke(This,errorCode,result)	\
    ( (This)->lpVtbl -> Invoke(This,errorCode,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalUpdateRuntimeCompletedHandler_INTERFACE_DEFINED__ */


#ifndef __ICoreWebView2ExperimentalUpdateRuntimeResult_INTERFACE_DEFINED__
#define __ICoreWebView2ExperimentalUpdateRuntimeResult_INTERFACE_DEFINED__

/* interface ICoreWebView2ExperimentalUpdateRuntimeResult */
/* [unique][object][uuid] */ 


EXTERN_C __declspec(selectany) const IID IID_ICoreWebView2ExperimentalUpdateRuntimeResult = {0xDD503E49,0xAB19,0x47C0,{0xB2,0xAD,0x6D,0xDD,0x09,0xCC,0x3E,0x3A}};

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DD503E49-AB19-47C0-B2AD-6DDD09CC3E3A")
    ICoreWebView2ExperimentalUpdateRuntimeResult : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Status( 
            /* [retval][out] */ COREWEBVIEW2_UPDATE_RUNTIME_STATUS *status) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ExtendedError( 
            /* [retval][out] */ HRESULT *error) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ICoreWebView2ExperimentalUpdateRuntimeResultVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICoreWebView2ExperimentalUpdateRuntimeResult * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICoreWebView2ExperimentalUpdateRuntimeResult * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICoreWebView2ExperimentalUpdateRuntimeResult * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Status )( 
            ICoreWebView2ExperimentalUpdateRuntimeResult * This,
            /* [retval][out] */ COREWEBVIEW2_UPDATE_RUNTIME_STATUS *status);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_ExtendedError )( 
            ICoreWebView2ExperimentalUpdateRuntimeResult * This,
            /* [retval][out] */ HRESULT *error);
        
        END_INTERFACE
    } ICoreWebView2ExperimentalUpdateRuntimeResultVtbl;

    interface ICoreWebView2ExperimentalUpdateRuntimeResult
    {
        CONST_VTBL struct ICoreWebView2ExperimentalUpdateRuntimeResultVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICoreWebView2ExperimentalUpdateRuntimeResult_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICoreWebView2ExperimentalUpdateRuntimeResult_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICoreWebView2ExperimentalUpdateRuntimeResult_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICoreWebView2ExperimentalUpdateRuntimeResult_get_Status(This,status)	\
    ( (This)->lpVtbl -> get_Status(This,status) ) 

#define ICoreWebView2ExperimentalUpdateRuntimeResult_get_ExtendedError(This,error)	\
    ( (This)->lpVtbl -> get_ExtendedError(This,error) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICoreWebView2ExperimentalUpdateRuntimeResult_INTERFACE_DEFINED__ */

#endif /* __WebView2Experimental_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


