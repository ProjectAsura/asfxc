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
    bool WriteVariationInfo(const char* filename);
    bool WriteSourceCode(const char* filename);

    const char* GetSourceCode() const;
    size_t GetSourceCodeSize() const;
    const std::map<std::string, BlendState>& GetBlendStates() const;
    const std::map<std::string, RasterizerState>& GetRasterizerStates() const;
    const std::map<std::string, DepthStencilState>& GetDepthStencilStates() const;
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
    std::vector<std::string>                    m_Includes;
    std::string                                 m_SourceCode;
    int                                         m_ShaderCounter;
    size_t                                      m_BufferSize;

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
    SHADER_TYPE GetShaderType();
};

} // namespace asura