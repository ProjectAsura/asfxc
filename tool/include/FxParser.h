//-----------------------------------------------------------------------------
// File : FxParser.h
// Desc : Shader Effect File Parser Module.
// Copyright(c) Project Asura. All right reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Tokenizer.h"
#include <vector>
#include <map>


namespace asura {

///////////////////////////////////////////////////////////////////////////////
// SHADER_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum SHADER_TYPE
{
    SHADER_TYPE_VERTEX = 0,
    SHADER_TYPE_DOMAIN,
    SHADER_TYPE_GEOMETRY,
    SHADER_TYPE_HULL,
    SHADER_TYPE_PIXEL,
    SHADER_TYPE_COMPUTE
};

///////////////////////////////////////////////////////////////////////////////
// POLYGON_MODE enum
///////////////////////////////////////////////////////////////////////////////
enum POLYGON_MODE
{
    POLYGON_MODE_WIREFRAME = 0,
    POLYGON_MODE_SOLID,
};

///////////////////////////////////////////////////////////////////////////////
// BLEND_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum BLEND_TYPE
{
    BLEND_TYPE_ZERO,
    BLEND_TYPE_ONE,
    BLEND_TYPE_SRC_COLOR,
    BLEND_TYPE_INV_SRC_COLOR,
    BLEND_TYPE_SRC_ALPHA,
    BLEND_TYPE_INV_SRC_ALPHA,
    BLEND_TYPE_DST_ALPHA,
    BLEND_TYPE_INV_DST_ALPHA,
    BLEND_TYPE_DST_COLOR,
    BLEND_TYPE_INV_DST_COLOR,
};

///////////////////////////////////////////////////////////////////////////////
// FILTER_MODE enum
//////////////////////////////////////////////////////////////////////////////
enum FILTER_MODE
{
    FILTER_MODE_NEAREST,
    FILTER_MODE_LINEAR
};

///////////////////////////////////////////////////////////////////////////////
// MIPMAP_MODE enum
///////////////////////////////////////////////////////////////////////////////
enum MIPMAP_MODE
{
    MIPMAP_MODE_NEAREST,
    MIPMAP_MODE_LINEAR,
    MIPMAP_MODE_NONE,
};

///////////////////////////////////////////////////////////////////////////////
// ADDRESS_MODE enum
///////////////////////////////////////////////////////////////////////////////
enum ADDRESS_MODE
{
    ADDRESS_MODE_CLAMP,
    ADDRESS_MODE_WRAP,
    ADDRESS_MODE_MIRROR,
    ADDRESS_MODE_BORDER,
};

///////////////////////////////////////////////////////////////////////////////
// BORDER_COLOR enum
///////////////////////////////////////////////////////////////////////////////
enum BORDER_COLOR
{
    BORDER_COLOR_TRANSPARENT_BLACK  = 0,    // (0, 0, 0, 0).
    BORDER_COLOR_OPAQUE_BLACK       = 1,    // (0, 0, 0, 1).
    BORDER_COLOR_OPAQUE_WHITE       = 2,    // (1, 1, 1, 1).
};

///////////////////////////////////////////////////////////////////////////////
// CULL_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum CULL_TYPE
{
    CULL_TYPE_NONE,
    CULL_TYPE_FRONT,
    CULL_TYPE_BACK,
};

///////////////////////////////////////////////////////////////////////////////
// COMPARE_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum COMPARE_TYPE
{
    COMPARE_TYPE_NEVER,
    COMPARE_TYPE_LESS,
    COMPARE_TYPE_EQUAL,
    COMPARE_TYPE_LEQUAL,
    COMPARE_TYPE_GREATER,
    COMPARE_TYPE_NEQUAL,
    COMPARE_TYPE_GEQUAL,
    COMPARE_TYPE_ALWAYS,
};

///////////////////////////////////////////////////////////////////////////////
// STENCIL_OP_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum STENCIL_OP_TYPE
{
    STENCIL_OP_KEEP,
    STENCIL_OP_ZERO,
    STENCIL_OP_REPLACE,
    STENCIL_OP_INCR_SAT,
    STENCIL_OP_DECR_SAT,
    STENCIL_OP_INVERT,
    STENCIL_OP_INCR,
    STENCIL_OP_DECR,
};

///////////////////////////////////////////////////////////////////////////////
// DEPTH_WRITE_MASK enum
///////////////////////////////////////////////////////////////////////////////
enum DEPTH_WRITE_MASK
{
    DEPTH_WRITE_MASK_ZERO,
    DEPTH_WRITE_MASK_ALL,
};

///////////////////////////////////////////////////////////////////////////////
// BLEND_OP_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum BLEND_OP_TYPE
{
    BLEND_OP_TYPE_ADD,
    BLEND_OP_TYPE_SUB,
    BLEND_OP_TYPE_REV_SUB,
    BLEND_OP_TYPE_MIN,
    BLEND_OP_TYPE_MAX,
};

///////////////////////////////////////////////////////////////////////////////
// MEMBER_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum MEMBER_TYPE
{
    MEMBER_TYPE_UNKNOWN,
    MEMBER_TYPE_BOOL,
    MEMBER_TYPE_BOOL1x2,
    MEMBER_TYPE_BOOL1x3,
    MEMBER_TYPE_BOOL1x4,
    MEMBER_TYPE_BOOL2,
    MEMBER_TYPE_BOOL2x1,
    MEMBER_TYPE_BOOL2x2,
    MEMBER_TYPE_BOOL2x3,
    MEMBER_TYPE_BOOL2x4,
    MEMBER_TYPE_BOOL3,
    MEMBER_TYPE_BOOL3x1,
    MEMBER_TYPE_BOOL3x2,
    MEMBER_TYPE_BOOL3x3,
    MEMBER_TYPE_BOOL3x4,
    MEMBER_TYPE_BOOL4,
    MEMBER_TYPE_BOOL4x1,
    MEMBER_TYPE_BOOL4x2,
    MEMBER_TYPE_BOOL4x3,
    MEMBER_TYPE_BOOL4x4,
    MEMBER_TYPE_INT,
    MEMBER_TYPE_INT1x2,
    MEMBER_TYPE_INT1x3,
    MEMBER_TYPE_INT1x4,
    MEMBER_TYPE_INT2,
    MEMBER_TYPE_INT2x1,
    MEMBER_TYPE_INT2x2,
    MEMBER_TYPE_INT2x3,
    MEMBER_TYPE_INT2x4,
    MEMBER_TYPE_INT3,
    MEMBER_TYPE_INT3x1,
    MEMBER_TYPE_INT3x2,
    MEMBER_TYPE_INT3x3,
    MEMBER_TYPE_INT3x4,
    MEMBER_TYPE_INT4,
    MEMBER_TYPE_INT4x1,
    MEMBER_TYPE_INT4x2,
    MEMBER_TYPE_INT4x3,
    MEMBER_TYPE_INT4x4,
    MEMBER_TYPE_UINT,
    MEMBER_TYPE_UINT1x2,
    MEMBER_TYPE_UINT1x3,
    MEMBER_TYPE_UINT1x4,
    MEMBER_TYPE_UINT2,
    MEMBER_TYPE_UINT2x1,
    MEMBER_TYPE_UINT2x2,
    MEMBER_TYPE_UINT2x3,
    MEMBER_TYPE_UINT2x4,
    MEMBER_TYPE_UINT3,
    MEMBER_TYPE_UINT3x1,
    MEMBER_TYPE_UINT3x2,
    MEMBER_TYPE_UINT3x3,
    MEMBER_TYPE_UINT3x4,
    MEMBER_TYPE_UINT4,
    MEMBER_TYPE_UINT4x1,
    MEMBER_TYPE_UINT4x2,
    MEMBER_TYPE_UINT4x3,
    MEMBER_TYPE_UINT4x4,
    MEMBER_TYPE_DOUBLE,
    MEMBER_TYPE_DOUBLE1x2,
    MEMBER_TYPE_DOUBLE1x3,
    MEMBER_TYPE_DOUBLE1x4,
    MEMBER_TYPE_DOUBLE2,
    MEMBER_TYPE_DOUBLE2x1,
    MEMBER_TYPE_DOUBLE2x2,
    MEMBER_TYPE_DOUBLE2x3,
    MEMBER_TYPE_DOUBLE2x4,
    MEMBER_TYPE_DOUBLE3,
    MEMBER_TYPE_DOUBLE3x1,
    MEMBER_TYPE_DOUBLE3x2,
    MEMBER_TYPE_DOUBLE3x3,
    MEMBER_TYPE_DOUBLE3x4,
    MEMBER_TYPE_DOUBLE4,
    MEMBER_TYPE_DOUBLE4x1,
    MEMBER_TYPE_DOUBLE4x2,
    MEMBER_TYPE_DOUBLE4x3,
    MEMBER_TYPE_DOUBLE4x4,
    MEMBER_TYPE_FLOAT,
    MEMBER_TYPE_FLOAT1x2,
    MEMBER_TYPE_FLOAT1x3,
    MEMBER_TYPE_FLOAT1x4,
    MEMBER_TYPE_FLOAT2,
    MEMBER_TYPE_FLOAT2x1,
    MEMBER_TYPE_FLOAT2x2,
    MEMBER_TYPE_FLOAT2x3,
    MEMBER_TYPE_FLOAT2x4,
    MEMBER_TYPE_FLOAT3,
    MEMBER_TYPE_FLOAT3x1,
    MEMBER_TYPE_FLOAT3x2,
    MEMBER_TYPE_FLOAT3x3,
    MEMBER_TYPE_FLOAT3x4,
    MEMBER_TYPE_FLOAT4,
    MEMBER_TYPE_FLOAT4x1,
    MEMBER_TYPE_FLOAT4x2,
    MEMBER_TYPE_FLOAT4x3,
    MEMBER_TYPE_FLOAT4x4,
    MEMBER_TYPE_STRUCT,
};

///////////////////////////////////////////////////////////////////////////////
// PROPERTY_TYPE enum
///////////////////////////////////////////////////////////////////////////////
enum PROPERTY_TYPE
{
    PROPERTY_TYPE_BOOL,
    PROPERTY_TYPE_INT,
    PROPERTY_TYPE_FLOAT,
    PROPERTY_TYPE_FLOAT2,
    PROPERTY_TYPE_FLOAT3,
    PROPERTY_TYPE_FLOAT4,
    PROPERTY_TYPE_COLOR3,
    PROPERTY_TYPE_COLOR4,
    PROPERTY_TYPE_TEXTURE1D,
    PROPERTY_TYPE_TEXTURE1D_ARRAY,
    PROPERTY_TYPE_TEXTURE2D,
    PROPERTY_TYPE_TEXTURE2D_ARRAY,
    PROPERTY_TYPE_TEXTURE3D,
    PROPERTY_TYPE_TEXTURECUBE,
    PROPERTY_TYPE_TEXTURECUBE_ARRAY
};

///////////////////////////////////////////////////////////////////////////////
// TYPE_MODIFIER enum
///////////////////////////////////////////////////////////////////////////////
enum TYPE_MODIFIER
{
    TYPE_MODIFIER_NONE          = 0,
    TYPE_MODIFIER_CONST         = 0x1 << 0,
    TYPE_MODIFIER_ROW_MAJOR     = 0x1 << 1,
    TYPE_MODIFIER_COLUMN_MAJOR  = 0x1 << 2,
};

///////////////////////////////////////////////////////////////////////////////
// RESOURCE_TYPE
///////////////////////////////////////////////////////////////////////////////
enum RESOURCE_TYPE
{
    RESOURCE_TYPE_TEXTURE1D,
    RESOURCE_TYPE_TEXTURE1DARRAY,
    RESOURCE_TYPE_TEXTURE2D,
    RESOURCE_TYPE_TEXTURE2DARRAY,
    RESOURCE_TYPE_TEXTURE2DMS,
    RESOURCE_TYPE_TEXTURE2DMSARRAY,
    RESOURCE_TYPE_TEXTURE3D,
    RESOURCE_TYPE_TEXTURECUBE,
    RESOURCE_TYPE_TEXTURECUBEARRAY,
    RESOURCE_TYPE_BUFFER,
    RESOURCE_TYPE_STRUCTURED_BUFFER,
    RESOURCE_TYPE_BYTEADDRESS_BUFFER,
    RESOURCE_TYPE_RWTEXTURE1D,
    RESOURCE_TYPE_RWTEXTURE1DARRAY,
    RESOURCE_TYPE_RWTEXTURE2D,
    RESOURCE_TYPE_RWTEXTURE2DARRAY,
    RESOURCE_TYPE_RWTEXTURE3D,
    RESOURCE_TYPE_RWBUFFER,
    RESOURCE_TYPE_RWSTRUCTURED_BUFFER,
    RESOURCE_TYPE_RWBYTEADDRESS_BUFFER,
    RESOURCE_TYPE_SAMPLER_STATE,
    RESOURCE_TYPE_SAMPLER_COMPRISON_STATE,
};

///////////////////////////////////////////////////////////////////////////////
// Shader 
///////////////////////////////////////////////////////////////////////////////
struct Shader
{
    SHADER_TYPE                 Type;           //!< シェーダタイプです.
    std::string                 EntryPoint;     //!< エントリーポイント名です.
    std::string                 Profile;        //!< シェーダプロファイルです.
    std::vector<std::string>    Arguments;      //!< 引数です.
};

///////////////////////////////////////////////////////////////////////////////
// RasterizerState
///////////////////////////////////////////////////////////////////////////////
struct RasterizerState
{
    POLYGON_MODE  PolygonMode                   = POLYGON_MODE_SOLID;
    CULL_TYPE     CullMode                      = CULL_TYPE_NONE; 
    bool          FrontCCW                      = true;
    uint32_t      DepthBias                     = 0;
    float         DepthBiasClamp                = 0.0f;
    float         SlopeScaledDepthBias          = 0.0f;
    bool          DepthClipEnable               = false;
    bool          EnableConservativeRaster      = false;
};

///////////////////////////////////////////////////////////////////////////////
// DepthStencilState
///////////////////////////////////////////////////////////////////////////////
struct DepthStencilState
{
    bool                DepthEnable                   = true;
    DEPTH_WRITE_MASK    DepthWriteMask                = DEPTH_WRITE_MASK_ALL;
    COMPARE_TYPE        DepthFunc                     = COMPARE_TYPE_LESS;
    bool                StencilEnable                 = false;
    uint8_t             StencilReadMask               = 0xff;
    uint8_t             StencilWriteMask              = 0xff;
    STENCIL_OP_TYPE     FrontFaceStencilFail          = STENCIL_OP_KEEP;
    STENCIL_OP_TYPE     FrontFaceStencilDepthFail     = STENCIL_OP_KEEP;
    STENCIL_OP_TYPE     FrontFaceStencilPass          = STENCIL_OP_KEEP;
    COMPARE_TYPE        FrontFaceStencilFunc          = COMPARE_TYPE_ALWAYS;
    STENCIL_OP_TYPE     BackFaceStencilFail           = STENCIL_OP_KEEP;
    STENCIL_OP_TYPE     BackFaceStencilDepthFail      = STENCIL_OP_KEEP;
    STENCIL_OP_TYPE     BackFaceStencilPass           = STENCIL_OP_KEEP;
    COMPARE_TYPE        BackFaceStencilFunc           = COMPARE_TYPE_ALWAYS;
};

///////////////////////////////////////////////////////////////////////////////
// BlendState
///////////////////////////////////////////////////////////////////////////////
struct BlendState
{
    bool            AlphaToCoverageEnable   = false;
    bool            BlendEnable             = false;
    BLEND_TYPE      SrcBlend                = BLEND_TYPE_ONE;
    BLEND_TYPE      DstBlend                = BLEND_TYPE_ZERO;
    BLEND_OP_TYPE   BlendOp                 = BLEND_OP_TYPE_ADD;
    BLEND_TYPE      SrcBlendAlpha           = BLEND_TYPE_ONE;
    BLEND_TYPE      DstBlendAlpha           = BLEND_TYPE_ZERO;
    BLEND_OP_TYPE   BlendOpAlpha            = BLEND_OP_TYPE_ADD;
    uint8_t         RenderTargetWriteMask   = 0xff;
};

///////////////////////////////////////////////////////////////////////////////
// Member
///////////////////////////////////////////////////////////////////////////////
struct Member
{
    std::string     Name;
    MEMBER_TYPE     Type;
    TYPE_MODIFIER   Modifier;
    uint32_t        PackOffset;
};

///////////////////////////////////////////////////////////////////////////////
// ConstantBuffer
///////////////////////////////////////////////////////////////////////////////
struct ConstantBuffer
{
    std::string             Name;
    uint32_t                Register;
    std::vector<Member>     Members;
};

///////////////////////////////////////////////////////////////////////////////
// Structure
///////////////////////////////////////////////////////////////////////////////
struct Structure
{
    std::string             Name;
    std::vector<Member>     Members;
};

///////////////////////////////////////////////////////////////////////////////
// ValueProperty 
///////////////////////////////////////////////////////////////////////////////
struct ValueProperty
{
    std::string         Name;
    std::string         DisplayTag;
    PROPERTY_TYPE       Type;
    float               Min;
    float               Max;
    float               Step;
    std::string         DefaultValue0;
    std::string         DefaultValue1;
    std::string         DefaultValue2;
    std::string         DefaultValue3;
};

///////////////////////////////////////////////////////////////////////////////
// TextureProperty
///////////////////////////////////////////////////////////////////////////////
struct TextureProperty
{
    std::string         Name;
    std::string         DisplayTag;
    PROPERTY_TYPE       Type;
    bool                SRGB;
    std::string         DefaultValue;
};

///////////////////////////////////////////////////////////////////////////////
// Resource
///////////////////////////////////////////////////////////////////////////////
struct Resource
{
    std::string         Name;
    RESOURCE_TYPE       ResourceType;
    MEMBER_TYPE         DataType;
    uint32_t            Register;
};

///////////////////////////////////////////////////////////////////////////////
// Pass
///////////////////////////////////////////////////////////////////////////////
struct Pass
{
    std::string                 Name;           //!< パス名です.
    std::vector<Shader>         Shaders;        //!< シェーダデータです.
    std::string                 RS;             //!< ラスタライザーステートです.
    std::string                 DSS;            //!< 深度ステンシルステートです.
    std::string                 BS;             //!< ブレンドステートです.
};

///////////////////////////////////////////////////////////////////////////////
// Technique
///////////////////////////////////////////////////////////////////////////////
struct Technique
{
    std::string         Name;   //!< テクニック名です.
    std::vector<Pass>   Pass;   //!< パスデータです.
};

///////////////////////////////////////////////////////////////////////////////
// FxParser class
///////////////////////////////////////////////////////////////////////////////
class FxParser
{
    //========================================================================
    // list of friend classes and methods.
    //========================================================================
    /* NOTHING */

public:
    //========================================================================
    // public variables.
    //========================================================================
    /* NOTHING */

    //========================================================================
    // public methods.
    //========================================================================

    FxParser ();
    ~FxParser();

    void Clear();
    bool Parse(const char* filename);
    bool WriteVariationInfo(const char* xmlpath, const char* hlslpath);
    bool WriteSourceCode(const char* filename);

    const char* GetSourceCode() const;
    size_t GetSourceCodeSize() const;
    const std::map<std::string, BlendState>& GetBlendStates() const;
    const std::map<std::string, RasterizerState>& GetRasterizerStates() const;
    const std::map<std::string, DepthStencilState>& GetDepthStencilStates() const;
    const std::map<std::string, ConstantBuffer>& GetConstantBuffers() const;
    const std::map<std::string, Structure>& GetStructures() const;
    const std::map<std::string, Resource>& GetResources() const;
    const std::vector<Technique>& GetTechniques() const;

private:
    //========================================================================
    // private variables.
    //========================================================================
    char*                                       m_pBuffer;
    Tokenizer                                   m_Tokenizer;
    std::vector<Technique>                      m_Technieues;
    std::map<std::string, Shader>               m_Shaders;
    std::map<std::string, std::string>          m_Defines;
    std::map<std::string, BlendState>           m_BlendStates;
    std::map<std::string, RasterizerState>      m_RasterizerStates;
    std::map<std::string, DepthStencilState>    m_DepthStencilStates;
    std::map<std::string, ConstantBuffer>       m_ConstantBuffers;
    std::map<std::string, Structure>            m_Structures;
    std::map<std::string, Resource>             m_Resources;
    std::map<std::string, ValueProperty>        m_ValueProperties;
    std::map<std::string, TextureProperty>      m_TextureProperties;
    std::vector<std::string>                    m_Includes;
    std::string                                 m_SourceCode;
    int                                         m_ShaderCounter;
    size_t                                      m_BufferSize;
    size_t                                      m_ReadSize;

    //========================================================================
    // private methods.
    //========================================================================
    bool Load(const char* filename);
    void ParseShader();
    void ParsePass(Technique& technique);
    void ParseTechnique();
    void ParseBlendState();
    void ParseRasterizerState();
    void ParseDepthStencilState();
    void ParsePreprocessor();
    void ParseConstantBuffer();
    void ParseConstantBufferMember(MEMBER_TYPE type, ConstantBuffer& buffer, TYPE_MODIFIER& modifier);
    void ParseStruct();
    void ParseProperties();
    void ParseStructMember(MEMBER_TYPE type, Structure& structure, TYPE_MODIFIER& modifier);
    void ParseResource();
    void ParseResourceDetail(RESOURCE_TYPE type);
    void ParseTextureProperty(PROPERTY_TYPE type);
    SHADER_TYPE GetShaderType();
};

} // namespace asura