#pragma once

#define aPtr(x) std::auto_ptr<x*>

#define USE_DIRECX_11

#if defined (USE_DIRECX_11)

#include <windows.h>

#pragma region Undefine Windows Macros
// Only undefine, if DXGIType.h has not been included yet
#undef DXGI_STATUS_OCCLUDED
#undef DXGI_STATUS_CLIPPED
#undef DXGI_STATUS_NO_REDIRECTION
#undef DXGI_STATUS_NO_DESKTOP_ACCESS
#undef DXGI_STATUS_GRAPHICS_VIDPN_SOURCE_IN_USE
#undef DXGI_STATUS_MODE_CHANGED
#undef DXGI_STATUS_MODE_CHANGE_IN_PROGRESS
#undef DXGI_ERROR_INVALID_CALL
#undef DXGI_ERROR_NOT_FOUND
#undef DXGI_ERROR_MORE_DATA
#undef DXGI_ERROR_UNSUPPORTED
#undef DXGI_ERROR_DEVICE_REMOVED
#undef DXGI_ERROR_DEVICE_HUNG
#undef DXGI_ERROR_DEVICE_RESET
#undef DXGI_ERROR_WAS_STILL_DRAWING
#undef DXGI_ERROR_FRAME_STATISTICS_DISJOINT
#undef DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE
#undef DXGI_ERROR_DRIVER_INTERNAL_ERROR
#undef DXGI_ERROR_NONEXCLUSIVE
#undef DXGI_ERROR_NOT_CURRENTLY_AVAILABLE
#undef DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED
#undef DXGI_ERROR_REMOTE_OUTOFMEMORY
#undef D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#undef D3D11_ERROR_FILE_NOT_FOUND
#undef D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS
#undef D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD
#undef D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS
#undef D3D10_ERROR_FILE_NOT_FOUND

#pragma endregion



#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

typedef DirectX::XMMATRIX mat4;
typedef DirectX::XMVECTOR vec4d;
#define vec4d_normalize(V1) DirectX::XMVector4Normalize(V1)
#define vec4d_add(V1, V2) DirectX::XMVectorAdd(V1,V2) 
#define vec4d_sub(V1, V2) DirectX::XMVectorSubtract(V1,V2) 
#define vec4d_dot(V1, V2) DirectX::XMVector4Dot(V1,V2);
#define vec4d_cross(V1, V2) DirectX::XMVector4Cross(V1,V2);
#define vec4d_scale(V1, SCALE) DirectX::XMVectorScale(V1,SCALE);

typedef ID3D11Texture1D Texture1D;
typedef ID3D11Texture2D Texture2D;
typedef ID3D11Texture3D Texture3D;
typedef Texture2D       NormalMap;

typedef unsigned int    u32;
typedef signed int      i32;
typedef unsigned long   u64;
typedef signed long     s64;
typedef unsigned char   u8;
typedef signed char     s8;
typedef unsigned short  u16;
typedef signed short    s16;
typedef float           f32;
typedef double          f64;

#endif

