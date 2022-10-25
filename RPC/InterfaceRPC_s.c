

/* this ALWAYS GENERATED file contains the RPC server stubs */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 06:14:07 2038
 */
/* Compiler settings for InterfaceRPC.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0626 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>
#include "InterfaceRPC.h"

#define TYPE_FORMAT_STRING_SIZE   27                                
#define PROC_FORMAT_STRING_SIZE   265                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _InterfaceRPC_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } InterfaceRPC_MIDL_TYPE_FORMAT_STRING;

typedef struct _InterfaceRPC_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } InterfaceRPC_MIDL_PROC_FORMAT_STRING;

typedef struct _InterfaceRPC_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } InterfaceRPC_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

#if defined(_CONTROL_FLOW_GUARD_XFG)
#define XFG_TRAMPOLINES(ObjectType)\
static unsigned long ObjectType ## _UserSize_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize(pFlags, Offset, pObject);\
}\
static unsigned char * ObjectType ## _UserMarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal(pFlags, pBuffer, pObject);\
}\
static unsigned char * ObjectType ## _UserUnmarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal(pFlags, pBuffer, pObject);\
}\
static void ObjectType ## _UserFree_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree(pFlags, pObject);\
}
#define XFG_TRAMPOLINES64(ObjectType)\
static unsigned long ObjectType ## _UserSize64_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize64(pFlags, Offset, pObject);\
}\
static unsigned char * ObjectType ## _UserMarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal64(pFlags, pBuffer, pObject);\
}\
static unsigned char * ObjectType ## _UserUnmarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal64(pFlags, pBuffer, pObject);\
}\
static void ObjectType ## _UserFree64_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree64(pFlags, pObject);\
}
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)\
static void* ObjectType ## _bind_XFG(HandleType pObject)\
{\
return ObjectType ## _bind((ObjectType) pObject);\
}\
static void ObjectType ## _unbind_XFG(HandleType pObject, handle_t ServerHandle)\
{\
ObjectType ## _unbind((ObjectType) pObject, ServerHandle);\
}
#define XFG_TRAMPOLINE_FPTR(Function) Function ## _XFG
#else
#define XFG_TRAMPOLINES(ObjectType)
#define XFG_TRAMPOLINES64(ObjectType)
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)
#define XFG_TRAMPOLINE_FPTR(Function) Function
#endif

extern const InterfaceRPC_MIDL_TYPE_FORMAT_STRING InterfaceRPC__MIDL_TypeFormatString;
extern const InterfaceRPC_MIDL_PROC_FORMAT_STRING InterfaceRPC__MIDL_ProcFormatString;
extern const InterfaceRPC_MIDL_EXPR_FORMAT_STRING InterfaceRPC__MIDL_ExprFormatString;

/* Standard interface: InterfaceRPC, ver. 1.0,
   GUID={0x7ac9967a,0x6d7b,0x44c5,{0x97,0x64,0x33,0x59,0x21,0x9a,0x9e,0x4f}} */


extern const MIDL_SERVER_INFO InterfaceRPC_ServerInfo;

extern const RPC_DISPATCH_TABLE InterfaceRPC_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE InterfaceRPC___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x7ac9967a,0x6d7b,0x44c5,{0x97,0x64,0x33,0x59,0x21,0x9a,0x9e,0x4f}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    (RPC_DISPATCH_TABLE*)&InterfaceRPC_v1_0_DispatchTable,
    0,
    0,
    0,
    &InterfaceRPC_ServerInfo,
    0x04000000
    };
RPC_IF_HANDLE InterfaceRPC_v1_0_s_ifspec = (RPC_IF_HANDLE)& InterfaceRPC___RpcServerInterface;

extern const MIDL_STUB_DESC InterfaceRPC_StubDesc;


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const InterfaceRPC_MIDL_PROC_FORMAT_STRING InterfaceRPC__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure login */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x5 ),	/* 5 */
/* 18 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 20 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter username */

/* 28 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 30 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 32 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter password */

/* 34 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 36 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 38 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 40 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 42 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 44 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Procedure logout */

/* 46 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 48 */	NdrFcLong( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x1 ),	/* 1 */
/* 54 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 56 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x0 ),	/* 0 */
/* 64 */	0x40,		/* Oi2 Flags:  has ext, */
			0x0,		/* 0 */
/* 66 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Procedure getFileSize */

/* 74 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 76 */	NdrFcLong( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x2 ),	/* 2 */
/* 82 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 84 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 86 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 88 */	NdrFcShort( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x21 ),	/* 33 */
/* 92 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 94 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 96 */	NdrFcShort( 0x0 ),	/* 0 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter file_name */

/* 102 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 104 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 106 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter file_size */

/* 108 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 110 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 114 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Procedure downloadFile */

/* 120 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 122 */	NdrFcLong( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0x3 ),	/* 3 */
/* 128 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 130 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 132 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 134 */	NdrFcShort( 0x8 ),	/* 8 */
/* 136 */	NdrFcShort( 0x5 ),	/* 5 */
/* 138 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 140 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 142 */	NdrFcShort( 0x1 ),	/* 1 */
/* 144 */	NdrFcShort( 0x0 ),	/* 0 */
/* 146 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter file_name */

/* 148 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 150 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 152 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter file_size */

/* 154 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 156 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 158 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter data */

/* 160 */	NdrFcShort( 0x113 ),	/* Flags:  must size, must free, out, simple ref, */
/* 162 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 164 */	NdrFcShort( 0xe ),	/* Type Offset=14 */

	/* Return value */

/* 166 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 168 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 170 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Procedure uploadFile */

/* 172 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 174 */	NdrFcLong( 0x0 ),	/* 0 */
/* 178 */	NdrFcShort( 0x4 ),	/* 4 */
/* 180 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 182 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 184 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 188 */	NdrFcShort( 0x5 ),	/* 5 */
/* 190 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 192 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0x1 ),	/* 1 */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter file_name */

/* 200 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 202 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 204 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Parameter file_size */

/* 206 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 208 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 210 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter data */

/* 212 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 214 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 216 */	NdrFcShort( 0xe ),	/* Type Offset=14 */

	/* Return value */

/* 218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 220 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 222 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

	/* Procedure deleteFile */

/* 224 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 230 */	NdrFcShort( 0x5 ),	/* 5 */
/* 232 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 234 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 236 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x5 ),	/* 5 */
/* 242 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 244 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter file_name */

/* 252 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 254 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 256 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

	/* Return value */

/* 258 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 260 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 262 */	0x2,		/* FC_CHAR */
			0x0,		/* 0 */

			0x0
        }
    };

static const InterfaceRPC_MIDL_TYPE_FORMAT_STRING InterfaceRPC__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  8 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x11, 0x0,	/* FC_RP */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 16 */	NdrFcShort( 0x1 ),	/* 1 */
/* 18 */	0x29,		/* Corr desc:  parameter, FC_ULONG */
			0x57,		/* FC_ADD_1 */
/* 20 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 22 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 24 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */

			0x0
        }
    };

static const unsigned short InterfaceRPC_FormatStringOffsetTable[] =
    {
    0,
    46,
    74,
    120,
    172,
    224
    };


static const MIDL_STUB_DESC InterfaceRPC_StubDesc = 
    {
    (void *)& InterfaceRPC___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
    0,
    0,
    0,
    0,
    InterfaceRPC__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x8010272, /* MIDL Version 8.1.626 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

static const RPC_DISPATCH_FUNCTION InterfaceRPC_table[] =
    {
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    NdrServerCall2,
    0
    };
static const RPC_DISPATCH_TABLE InterfaceRPC_v1_0_DispatchTable = 
    {
    6,
    (RPC_DISPATCH_FUNCTION*)InterfaceRPC_table
    };

static const SERVER_ROUTINE InterfaceRPC_ServerRoutineTable[] = 
    {
    (SERVER_ROUTINE)login,
    (SERVER_ROUTINE)logout,
    (SERVER_ROUTINE)getFileSize,
    (SERVER_ROUTINE)downloadFile,
    (SERVER_ROUTINE)uploadFile,
    (SERVER_ROUTINE)deleteFile
    };

static const MIDL_SERVER_INFO InterfaceRPC_ServerInfo = 
    {
    &InterfaceRPC_StubDesc,
    InterfaceRPC_ServerRoutineTable,
    InterfaceRPC__MIDL_ProcFormatString.Format,
    InterfaceRPC_FormatStringOffsetTable,
    0,
    0,
    0,
    0};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

