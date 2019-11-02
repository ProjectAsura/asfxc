﻿//-------------------------------------------------------------------------------------------------
// File : FxParser.cpp
// Desc : Shader Effect Parser
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include "FxParser.h"
#include <cstdio>
#include <new>
#include <cassert>


#ifndef DLOG
#if defined(DEBUG) || defined(_DEBUG)
#define DLOG( x, ... ) fprintf_s( stdout, "[File:%s, Line:%d] " x "\n", __FILE__, __LINE__, ##__VA_ARGS__ )
#else
#define DLOG( x, ... )
#endif
#endif//DLOG

#ifndef ELOG
#define ELOG( x, ... ) fprintf_s( stderr, x "\n", ##__VA_ARGS__)
#endif//ELOG


namespace asura {

//-----------------------------------------------------------------------------
//      シェーダタイプ文字列を返却します.
//-----------------------------------------------------------------------------
const char* ShaderTypeString(SHADER_TYPE value)
{
    switch(value)
    {
        case SHADER_TYPE_VERTEX:
            return "vertex";

        case SHADER_TYPE_GEOMETRY:
            return "geometry";

        case SHADER_TYPE_DOMAIN:
            return "domain";

        case SHADER_TYPE_HULL:
            return "hull";

        case SHADER_TYPE_PIXEL:
            return "pixel";

        case SHADER_TYPE_COMPUTE:
            return "compute";
    }

    return nullptr;
}

const char* ToString(POLYGON_MODE mode)
{
    switch(mode)
    {
    default:
    case POLYGON_MODE_SOLID:
        return "solid";

    case POLYGON_MODE_WIREFRAME:
        return "wireframe";
    }
}

const char* ToString(CULL_TYPE type)
{
    switch(type)
    {
    default:
    case CULL_TYPE_NONE:
        return "none";

    case CULL_TYPE_FRONT:
        return "front";

    case CULL_TYPE_BACK:
        return "back";
    }
}

const char* ToString(BLEND_TYPE type)
{
    switch(type)
    {
    default:
    case BLEND_TYPE_ZERO:
        return "zero";

    case BLEND_TYPE_ONE:
        return "one";

    case BLEND_TYPE_SRC_COLOR:
        return "src_color";

    case BLEND_TYPE_INV_SRC_COLOR:
        return "inv_src_color";

    case BLEND_TYPE_SRC_ALPHA:
        return "src_alpha";

    case BLEND_TYPE_INV_SRC_ALPHA:
        return "inv_src_alpha";

    case BLEND_TYPE_DST_ALPHA:
        return "dst_alpha";

    case BLEND_TYPE_INV_DST_ALPHA:
        return "inv_dst_alpha";

    case BLEND_TYPE_DST_COLOR:
        return "dst_color";

    case BLEND_TYPE_INV_DST_COLOR:
        return "inv_dst_color";
    }
}

const char* ToString(FILTER_MODE type)
{
    switch(type)
    {
    default:
    case FILTER_MODE_NEAREST:
        return "nearest";

    case FILTER_MODE_LINEAR:
        return "linear";
    }
}

const char* ToString(MIPMAP_MODE type)
{
    switch(type)
    {
    default:
    case MIPMAP_MODE_NEAREST:
        return "nearest";

    case MIPMAP_MODE_LINEAR:
        return "linear";

    case MIPMAP_MODE_NONE:
        return "none";
    }
}

const char* ToString(ADDRESS_MODE type)
{
    switch(type)
    {
    default:
    case ADDRESS_MODE_WRAP:
        return "wrap";

    case ADDRESS_MODE_CLAMP:
        return "clamp";

    case ADDRESS_MODE_MIRROR:
        return "mirror";

    case ADDRESS_MODE_BORDER:
        return "border";
    }
}

const char* ToString(BORDER_COLOR type)
{
    switch(type)
    {
    default:
    case BORDER_COLOR_TRANSPARENT_BLACK:
        return "transparent_black";

    case BORDER_COLOR_OPAQUE_BLACK:
        return "opaque_black";

    case BORDER_COLOR_OPAQUE_WHITE:
        return "opaque_white";
    }
}

const char* ToString(COMPARE_TYPE type)
{
    switch(type)
    {
    default:
    case COMPARE_TYPE_NEVER:
        return "never";

    case COMPARE_TYPE_LESS:
        return "less";

    case COMPARE_TYPE_EQUAL:
        return "equal";

    case COMPARE_TYPE_LEQUAL:
        return "less_equal";

    case COMPARE_TYPE_GREATER:
        return "greater";

    case COMPARE_TYPE_NEQUAL:
        return "not_equal";

    case COMPARE_TYPE_GEQUAL:
        return "greater_equal";

    case COMPARE_TYPE_ALWAYS:
        return "always";
    }
}

const char* ToString(STENCIL_OP_TYPE type)
{
    switch(type)
    {
    default:
    case STENCIL_OP_KEEP:
        return "keep";

    case STENCIL_OP_ZERO:
        return "zero";

    case STENCIL_OP_REPLACE:
        return "replace";

    case STENCIL_OP_INCR_SAT:
        return "incr_sat";

    case STENCIL_OP_DECR_SAT:
        return "decr_sat";

    case STENCIL_OP_INVERT:
        return "invert";

    case STENCIL_OP_INCR:
        return "incr";

    case STENCIL_OP_DECR:
        return "decr";
    }
}

const char* ToString(DEPTH_WRITE_MASK type)
{
    switch(type)
    {
    default:
    case DEPTH_WRITE_MASK_ZERO:
        return "zero";

    case DEPTH_WRITE_MASK_ALL:
        return "all";
    }
}

const char* ToString(BLEND_OP_TYPE type)
{
    switch(type)
    {
    default:
    case BLEND_OP_TYPE_ADD:
        return "add";

    case BLEND_OP_TYPE_SUB:
        return "sub";

    case BLEND_OP_TYPE_REV_SUB:
        return "rev_sub";

    case BLEND_OP_TYPE_MIN:
        return "min";

    case BLEND_OP_TYPE_MAX:
        return "max";
    }
}

//-----------------------------------------------------------------------------
//      POLYGON_MODE型を解析します.
//-----------------------------------------------------------------------------
POLYGON_MODE ParsePolygonMode(const char* value)
{
    POLYGON_MODE result = POLYGON_MODE_SOLID;

    if (_stricmp(value, "WIREFRAME") == 0)
    { result = POLYGON_MODE_WIREFRAME; }
    else if (_stricmp(value, "SOLID") == 0)
    { result = POLYGON_MODE_SOLID; }

    return result;
}

//-----------------------------------------------------------------------------
//      BLEND_TYPE型を解析します.
//-----------------------------------------------------------------------------
BLEND_TYPE ParseBlendType(const char* value)
{
    BLEND_TYPE type = BLEND_TYPE_ZERO;

    if (_stricmp(value, "ZERO") == 0)
    { type = BLEND_TYPE_ZERO; }
    else if (_stricmp(value, "ONE") == 0)
    { type = BLEND_TYPE_ONE; }
    else if (_stricmp(value, "SRC_COLOR") == 0)
    { type = BLEND_TYPE_SRC_COLOR; }
    else if (_stricmp(value, "INV_SRC_COLOR") == 0)
    { type = BLEND_TYPE_INV_SRC_COLOR; }
    else if (_stricmp(value, "SRC_ALPHA") == 0)
    { type = BLEND_TYPE_SRC_ALPHA; }
    else if (_stricmp(value, "INV_SRC_ALPHA") == 0)
    { type = BLEND_TYPE_INV_SRC_ALPHA; }
    else if (_stricmp(value, "DST_ALPHA") == 0)
    { type = BLEND_TYPE_DST_ALPHA; }
    else if (_stricmp(value, "INV_DST_ALPHA") == 0)
    { type = BLEND_TYPE_INV_DST_ALPHA; }
    else if (_stricmp(value, "DST_COLOR") == 0)
    { type = BLEND_TYPE_DST_COLOR; }
    else if (_stricmp(value, "INV_DST_COLOR") == 0)
    { type = BLEND_TYPE_INV_DST_COLOR; }

    return type;
}

//-----------------------------------------------------------------------------
//      FILTER_MODE型を解析します.
//-----------------------------------------------------------------------------
FILTER_MODE ParseFilterMode(const char* value)
{
    FILTER_MODE result = FILTER_MODE_NEAREST;

    if (_stricmp(value, "NEAREST") == 0)
    { result = FILTER_MODE_NEAREST; }
    else if (_stricmp(value, "LINEAR") == 0)
    { result = FILTER_MODE_LINEAR; }

    return result;
}

//-----------------------------------------------------------------------------
//      MIPMAP_MODE型を解析します.
//-----------------------------------------------------------------------------
MIPMAP_MODE ParseMipmapMode(const char* value)
{
    MIPMAP_MODE result = MIPMAP_MODE_NEAREST;

    if (_stricmp(value, "NEAREST") == 0)
    { result = MIPMAP_MODE_NEAREST; }
    else if (_stricmp(value, "LINEAR") == 0)
    { result = MIPMAP_MODE_LINEAR; }
    else if (_stricmp(value, "NONE") == 0)
    { result = MIPMAP_MODE_NONE; }

    return result;
}

//-----------------------------------------------------------------------------
//      ADDRESS_MODE型を解析します.
//-----------------------------------------------------------------------------
ADDRESS_MODE ParseAddressMode(const char* value)
{
    ADDRESS_MODE result = ADDRESS_MODE_CLAMP;

    if (_stricmp(value, "CLAMP") == 0)
    { result = ADDRESS_MODE_CLAMP; }
    else if (_stricmp(value, "WRAP") == 0)
    { result = ADDRESS_MODE_WRAP; }
    else if (_stricmp(value, "MIRROR") == 0)
    { result = ADDRESS_MODE_MIRROR; }
    else if (_stricmp(value, "BORDER") == 0)
    { result = ADDRESS_MODE_BORDER; }

    return result;
}

//-----------------------------------------------------------------------------
//      BORDER_COLOR型を解析します.
//-----------------------------------------------------------------------------
BORDER_COLOR ParseBorderColor(const char* value)
{
    BORDER_COLOR result = BORDER_COLOR_TRANSPARENT_BLACK;

    if (_stricmp(value, "TRANSPARENT_BLACK") == 0)
    { result = BORDER_COLOR_TRANSPARENT_BLACK; }
    else if (_stricmp(value, "OPAQUE_BLACK") == 0)
    { result = BORDER_COLOR_OPAQUE_BLACK; }
    else if (_stricmp(value, "OAPQUE_WHITE") == 0)
    { result = BORDER_COLOR_OPAQUE_WHITE; }

    return result;
}

//-----------------------------------------------------------------------------
//      CULL_TYPE型を解析します.
//-----------------------------------------------------------------------------
CULL_TYPE ParseCullType(const char* value)
{
    CULL_TYPE result = CULL_TYPE_NONE;

    if (_stricmp(value, "NONE") == 0)
    { result = CULL_TYPE_NONE; }
    else if (_stricmp(value, "FRONT") == 0)
    { result = CULL_TYPE_FRONT; }
    else if (_stricmp(value, "BACK") == 0)
    { result = CULL_TYPE_BACK; }

    return result;
}

//-----------------------------------------------------------------------------
//      COMPARE_TYPE型を解析します.
//-----------------------------------------------------------------------------
COMPARE_TYPE ParseCompareType(const char* value)
{
    COMPARE_TYPE result = COMPARE_TYPE_NEVER;

    if (_stricmp(value, "NEVER") == 0)
    { result = COMPARE_TYPE_NEVER; }
    else if (_stricmp(value, "LESS") == 0)
    { result = COMPARE_TYPE_LESS; }
    else if (_stricmp(value, "EQUAL") == 0)
    { result = COMPARE_TYPE_EQUAL; }
    else if (_stricmp(value, "LEQUAL") == 0)
    { result = COMPARE_TYPE_LEQUAL; }
    else if (_stricmp(value, "GREATER") == 0)
    { result = COMPARE_TYPE_GREATER; }
    else if (_stricmp(value, "NEQUAL") == 0)
    { result = COMPARE_TYPE_NEQUAL; }
    else if (_stricmp(value, "GEQUAL") == 0)
    { result = COMPARE_TYPE_GREATER; }

    return result;
}

//-----------------------------------------------------------------------------
//      STENCIL_OP_TYPE型を解析します.
//-----------------------------------------------------------------------------
STENCIL_OP_TYPE ParseStencilOpType(const char* value)
{
    STENCIL_OP_TYPE result = STENCIL_OP_KEEP;

    if (_stricmp(value, "KEEP") == 0)
    { result = STENCIL_OP_KEEP; }
    else if (_stricmp(value, "ZERO") == 0)
    { result = STENCIL_OP_ZERO; }
    else if (_stricmp(value, "REPLACE") == 0)
    { result = STENCIL_OP_REPLACE; }
    else if (_stricmp(value, "INCR_SAT") == 0)
    { result = STENCIL_OP_INCR_SAT; }
    else if (_stricmp(value, "DECR_SAT") == 0)
    { result = STENCIL_OP_DECR_SAT; }
    else if (_stricmp(value, "INVERT") == 0)
    { result = STENCIL_OP_INVERT; }
    else if (_stricmp(value, "INCR") == 0)
    { result = STENCIL_OP_INCR; }
    else if (_stricmp(value, "DECR") == 0)
    { result = STENCIL_OP_DECR; }

    return result;
}

//-----------------------------------------------------------------------------
//      DEPTH_WRITE_MASK型を解析します.
//-----------------------------------------------------------------------------
DEPTH_WRITE_MASK ParseDepthWriteMask(const char* value)
{
    DEPTH_WRITE_MASK result = DEPTH_WRITE_MASK_ALL;

    if (_stricmp(value, "ALL") == 0)
    { result = DEPTH_WRITE_MASK_ALL; }
    else if (_stricmp(value, "ZERO") == 0)
    { result = DEPTH_WRITE_MASK_ZERO; }

    return result;
}

//-----------------------------------------------------------------------------
//      BLEND_OP_TYPE型を解析します.
//-----------------------------------------------------------------------------
BLEND_OP_TYPE ParseBlendOpType(const char* value)
{
    BLEND_OP_TYPE result = BLEND_OP_TYPE_ADD;

    if (_stricmp(value, "ADD") == 0)
    { result = BLEND_OP_TYPE_ADD; }
    else if (_stricmp(value, "SUB") == 0)
    { result = BLEND_OP_TYPE_SUB; }
    else if (_stricmp(value, "REV_SUB") == 0)
    { result = BLEND_OP_TYPE_REV_SUB; }
    else if (_stricmp(value, "MIN") == 0)
    { result = BLEND_OP_TYPE_MIN; }
    else if (_stricmp(value, "MAX") == 0)
    { result = BLEND_OP_TYPE_MAX; }

    return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// FxParser class
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//      コンストラクタです.
//-------------------------------------------------------------------------------------------------
FxParser::FxParser()
: m_pBuffer      (nullptr)
, m_Tokenizer    ()
, m_Technieues   ()
, m_ShaderCounter(0)
, m_BufferSize   (0)
{ /* DO_NOTHING */ }

//-------------------------------------------------------------------------------------------------
//      デストラクタです.
//-------------------------------------------------------------------------------------------------
FxParser::~FxParser()
{ Clear(); }

//-------------------------------------------------------------------------------------------------
//      クリアします.
//-------------------------------------------------------------------------------------------------
void FxParser::Clear()
{
    if (m_pBuffer != nullptr)
    {
        delete[] m_pBuffer;
        m_pBuffer = nullptr;
    }

    m_Tokenizer.Term();
    m_Technieues.clear();
    m_Technieues.shrink_to_fit();
    m_ShaderCounter = 0;
    m_BufferSize    = 0;
}

//-------------------------------------------------------------------------------------------------
//      解析します.
//-------------------------------------------------------------------------------------------------
bool FxParser::Parse(const char* filename)
{
    if (!Load(filename))
    {
        ELOG( "Error : File Load Failed. filename = %s", filename );
        return false;
    }

    if (!m_Tokenizer.Init(2048))
    {
        ELOG( "Error : Tokenizer Initialize failed." );
        return false;
    }

    m_SourceCode.reserve( m_ReadSize );

    m_Tokenizer.SetSeparator( " \t\r\n,\"" );
    m_Tokenizer.SetCutOff( "{}()=#<>;" );
    m_Tokenizer.SetBuffer( m_pBuffer, m_ReadSize );

    auto cur = m_Tokenizer.GetBuffer();

    while(!m_Tokenizer.IsEnd())
    {
        auto tag = m_Tokenizer.GetAsChar();

        bool output = true;

        if (strlen(tag) >= 2 )
        {
            // Cスタイルによるコメント行は処理しない.
            if (tag[0] == '/' && tag[1] == '*')
            {
                output = false;
                m_Tokenizer.SkipTo("*/");
            }

            if (tag[0] == '/' && tag[1] == '/' )
            {
                output = false;
                m_Tokenizer.SkipLine();
            }
        }

        // プリプロセッサ系.
        if (m_Tokenizer.Compare("#"))
        {
            ParsePreprocessor();
        }
        // テクニック.
        else if (m_Tokenizer.CompareAsLower("technique"))
        {
            output = false;
            ParseTechnique();
        }
        // 定数バッファ.
        else if (m_Tokenizer.CompareAsLower("cbuffer"))
        {
            ParseConstantBuffer();
        }
        // 構造体.
        else if (m_Tokenizer.CompareAsLower("struct"))
        {
            ParseStruct();
        }
        // プロパティ.
        else if (m_Tokenizer.CompareAsLower("properties"))
        {
            output = false;
            ParseProperties();
        }
        else if (
           m_Tokenizer.CompareAsLower("Texture1D")
        || m_Tokenizer.CompareAsLower("Texture1DArray")
        || m_Tokenizer.CompareAsLower("Texture2D")
        || m_Tokenizer.CompareAsLower("Texture2DArray")
        || m_Tokenizer.CompareAsLower("Texture2DMS")
        || m_Tokenizer.CompareAsLower("Texture2DMSArray")
        || m_Tokenizer.CompareAsLower("Texture3D")
        || m_Tokenizer.CompareAsLower("TextureCube")
        || m_Tokenizer.CompareAsLower("TextureCubeArray")
        || m_Tokenizer.CompareAsLower("Buffer")
        || m_Tokenizer.CompareAsLower("ByteAddressBuffer")
        || m_Tokenizer.CompareAsLower("StructuredBuffer")
        || m_Tokenizer.CompareAsLower("RWTexture1D")
        || m_Tokenizer.CompareAsLower("RWTexture1DArray")
        || m_Tokenizer.CompareAsLower("RWTexture2D")
        || m_Tokenizer.CompareAsLower("RWTexture2DArray")
        || m_Tokenizer.CompareAsLower("RWTexture3D")
        || m_Tokenizer.CompareAsLower("RWBuffer")
        || m_Tokenizer.CompareAsLower("RWByteAddressBuffer")
        || m_Tokenizer.CompareAsLower("RWStructuredBuffer")
        || m_Tokenizer.CompareAsLower("SamplerState")
        || m_Tokenizer.CompareAsLower("SamplerComparisonState"))
        {
            ParseResource();
        }
        // シェーダ.
        else if (
            m_Tokenizer.CompareAsLower("vertexshader") 
         || m_Tokenizer.CompareAsLower("pixelshader")
         || m_Tokenizer.CompareAsLower("geometryshader")
         || m_Tokenizer.CompareAsLower("domainshader")
         || m_Tokenizer.CompareAsLower("hullshader") 
         || m_Tokenizer.CompareAsLower("computeshader"))
        {
            output = false;
            ParseShader();
        }
        else if (m_Tokenizer.CompareAsLower("BlendState"))
        {
            output = false;
            ParseBlendState();
        }
        else if (m_Tokenizer.CompareAsLower("RasterizerState"))
        {
            output = false;
            ParseRasterizerState();
        }
        else if ((m_Tokenizer.CompareAsLower("DepthStencilState")))
        {
            output = false;
            ParseDepthStencilState();
        }

        auto ptr = m_Tokenizer.GetPtr();

        // ソースコード文字列に追加.
        if (output && !m_Tokenizer.IsEnd())
        {
            auto size = (ptr - cur);

        #if 0 // デバッグのための視認用.
            for(auto i=0; i<size; ++i)
            { putchar(cur[i]); }
        #endif

            m_SourceCode.append(cur, size);
        }

        cur = ptr;

        // 次のトークンを取得.
        m_Tokenizer.Next();
    }

    // 不要なメモリを解放.
    if (m_pBuffer != nullptr)
    {
        delete[] m_pBuffer;
        m_pBuffer = nullptr;
    }

    m_BufferSize = 0;

    // 一時データを削除.
    m_Shaders.clear();

    // 破棄処理.
    m_Tokenizer.Term();

    return true;
}

//-------------------------------------------------------------------------------------------------
//      ファイルを読み込みます.
//-------------------------------------------------------------------------------------------------
bool FxParser::Load(const char* filename)
{
    FILE* pFile = nullptr;

    // ファイルを開く.
    auto ret = fopen_s( &pFile, filename, "r" );
    if ( ret != 0 )
    {
        ELOG( "Error : File Open Failed." );
        return false;
    }

    // ファイルサイズを算出.
    // ※このファイルサイズ計算は推奨されない方式なので注意!!
    // https://www.jpcert.or.jp/sc-rules/c-fio19-c.html
    auto curpos = ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    auto endpos = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    m_BufferSize = static_cast<size_t>(endpos - curpos);

    // メモリを確保.
    m_pBuffer = new(std::nothrow) char[m_BufferSize + 1]; // null終端させるために +1 している.
    if (m_pBuffer == nullptr)
    {
        ELOG( "Error : Out of memory." );
        fclose(pFile);
        return false;
    }

    // null終端になるようゼロクリア.
    memset(m_pBuffer, 0, m_BufferSize + 1);

    // 一括読み込み.
    m_ReadSize = fread(m_pBuffer, 1, m_BufferSize, pFile);

    if (m_BufferSize != m_ReadSize)
    {
        auto size = m_BufferSize - m_ReadSize;
        assert(size >= 0);

        // ゴミが入るので，クリアする.
        memset(m_pBuffer + m_ReadSize, 0, size);
    }

    // ファイルを閉じる.
    fclose(pFile);

    // 正常終了.
    return true;
}

//-------------------------------------------------------------------------------------------------
//      シェーダを解析します.
//-------------------------------------------------------------------------------------------------
void FxParser::ParseShader()
{
    // シェーダタイプを取得.
    SHADER_TYPE type = GetShaderType();
    m_Tokenizer.Next();

    std::string variable;
    std::string entryPoint;
    std::string profile;

    // テクニック内からの呼び出し.
    if (m_Tokenizer.Compare("="))
    {
        // 適当な変数名を付ける.
        char name[256];
        sprintf_s(name, "Shader_%d", m_ShaderCounter);
        variable = name;

        m_Tokenizer.Next();
    }
    // テクニック外からの呼び出し.
    else
    {
        // 変数名取得.
        variable = std::string(m_Tokenizer.GetAsChar());
        m_Tokenizer.Next();
        assert(m_Tokenizer.Compare("="));
        m_Tokenizer.Next();
    }

    m_ShaderCounter++;
    assert(m_Tokenizer.Compare("compile"));

    // プロファイル名を取得.
    profile    = std::string(m_Tokenizer.NextAsChar());
   
    // エントリーポイント名を取得.
    entryPoint = std::string(m_Tokenizer.NextAsChar());

    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("("));

    // シェーダデータを設定.
    Shader data = {};
    data.EntryPoint = entryPoint;
    data.Profile    = profile;

    m_Tokenizer.Next();
    while(!m_Tokenizer.IsEnd())
    {
        if (m_Tokenizer.Compare(")"))
        { break; }
       
        // メソッド引数を追加.
        data.Arguments.push_back( std::string(m_Tokenizer.GetAsChar()) );

        m_Tokenizer.Next();
    }

    // シェーダを登録.
    if (m_Shaders.find(variable) == m_Shaders.end() )
    { m_Shaders[variable] = data; }
}

//-------------------------------------------------------------------------------------------------
//      テクニックを解析します.
//-------------------------------------------------------------------------------------------------
void FxParser::ParseTechnique()
{
    m_Tokenizer.Next();

    // テクニック名を取得.
    auto name = std::string(m_Tokenizer.GetAsChar());

    // テクニックブロック開始.
    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("{"));

    // テクニック名を設定.
    Technique technique = {};
    technique.Name = name;

    while(!m_Tokenizer.IsEnd())
    {
        // テクニックブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            break;
        }
        else if (m_Tokenizer.Compare("pass"))
        {
            // パスを解析.
            ParsePass(technique);
        }

        // 次のトークンを取得.
        m_Tokenizer.Next();
    }

    // テクニックを登録.
    m_Technieues.push_back(technique);
}

//------------------------------------------------------------------------------------------------
//      パスを解析します.
//------------------------------------------------------------------------------------------------
void FxParser::ParsePass(Technique& technique)
{
    m_Tokenizer.Next();

    // パス名を取得.
    auto name = std::string(m_Tokenizer.GetAsChar());

    // パスブロック開始.
    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("{"));
    int blockCount = 1;

    m_Tokenizer.Next();

    // パス名を設定.
    Pass pass = {};
    pass.Name = name;

    while(!m_Tokenizer.IsEnd())
    {
        // パスブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            blockCount--;
            if (blockCount == 0)
            { break; }
        }
        else if (m_Tokenizer.Compare("{"))
        {
            blockCount++;
        }
        else if (m_Tokenizer.CompareAsLower("RasterizerState"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            m_Tokenizer.Next();

            // 変数名を取得.
            name = std::string(m_Tokenizer.GetAsChar());

            // 変数名からステートを引っ張ってくる.
            auto itr = m_RasterizerStates.find(name);

            if (itr != m_RasterizerStates.end())
            {
                // 発見できたら，パスにステートを登録.
                pass.RS = itr->first;
            }
        }
        else if (m_Tokenizer.CompareAsLower("DepthStencilState"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            m_Tokenizer.Next();

            // 変数名を取得.
            name = std::string(m_Tokenizer.GetAsChar());

            // 変数名からステートを引っ張ってくる.
            auto itr = m_DepthStencilStates.find(name);

            if (itr != m_DepthStencilStates.end())
            {
                // 発見できたら，パスにステートを登録.
                pass.DSS = itr->first;
            }
        }
        else if (m_Tokenizer.CompareAsLower("BlendState"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            m_Tokenizer.Next();

            // 変数名を取得.
            name = std::string(m_Tokenizer.GetAsChar());

            // 変数名からステートを引っ張ってくる.
            auto itr = m_BlendStates.find(name);

            if (itr != m_BlendStates.end())
            {
                // 発見できたら，パスにステートを登録.
                pass.BS = itr->first;
            }
        }
        // シェーダデータ.
        else if (
            m_Tokenizer.CompareAsLower("VertexShader")
         || m_Tokenizer.CompareAsLower("PixelShader")
         || m_Tokenizer.CompareAsLower("GeometryShader")
         || m_Tokenizer.CompareAsLower("DomainShader")
         || m_Tokenizer.CompareAsLower("HullShader")
         || m_Tokenizer.CompareAsLower("ComputeShader"))
        {
            // シェーダタイプを取得.
            Shader shader = {};
            shader.Type = GetShaderType();

            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            m_Tokenizer.Next();

            // 変数で設定されていない場合.
            if (m_Tokenizer.Compare("compile"))
            {
                // シェーダプロファイル名を取得.
                shader.Profile      = std::string(m_Tokenizer.NextAsChar());

                // エントリーポイント名を取得.
                shader.EntryPoint   = std::string(m_Tokenizer.NextAsChar());

                // メソッド引数開始.
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare("("));

                int blankCounter = 1;
                m_Tokenizer.Next();
                while(!m_Tokenizer.IsEnd())
                {
                    // メソッド引数終了.
                    if (m_Tokenizer.Compare(")"))
                    {
                        blankCounter--;
                        if (blankCounter == 0)
                        { break; }
                    }
                    else if (m_Tokenizer.Compare("("))
                    {
                        blankCounter++;
                        m_Tokenizer.Next();
                        continue;
                    }

                    // 引数を追加.
                    shader.Arguments.push_back(m_Tokenizer.GetAsChar());

                    // 次のトークンを取得.
                    m_Tokenizer.Next();
                }

                // パスにシェーダを登録.
                pass.Shaders.push_back(shader);
            }
            // 変数で設定されている場合.
            else
            {
                if (m_Tokenizer.Compare("("))
                { m_Tokenizer.Next(); }

                // 変数名を取得.
                name = std::string(m_Tokenizer.GetAsChar());

                // 変数名からシェーダを引っ張ってくる.
                auto itr = m_Shaders.find(name);

                if (itr != m_Shaders.end())
                {
                    // 発見できたら，パスにシェーダを登録.
                    pass.Shaders.push_back( itr->second );
                }
            }
        }

        // 次のトークンを取得.
        m_Tokenizer.Next();
    }

    // テクニックにパスを登録.
    technique.Pass.push_back(pass);
}

//-------------------------------------------------------------------------------------------------
//      プリプロセッサを解析します.
//-------------------------------------------------------------------------------------------------
void FxParser::ParsePreprocessor()
{
    // 次のトークンを取得.
    m_Tokenizer.Next();

    if (m_Tokenizer.Compare("define"))
    {
        auto tag = std::string(m_Tokenizer.NextAsChar());
        auto val = std::string(m_Tokenizer.NextAsChar());
        m_Defines[tag] = val;
    }
    else if (m_Tokenizer.Compare("elif"))
    {
        //auto val = std::string(m_Tokenizer.NextAsChar());
    }
    else if (m_Tokenizer.Compare("else"))
    {
    }
    else if (m_Tokenizer.Compare("endif"))
    {
    }
    else if (m_Tokenizer.Compare("error"))
    {
    }
    else if (m_Tokenizer.Compare("if"))
    {
        //auto val = std::string(m_Tokenizer.NextAsChar());
    }
    else if (m_Tokenizer.Compare("ifdef"))
    {
        //auto val = std::string(m_Tokenizer.NextAsChar());
    }
    else if (m_Tokenizer.Compare("ifndef"))
    {
        //auto val = std::string(m_Tokenizer.NextAsChar());
    }
    else if (m_Tokenizer.Compare("include"))
    {
        m_Tokenizer.Next();
        assert(m_Tokenizer.Compare("<"));
        m_Includes.push_back(m_Tokenizer.NextAsChar());
        m_Tokenizer.Next();
        assert(m_Tokenizer.Compare(">"));
    }
    else if (m_Tokenizer.Compare("line"))
    {
    }
    else if (m_Tokenizer.Compare("pragma"))
    {
    }
    else if (m_Tokenizer.Compare("undef"))
    {
        auto tag = std::string(m_Tokenizer.NextAsChar());
        m_Defines.erase(tag);
    }
}

//-----------------------------------------------------------------------------
//      ブレンドステートを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseBlendState()
{
    m_Tokenizer.Next();

    // ステート名を取得.
    auto name = std::string(m_Tokenizer.GetAsChar());

    // ステートブロック開始.
    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("{"));
    int blockCount = 1;

    BlendState state;

    m_Tokenizer.Next();
    while(!m_Tokenizer.IsEnd())
    {
        // パスブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            blockCount--;
            if (blockCount == 0)
            { break; }
        }
        else if (m_Tokenizer.Compare("{"))
        {
            blockCount++;
        }
        else if (m_Tokenizer.CompareAsLower("AlphaToCoverageEnable"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.AlphaToCoverageEnable = m_Tokenizer.NextAsBool();
        }
        else if (m_Tokenizer.CompareAsLower("BlendEnable"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.BlendEnable = m_Tokenizer.NextAsBool();
        }
        else if (m_Tokenizer.CompareAsLower("SrcBlend"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            auto value = m_Tokenizer.NextAsChar();
            state.SrcBlend = ParseBlendType(value);
        }
        else if (m_Tokenizer.CompareAsLower("DstBlend"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            auto value = m_Tokenizer.NextAsChar();
            state.DstBlend = ParseBlendType(value);
        }
        else if (m_Tokenizer.CompareAsLower("BlendOp"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            auto value = m_Tokenizer.NextAsChar();
            state.BlendOp = ParseBlendOpType(value);
        }
        else if (m_Tokenizer.CompareAsLower("SrcBlendAlpha"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            auto value = m_Tokenizer.NextAsChar();
            state.SrcBlendAlpha = ParseBlendType(value);
        }
        else if (m_Tokenizer.CompareAsLower("DstBlendAlpha"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            auto value = m_Tokenizer.NextAsChar();
            state.DstBlendAlpha = ParseBlendType(value);
        }
        else if (m_Tokenizer.CompareAsLower("BlendOpAlpha"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            auto value = m_Tokenizer.NextAsChar();
            state.BlendOpAlpha = ParseBlendOpType(value);
        }
        else if (m_Tokenizer.CompareAsLower("RenderTargetWriteMask"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.RenderTargetWriteMask = m_Tokenizer.NextAsUint();
        }

        // 次のトークンを取得.
        m_Tokenizer.Next();
    }

    if (m_BlendStates.find(name) == m_BlendStates.end() )
    { m_BlendStates[name] = state; }
}

//-----------------------------------------------------------------------------
//      ラスタライザーステートを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseRasterizerState()
{
    m_Tokenizer.Next();

    // ステート名を取得.
    auto name = std::string(m_Tokenizer.GetAsChar());

    // ステートブロック開始.
    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("{"));
    int blockCount = 1;

    RasterizerState state;

    m_Tokenizer.Next();
    while(!m_Tokenizer.IsEnd())
    {
        // パスブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            blockCount--;
            if (blockCount == 0)
            { break; }
        }
        else if (m_Tokenizer.Compare("{"))
        {
            blockCount++;
        }
        else if (m_Tokenizer.CompareAsLower("PolygonMode"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.PolygonMode = ParsePolygonMode(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("CullMode"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.CullMode = ParseCullType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("FrontCCW"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.FrontCCW = m_Tokenizer.NextAsBool();
        }
        else if (m_Tokenizer.CompareAsLower("DepthBias"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.DepthBias = uint32_t(m_Tokenizer.NextAsInt());
        }
        else if (m_Tokenizer.CompareAsLower("DepthBiasClamp"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.DepthBiasClamp = m_Tokenizer.NextAsFloat();
        }
        else if (m_Tokenizer.CompareAsLower("SlopeScaledDepthBias"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.SlopeScaledDepthBias = m_Tokenizer.NextAsFloat();
        }
        else if (m_Tokenizer.CompareAsLower("DepthClipEnable"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.DepthClipEnable = m_Tokenizer.NextAsBool();
        }
        else if (m_Tokenizer.CompareAsLower("EnableConservativeRaster"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.EnableConservativeRaster = m_Tokenizer.NextAsBool();
        }

        m_Tokenizer.Next();
    }

    if (m_RasterizerStates.find(name) == m_RasterizerStates.end())
    { m_RasterizerStates[name] = state; }
}

//-----------------------------------------------------------------------------
//      深度ステンシルステートを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseDepthStencilState()
{
    m_Tokenizer.Next();

    // ステート名を取得.
    auto name = std::string(m_Tokenizer.GetAsChar());

    // ステートブロック開始.
    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("{"));
    int blockCount = 1;

    DepthStencilState state;

    m_Tokenizer.Next();
    while(!m_Tokenizer.IsEnd())
    {
        // パスブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            blockCount--;
            if (blockCount == 0)
            { break; }
        }
        else if (m_Tokenizer.Compare("{"))
        {
            blockCount++;
        }
        else if (m_Tokenizer.CompareAsLower("DepthEnable"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.DepthEnable = m_Tokenizer.NextAsBool();
        }
        else if (m_Tokenizer.CompareAsLower("DepthWriteMask"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.DepthWriteMask = ParseDepthWriteMask(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("DepthFunc"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.DepthFunc = ParseCompareType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("StencilEnable"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.StencilEnable = m_Tokenizer.NextAsBool();
        }
        else if (m_Tokenizer.CompareAsLower("StencilReadMask"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.StencilReadMask = uint8_t(m_Tokenizer.NextAsInt());
        }
        else if (m_Tokenizer.CompareAsLower("StencilWriteMask"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.StencilWriteMask = uint8_t(m_Tokenizer.NextAsInt());
        }
        else if (m_Tokenizer.CompareAsLower("FrontFaceStencilFail"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.FrontFaceStencilFail = ParseStencilOpType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("FrontFaceStencilDepthFail"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.FrontFaceStencilDepthFail = ParseStencilOpType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("FrontFaceStencilPass"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.FrontFaceStencilPass = ParseStencilOpType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("FrontFaceStencilFunc"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.FrontFaceStencilFunc = ParseCompareType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("BackFaceStencilFail"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.BackFaceStencilFail = ParseStencilOpType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("BackFaceStencilDepthFail"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.BackFaceStencilDepthFail = ParseStencilOpType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("BackFaceStencilPass"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.BackFaceStencilPass = ParseStencilOpType(m_Tokenizer.NextAsChar());
        }
        else if (m_Tokenizer.CompareAsLower("BackFaceStencilFunc"))
        {
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("="));

            state.BackFaceStencilFunc = ParseCompareType(m_Tokenizer.NextAsChar());
        }

        m_Tokenizer.Next();
    }

    if (m_DepthStencilStates.find(name) == m_DepthStencilStates.end())
    { m_DepthStencilStates[name] = state; }
}

//-----------------------------------------------------------------------------
//      定数バッファを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseConstantBuffer()
{
    m_Tokenizer.Next();

    // 定数バッファ名を取得.
    auto name = std::string(m_Tokenizer.GetAsChar());

    // 定数バッファ名を設定.
    ConstantBuffer buffer = {};
    buffer.Name     = name;
    buffer.Register = -1;

    m_Tokenizer.Next();
    if (m_Tokenizer.Compare("register"))
    {
        m_Tokenizer.Next();
        auto regStr = std::string(m_Tokenizer.GetAsChar());
        auto regNo  = std::stoi(regStr.substr(1));
        buffer.Register = uint32_t(regNo);

        m_Tokenizer.Next();
    }

    assert(m_Tokenizer.Compare("{"));

    TYPE_MODIFIER modifier = TYPE_MODIFIER_NONE;

    while(!m_Tokenizer.IsEnd())
    {
        // テクニックブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            break;
        }
        else if (m_Tokenizer.Compare("float"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float1x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT1x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float1x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT1x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float1x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT1x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float2x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT2x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float2x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT2x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float2x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT2x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float2x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT2x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float3x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT3x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float3x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT3x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float3x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT3x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float3x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT3x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float4x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT4x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float4x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT4x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float4x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT4x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("float4x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_FLOAT4x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int1x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT1x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int1x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT1x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int1x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT1x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int2x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT2x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int2x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT2x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int2x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT2x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int2x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT2x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int3x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT3x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int3x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT3x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int3x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT3x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int3x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT3x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int4x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT4x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int4x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT4x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int4x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT4x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("int4x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_INT4x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint1x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT1x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint1x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT1x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint1x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT1x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT2x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT2x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT2x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT2x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT3x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT3x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT3x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT3x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT4x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT4x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT4x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_UINT4x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool1x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL1x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool1x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL1x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool1x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL1x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL2x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL2x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL2x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL2x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL3x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL3x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL3x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL3x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL4x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL4x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL4x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_BOOL4x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double1x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE1x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double1x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE1x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double1x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE1x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double2x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE2x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double2x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE2x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double2x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE2x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double2x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE2x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double3x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE3x1, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double3x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE3x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double3x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE3x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double3x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE3x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double4x1"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double4x2"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE4x2, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double4x3"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE4x3, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("double4x4"))
        {
            ParseConstantBufferMember(MEMBER_TYPE_DOUBLE4x4, buffer, modifier);
        }
        else if (m_Tokenizer.Compare("row_major"))
        {
            modifier = TYPE_MODIFIER_ROW_MAJOR;
            m_Tokenizer.Next();
        }
        else if (m_Tokenizer.Compare("colum_major"))
        {
            modifier = TYPE_MODIFIER_COLUMN_MAJOR;
            m_Tokenizer.Next();
        }
        else
        {
            auto name = m_Tokenizer.GetAsChar();
            if (m_Structures.find(name) != m_Structures.end())
            {
                ParseConstantBufferMember(MEMBER_TYPE_STRUCT, buffer, modifier);
            }
            else
            {
                // 次のトークンを取得.
                m_Tokenizer.Next();
            }
        }
    }

    if (m_ConstantBuffers.find(name) == m_ConstantBuffers.end())
    { m_ConstantBuffers[name] = buffer; }
}

//-----------------------------------------------------------------------------
//      定数バッファのメンバーを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseConstantBufferMember
(
    MEMBER_TYPE     type,
    ConstantBuffer& buffer,
    TYPE_MODIFIER&  modifier
)
{
    Member member = {};
    member.Type         = type;
    member.Modifier     = modifier;
    member.PackOffset   = -1;

    auto name = std::string(m_Tokenizer.NextAsChar());
    auto pos = name.find(";");
    auto end = false;
    if (pos != std::string::npos)
    {
        end = true;
        name = name.substr(0, pos);
    }

    member.Name = name;

    modifier = TYPE_MODIFIER_NONE;

    if (end)
    {
        buffer.Members.push_back(member);
        return;
    }

    m_Tokenizer.Next();
    if (m_Tokenizer.Compare("packoffset"))
    {
        auto offsetStr  = std::string(m_Tokenizer.GetAsChar());
        pos = offsetStr.find(";");
        auto offset     = std::stoi(offsetStr.substr(1, pos));
        member.PackOffset = uint32_t(offset);
    }

    buffer.Members.push_back(member);
}

//-----------------------------------------------------------------------------
//      構造体を解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseStruct()
{
    m_Tokenizer.Next();

    // 構造体名を取得.
    auto name = std::string(m_Tokenizer.GetAsChar());

    // 構造体名を設定.
    Structure structure;
    structure.Name = name;

    m_Tokenizer.Next();

    assert(m_Tokenizer.Compare("{"));

    TYPE_MODIFIER modifier = TYPE_MODIFIER_NONE;

    while(!m_Tokenizer.IsEnd())
    {
        // 構造体ブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            break;
        }
        else if (m_Tokenizer.Compare("float"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float1"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float1x2"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT1x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float1x3"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT1x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float1x4"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT1x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float2"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float2x1"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT2x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float2x2"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT2x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float2x3"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT2x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float2x4"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT2x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float3"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float3x1"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT3x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float3x2"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT3x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float3x3"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT3x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float3x4"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT3x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float4"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float4x1"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT4x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float4x2"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT4x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float4x3"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT4x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("float4x4"))
        {
            ParseStructMember(MEMBER_TYPE_FLOAT4x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int"))
        {
            ParseStructMember(MEMBER_TYPE_INT, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int1"))
        {
            ParseStructMember(MEMBER_TYPE_INT, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int1x2"))
        {
            ParseStructMember(MEMBER_TYPE_INT1x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int1x3"))
        {
            ParseStructMember(MEMBER_TYPE_INT1x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int1x4"))
        {
            ParseStructMember(MEMBER_TYPE_INT1x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int2"))
        {
            ParseStructMember(MEMBER_TYPE_INT2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int2x1"))
        {
            ParseStructMember(MEMBER_TYPE_INT2x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int2x2"))
        {
            ParseStructMember(MEMBER_TYPE_INT2x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int2x3"))
        {
            ParseStructMember(MEMBER_TYPE_INT2x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int2x4"))
        {
            ParseStructMember(MEMBER_TYPE_INT2x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int3"))
        {
            ParseStructMember(MEMBER_TYPE_INT3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int3x1"))
        {
            ParseStructMember(MEMBER_TYPE_INT3x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int3x2"))
        {
            ParseStructMember(MEMBER_TYPE_INT3x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int3x3"))
        {
            ParseStructMember(MEMBER_TYPE_INT3x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int3x4"))
        {
            ParseStructMember(MEMBER_TYPE_INT3x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int4"))
        {
            ParseStructMember(MEMBER_TYPE_INT4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int4x1"))
        {
            ParseStructMember(MEMBER_TYPE_INT4x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int4x2"))
        {
            ParseStructMember(MEMBER_TYPE_INT4x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int4x3"))
        {
            ParseStructMember(MEMBER_TYPE_INT4x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("int4x4"))
        {
            ParseStructMember(MEMBER_TYPE_INT4x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint"))
        {
            ParseStructMember(MEMBER_TYPE_UINT, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint1"))
        {
            ParseStructMember(MEMBER_TYPE_UINT, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint1x2"))
        {
            ParseStructMember(MEMBER_TYPE_UINT1x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint1x3"))
        {
            ParseStructMember(MEMBER_TYPE_UINT1x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint1x4"))
        {
            ParseStructMember(MEMBER_TYPE_UINT1x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint2"))
        {
            ParseStructMember(MEMBER_TYPE_UINT2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x1"))
        {
            ParseStructMember(MEMBER_TYPE_UINT2x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x2"))
        {
            ParseStructMember(MEMBER_TYPE_UINT2x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x3"))
        {
            ParseStructMember(MEMBER_TYPE_UINT2x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint2x4"))
        {
            ParseStructMember(MEMBER_TYPE_UINT2x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint3"))
        {
            ParseStructMember(MEMBER_TYPE_UINT3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x1"))
        {
            ParseStructMember(MEMBER_TYPE_UINT3x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x2"))
        {
            ParseStructMember(MEMBER_TYPE_UINT3x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x3"))
        {
            ParseStructMember(MEMBER_TYPE_UINT3x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint3x4"))
        {
            ParseStructMember(MEMBER_TYPE_UINT3x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint4"))
        {
            ParseStructMember(MEMBER_TYPE_UINT4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x1"))
        {
            ParseStructMember(MEMBER_TYPE_UINT4x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x2"))
        {
            ParseStructMember(MEMBER_TYPE_UINT4x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x3"))
        {
            ParseStructMember(MEMBER_TYPE_UINT4x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("uint4x4"))
        {
            ParseStructMember(MEMBER_TYPE_UINT4x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool1"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool1x2"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL1x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool1x3"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL1x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool1x4"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL1x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool2"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x1"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL2x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x2"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL2x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x3"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL2x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool2x4"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL2x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool3"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x1"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL3x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x2"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL3x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x3"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL3x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool3x4"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL3x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool4"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x1"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL4x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x2"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL4x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x3"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL4x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("bool4x4"))
        {
            ParseStructMember(MEMBER_TYPE_BOOL4x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double1"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double1x2"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE1x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double1x3"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE1x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double1x4"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE1x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double2"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double2x1"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE2x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double2x2"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE2x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double2x3"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE2x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double2x4"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE2x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double3"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double3x1"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE3x1, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double3x2"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE3x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double3x3"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE3x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double3x4"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE3x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double4"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double4x1"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double4x2"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE4x2, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double4x3"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE4x3, structure, modifier);
        }
        else if (m_Tokenizer.Compare("double4x4"))
        {
            ParseStructMember(MEMBER_TYPE_DOUBLE4x4, structure, modifier);
        }
        else if (m_Tokenizer.Compare("row_major"))
        {
            modifier = TYPE_MODIFIER_ROW_MAJOR;
            m_Tokenizer.Next();
        }
        else if (m_Tokenizer.Compare("colum_major"))
        {
            modifier = TYPE_MODIFIER_COLUMN_MAJOR;
            m_Tokenizer.Next();
        }
        else 
        {
            auto name = m_Tokenizer.GetAsChar();
            if (m_Structures.find(name) != m_Structures.end())
            {
                ParseStructMember(MEMBER_TYPE_STRUCT, structure, modifier);
            }
            else
            {
                // 次のトークンを取得.
                m_Tokenizer.Next();
            }
        }
    }

    if (m_Structures.find(name) == m_Structures.end())
    { m_Structures[name] = structure; }
}

//-----------------------------------------------------------------------------
//      構造体のメンバーを解析します
//-----------------------------------------------------------------------------
void FxParser::ParseStructMember
(
    MEMBER_TYPE     type,
    Structure&      structure,
    TYPE_MODIFIER&  modifier
)
{
    Member member = {};
    member.Type         = type;
    member.Modifier     = modifier;
    member.PackOffset   = -1;

    auto name = std::string(m_Tokenizer.NextAsChar());
    auto pos = name.find(";");
    bool end = false;
    if (pos != std::string::npos)
    {
        end = true;
        name = name.substr(0, pos);
    }

    member.Name = name;
    modifier = TYPE_MODIFIER_NONE;

    if (end)
    {
        structure.Members.push_back(member);
        return;
    }

    m_Tokenizer.Next();
    if (m_Tokenizer.Compare(":"))
    {
        auto semantics = std::string(m_Tokenizer.NextAsChar());
        pos = semantics.find(";");
        semantics = semantics.substr(0, pos);
    }

    structure.Members.push_back(member);
}

//-----------------------------------------------------------------------------
//      プロパティを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseProperties()
{
    /*
        Properties
        {
            bool    value1("flag") = false;
            float   value1("alpha", 0.1, range(0.0f, 1.0f)) = 1.0f;
            float2  value2("uv_offset", 0.01f) = float2(0.0f, 0.0f);
            float3  value3("color_scale", 0.01f) = float3(1.0f, 1.0f, 1.0f);
            float4  value4("test", 1.0f) = float4(0.0f, 0.0f, 0.0f, 0.0f);
            color3  value5("emissive") = color3(0.0f, 0.0f, 0.0f);
            color4  value6("base_color") = color4(0.0f, 0.0f, 0.0f, 1.0f);
            map1D   myTexture1("noise", false) = "texture_file_name";
            map2D   myTexture2("roughness") = "white";
            map3D   myTexture3("volume", true) = "white";
            mapCube mayTexture4("ibl_diffuse", false) = "black";
        };
    
    */
    m_Tokenizer.Next();

    assert(m_Tokenizer.Compare("{"));
    int count = 1;
    m_Tokenizer.Next();


    while(!m_Tokenizer.IsEnd())
    {
        // ブロック終了.
        if (m_Tokenizer.Compare("}"))
        {
            count--;
            if (count == 0)
            { break; }
        }
        // ブロック開始.
        else if (m_Tokenizer.Compare("{"))
        {
            count++;
        }

        if (m_Tokenizer.CompareAsLower("bool"))
        {
            // 次のフォーマット.
            // bool name("display") = default;

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            auto defValue = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(";"));

            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_BOOL;
            prop.Step           = 0;
            prop.Min            = 0.0f;
            prop.Max            = 0.0f;
            prop.DefaultValue0  = defValue;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("int"))
        {
            // 次のフォーマット
            // int name("display", step, range(min, max)) = default;

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            auto step = m_Tokenizer.NextAsFloat();
            m_Tokenizer.Next();

            float mini = 0.0f;
            float maxi = 0.0f;
            if (m_Tokenizer.CompareAsLower("range"))
            {
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare("("));
                mini = m_Tokenizer.NextAsFloat();
                maxi = m_Tokenizer.NextAsFloat();
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare(")"));
                m_Tokenizer.Next();
            }

            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            auto defValue = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(";"));

            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_INT;
            prop.Step           = step;
            prop.Min            = mini;
            prop.Max            = maxi;
            prop.DefaultValue0  = defValue;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("float"))
        {
            // 次のフォーマット
            // float name("display", step, range(min, max)) = default;

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            auto step = m_Tokenizer.NextAsFloat();
            m_Tokenizer.Next();

            float mini = 0.0f;
            float maxi = 0.0f;
            if (m_Tokenizer.CompareAsLower("range"))
            {
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare("("));
                mini = m_Tokenizer.NextAsFloat();
                maxi = m_Tokenizer.NextAsFloat();
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare(")"));
                m_Tokenizer.Next();
            }

            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            auto defValue = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(";"));
            m_Tokenizer.Next();


            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_FLOAT;
            prop.Step           = step;
            prop.Min            = mini;
            prop.Max            = maxi;
            prop.DefaultValue0  = defValue;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("float2"))
        {
            // 次のフォーマット
            // float2 name("display", step, range(min, max)) = float2(default.x, default.y);

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            auto step = m_Tokenizer.NextAsFloat();
            m_Tokenizer.Next();

            float mini = 0.0f;
            float maxi = 0.0f;
            if (m_Tokenizer.CompareAsLower("range"))
            {
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare("("));
                mini = m_Tokenizer.NextAsFloat();
                maxi = m_Tokenizer.NextAsFloat();
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare(")"));
                m_Tokenizer.Next();
            }

            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("float2"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("("));
            auto defValueX = std::string(m_Tokenizer.NextAsChar());
            auto defValueY = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(";"));
            m_Tokenizer.Next();

            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_FLOAT2;
            prop.Step           = step;
            prop.Min            = mini;
            prop.Max            = maxi;
            prop.DefaultValue0  = defValueX;
            prop.DefaultValue1  = defValueY;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("float3"))
        {
            // 次のフォーマット
            // float3 name("display", step, range(min, max)) = float3(default.x, default.y, default.z);

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            auto step = m_Tokenizer.NextAsFloat();
            m_Tokenizer.Next();

            float mini = 0.0f;
            float maxi = 0.0f;
            if (m_Tokenizer.CompareAsLower("range"))
            {
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare("("));
                mini = m_Tokenizer.NextAsFloat();
                maxi = m_Tokenizer.NextAsFloat();
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare(")"));
                m_Tokenizer.Next();
            }

            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("float3"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("("));
            auto defValueX = std::string(m_Tokenizer.NextAsChar());
            auto defValueY = std::string(m_Tokenizer.NextAsChar());
            auto defValueZ = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(";"));
            m_Tokenizer.Next();

            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_FLOAT3;
            prop.Step           = step;
            prop.Min            = mini;
            prop.Max            = maxi;
            prop.DefaultValue0  = defValueX;
            prop.DefaultValue1  = defValueY;
            prop.DefaultValue2  = defValueZ;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("float4"))
        {
            // 次のフォーマット
            // float4 name("display", step, range(min, max)) = float4(default.x, default.y, default.z, default.w);

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            auto step = m_Tokenizer.NextAsFloat();
            m_Tokenizer.Next();

            float mini = 0.0f;
            float maxi = 0.0f;
            if (m_Tokenizer.CompareAsLower("range"))
            {
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare("("));
                mini = m_Tokenizer.NextAsFloat();
                maxi = m_Tokenizer.NextAsFloat();
                m_Tokenizer.Next();
                assert(m_Tokenizer.Compare(")"));
                m_Tokenizer.Next();
            }

            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("float4"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("("));
            auto defValueX = std::string(m_Tokenizer.NextAsChar());
            auto defValueY = std::string(m_Tokenizer.NextAsChar());
            auto defValueZ = std::string(m_Tokenizer.NextAsChar());
            auto defValueW = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(";"));
            m_Tokenizer.Next();

            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_FLOAT4;
            prop.Step           = step;
            prop.Min            = mini;
            prop.Max            = maxi;
            prop.DefaultValue0  = defValueX;
            prop.DefaultValue1  = defValueY;
            prop.DefaultValue2  = defValueZ;
            prop.DefaultValue3  = defValueW;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("color3"))
        {
            // 次のフォーマット
            // color3 name("display") = color3(default.r, default.g, default.b);

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("color3"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("("));
            auto defValueX = std::string(m_Tokenizer.NextAsChar());
            auto defValueY = std::string(m_Tokenizer.NextAsChar());
            auto defValueZ = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_COLOR3;
            prop.Step           = 0.0f;
            prop.Min            = 0.0f;
            prop.Max            = 0.0f;
            prop.DefaultValue0  = defValueX;
            prop.DefaultValue1  = defValueY;
            prop.DefaultValue2  = defValueZ;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("color4"))
        {
            // 次のフォーマット
            // color4 name("display") = color4(default.r, default.g, default.b, default.a);

            auto name = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("("));
            auto display_tag = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("="));
            m_Tokenizer.Next();

            assert(m_Tokenizer.Compare("color4"));
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare("("));
            auto defValueX = std::string(m_Tokenizer.NextAsChar());
            auto defValueY = std::string(m_Tokenizer.NextAsChar());
            auto defValueZ = std::string(m_Tokenizer.NextAsChar());
            auto defValueW = std::string(m_Tokenizer.NextAsChar());
            m_Tokenizer.Next();
            assert(m_Tokenizer.Compare(")"));
            m_Tokenizer.Next();

            ValueProperty prop;
            prop.Name           = name;
            prop.DisplayTag     = display_tag;
            prop.Type           = PROPERTY_TYPE_COLOR4;
            prop.Step           = 0.0f;
            prop.Min            = 0.0f;
            prop.Max            = 0.0f;
            prop.DefaultValue0  = defValueX;
            prop.DefaultValue1  = defValueY;
            prop.DefaultValue2  = defValueZ;
            prop.DefaultValue3  = defValueW;

            if (m_ValueProperties.find(prop.Name) == m_ValueProperties.end())
            { m_ValueProperties[prop.Name] = prop; }
        }
        else if (m_Tokenizer.CompareAsLower("map1d"))
        {
            ParseTextureProperty(PROPERTY_TYPE_TEXTURE1D);
        }
        else if (m_Tokenizer.CompareAsLower("map1darray"))
        {
            ParseTextureProperty(PROPERTY_TYPE_TEXTURE1D_ARRAY);
        }
        else if (m_Tokenizer.CompareAsLower("map2d"))
        {
            ParseTextureProperty(PROPERTY_TYPE_TEXTURE2D);
        }
        else if (m_Tokenizer.CompareAsLower("map2darray"))
        {
            ParseTextureProperty(PROPERTY_TYPE_TEXTURE2D_ARRAY);
        }
        else if (m_Tokenizer.CompareAsLower("map3d"))
        {
            ParseTextureProperty(PROPERTY_TYPE_TEXTURE3D);
        }
        else if (m_Tokenizer.CompareAsLower("mapcube"))
        {
            ParseTextureProperty(PROPERTY_TYPE_TEXTURECUBE);
        }
        else if (m_Tokenizer.CompareAsLower("mapcubearray"))
        {
            ParseTextureProperty(PROPERTY_TYPE_TEXTURECUBE_ARRAY);
        }
        else
        {
            m_Tokenizer.Next();
        }
    }

    if (!m_ValueProperties.empty())
    {
        m_SourceCode += "cbuffer CbProperties\n";
        m_SourceCode += "{\n";

        for(auto& itr : m_ValueProperties)
        {
            auto& prop = itr.second;
            m_SourceCode += "    ";
            switch(prop.Type)
            {
            case PROPERTY_TYPE_BOOL:
                {
                    m_SourceCode += "int";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_INT:
                {
                    m_SourceCode += "int";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_FLOAT:
                {
                    m_SourceCode += "float";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_FLOAT2:
                {
                    m_SourceCode += "float2";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_FLOAT3:
                {
                    m_SourceCode += "float3";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_FLOAT4:
                {
                    m_SourceCode += "float4";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_COLOR3:
                {
                    m_SourceCode += "float3";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_COLOR4:
                {
                    m_SourceCode += "float4";
                    m_SourceCode += " ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;
            }
        }

        m_SourceCode += "};\n\n";
    }

    if (!m_TextureProperties.empty())
    {
        for(auto& itr : m_TextureProperties)
        {
            auto& prop = itr.second;
            switch(prop.Type)
            {
            case PROPERTY_TYPE_TEXTURE1D:
                {
                    m_SourceCode += "Texture1D ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_TEXTURE1D_ARRAY:
                {
                    m_SourceCode += "Texture1DArray ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_TEXTURE2D:
                {
                    m_SourceCode += "Texture2D ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_TEXTURE2D_ARRAY:
                {
                    m_SourceCode += "Texture2DArray ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_TEXTURE3D:
                {
                    m_SourceCode += "Texture3D ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_TEXTURECUBE:
                {
                    m_SourceCode += "TextureCube ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;

            case PROPERTY_TYPE_TEXTURECUBE_ARRAY:
                {
                    m_SourceCode += "TextureCubeArray ";
                    m_SourceCode += prop.Name;
                    m_SourceCode += ";";
                    m_SourceCode += "    //";
                    m_SourceCode += prop.DisplayTag;
                    m_SourceCode += "\n";
                }
                break;
            }
        }
        m_SourceCode += "\n";
    }
}

//-----------------------------------------------------------------------------
//      テクスチャプロパティを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseTextureProperty(PROPERTY_TYPE type)
{
    // 次のフォーマット.
    // mapXXX name("display", srgb) = default;
    auto srgb = false;

    auto name = std::string(m_Tokenizer.NextAsChar());
    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("("));
    auto display_tag = std::string(m_Tokenizer.NextAsChar());

    m_Tokenizer.Next();
    if (!m_Tokenizer.Compare(")"))
    {
        srgb = m_Tokenizer.GetAsBool();
        m_Tokenizer.Next();
    }
    assert(m_Tokenizer.Compare(")"));

    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare("="));

    auto defValue = std::string(m_Tokenizer.NextAsChar());
    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare(";"));
    m_Tokenizer.Next();

    TextureProperty prop;
    prop.Name           = name;
    prop.DisplayTag     = display_tag;
    prop.Type           = type;
    prop.SRGB           = srgb;
    prop.DefaultValue   = defValue;

    if (m_TextureProperties.find(prop.Name) == m_TextureProperties.end())
    { m_TextureProperties[prop.Name] = prop; }
}

//-----------------------------------------------------------------------------
//      リソースを解析します.
//-----------------------------------------------------------------------------
void FxParser::ParseResource()
{
    Resource res = {};

    if (m_Tokenizer.CompareAsLower("Texture1D"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURE1D);
    }
    else if (m_Tokenizer.CompareAsLower("Texture1DArray"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURE1DARRAY);
    }
    else if (m_Tokenizer.CompareAsLower("Texture2D"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURE2D);
    }
    else if (m_Tokenizer.CompareAsLower("Texture2DArray"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURE2DARRAY);
    }
    else if (m_Tokenizer.CompareAsLower("Texture2DMS"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURE2DMS);
    }
    else if (m_Tokenizer.CompareAsLower("Texture2DMSArray"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURE2DMSARRAY);
    }
    else if (m_Tokenizer.CompareAsLower("Texture3D"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURE3D);
    }
    else if (m_Tokenizer.CompareAsLower("TextureCube"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURECUBE);
    }
    else if (m_Tokenizer.CompareAsLower("TextureCubeArray"))
    {
        ParseResourceDetail(RESOURCE_TYPE_TEXTURECUBEARRAY);
    }
    else if (m_Tokenizer.CompareAsLower("Buffer"))
    {
        ParseResourceDetail(RESOURCE_TYPE_BUFFER);
    }
    else if (m_Tokenizer.CompareAsLower("ByteAddressBuffer"))
    {
        ParseResourceDetail(RESOURCE_TYPE_BYTEADDRESS_BUFFER);
    }
    else if (m_Tokenizer.CompareAsLower("StructuredBuffer"))
    {
        ParseResourceDetail(RESOURCE_TYPE_STRUCTURED_BUFFER);
    }
    else if (m_Tokenizer.CompareAsLower("RWTexture1D"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWTEXTURE1D);
    }
    else if (m_Tokenizer.CompareAsLower("RWTexture1DArray"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWTEXTURE1DARRAY);
    }
    else if (m_Tokenizer.CompareAsLower("RWTexture2D"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWTEXTURE2D);
    }
    else if (m_Tokenizer.CompareAsLower("RWTexture2DArray"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWTEXTURE2DARRAY);
    }
    else if (m_Tokenizer.CompareAsLower("RWTexture3D"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWTEXTURE3D);
    }
    else if (m_Tokenizer.CompareAsLower("RWBuffer"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWBUFFER);
    }
    else if (m_Tokenizer.CompareAsLower("RWByteAddressBuffer"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWBYTEADDRESS_BUFFER);
    }
    else if (m_Tokenizer.CompareAsLower("RWStructuredBuffer"))
    {
        ParseResourceDetail(RESOURCE_TYPE_RWSTRUCTURED_BUFFER);
    }
    else if (m_Tokenizer.CompareAsLower("SamplerState"))
    {
        ParseResourceDetail(RESOURCE_TYPE_SAMPLER_STATE);
    }
    else if (m_Tokenizer.CompareAsLower("SamplerComparisonState"))
    {
        ParseResourceDetail(RESOURCE_TYPE_SAMPLER_COMPRISON_STATE);
    }
}

//-----------------------------------------------------------------------------
//      リソース解析の実態.
//-----------------------------------------------------------------------------
void FxParser::ParseResourceDetail(RESOURCE_TYPE type)
{
    m_Tokenizer.Next();

    MEMBER_TYPE dataType = MEMBER_TYPE_UNKNOWN;

    if (m_Tokenizer.Compare("<"))
    {
        m_Tokenizer.Next();

        if (m_Tokenizer.CompareAsLower("float"))
        {
            dataType = MEMBER_TYPE_FLOAT;
        }
        else if (m_Tokenizer.CompareAsLower("float2"))
        {
            dataType = MEMBER_TYPE_FLOAT2;
        }
        else if (m_Tokenizer.CompareAsLower("float3"))
        {
            dataType = MEMBER_TYPE_FLOAT3;
        }
        else if (m_Tokenizer.CompareAsLower("float4"))
        {
            dataType = MEMBER_TYPE_FLOAT4;
        }
        else if (m_Tokenizer.CompareAsLower("double"))
        {
            dataType = MEMBER_TYPE_DOUBLE;
        }
        else if (m_Tokenizer.CompareAsLower("double2"))
        {
            dataType = MEMBER_TYPE_DOUBLE2;
        }
        else if (m_Tokenizer.CompareAsLower("double3"))
        {
            dataType = MEMBER_TYPE_DOUBLE3;
        }
        else if (m_Tokenizer.CompareAsLower("double4"))
        {
            dataType = MEMBER_TYPE_DOUBLE4;
        }
        else if (m_Tokenizer.CompareAsLower("int"))
        {
            dataType = MEMBER_TYPE_INT;
        }
        else if (m_Tokenizer.CompareAsLower("int2"))
        {
            dataType = MEMBER_TYPE_INT2;
        }
        else if (m_Tokenizer.CompareAsLower("int3"))
        {
            dataType = MEMBER_TYPE_INT3;
        }
        else if (m_Tokenizer.CompareAsLower("int4"))
        {
            dataType = MEMBER_TYPE_INT4;
        }
        else if (m_Tokenizer.CompareAsLower("uint"))
        {
            dataType = MEMBER_TYPE_UINT;
        }
        else if (m_Tokenizer.CompareAsLower("uint2"))
        {
            dataType = MEMBER_TYPE_UINT2;
        }
        else if (m_Tokenizer.CompareAsLower("uint3"))
        {
            dataType = MEMBER_TYPE_UINT3;
        }
        else if (m_Tokenizer.CompareAsLower("uint4"))
        {
            dataType = MEMBER_TYPE_UINT4;
        }
        else
        {
            auto name = m_Tokenizer.GetAsChar();
            if (m_Structures.find(name) != m_Structures.end())
            {
                dataType = MEMBER_TYPE_STRUCT;
            }
        }

        m_Tokenizer.Next();
        assert(m_Tokenizer.Compare(">"));
        m_Tokenizer.Next();
    }

    auto name = std::string(m_Tokenizer.GetAsChar());
    auto pos = name.find(";");
    auto end = false;
    if (pos != std::string::npos)
    {
        name = name.substr(0, pos);
        end  = true;
    }

    Resource res = {};
    res.Name            = name;
    res.ResourceType    = type;
    res.Register        = -1;
    res.DataType        = dataType;

    if (end)
    {
        if (m_Resources.find(name) == m_Resources.end())
        {
            m_Resources[name] = res;
            return;
        }
    }

    m_Tokenizer.Next();
    assert(m_Tokenizer.Compare(":"));
    m_Tokenizer.Next();

    if (m_Tokenizer.CompareAsLower("register"))
    {
        m_Tokenizer.Next();
        assert(m_Tokenizer.Compare("("));
        auto reg = std::string(m_Tokenizer.NextAsChar());
        auto idx = std::stoi(reg.substr(1));
        res.Register = uint32_t(idx);
        m_Tokenizer.Next();
        assert(m_Tokenizer.Compare(")"));
    }

    if (m_Resources.find(name) == m_Resources.end())
    {
        m_Resources[name] = res;
    }
}

//-----------------------------------------------------------------------------
//      トークンをシェーダタイプとして取得します.
//-----------------------------------------------------------------------------
SHADER_TYPE FxParser::GetShaderType()
{
    SHADER_TYPE type = SHADER_TYPE(-1);

    if (m_Tokenizer.CompareAsLower("vertexshader"))
    {
        type = SHADER_TYPE_VERTEX;
    }
    else if (m_Tokenizer.CompareAsLower("pixelshader"))
    {
        type = SHADER_TYPE_PIXEL;
    }
    else if (m_Tokenizer.CompareAsLower("geometryshader"))
    {
        type = SHADER_TYPE_GEOMETRY;
    }
    else if (m_Tokenizer.CompareAsLower("domainshader"))
    {
        type = SHADER_TYPE_DOMAIN;
    }
    else if (m_Tokenizer.CompareAsLower("hullshader"))
    {
        type = SHADER_TYPE_HULL;
    }
    else if (m_Tokenizer.CompareAsLower("computeshader"))
    {
        type = SHADER_TYPE_COMPUTE;
    }

    return type;
}

//-------------------------------------------------------------------------------------------------
//      バリエーション情報を書き出します.
//-------------------------------------------------------------------------------------------------
bool FxParser::WriteVariationInfo(const char* xmlpath, const char* hlslpath)
{
    FILE* pFile;

    auto err = fopen_s(&pFile, xmlpath, "w");
    if ( err != 0 )
    {
        ELOG( "Error : File Open Failed. filename = %s", xmlpath );
        return false;
    }

    fprintf_s(pFile, u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    fprintf_s(pFile, u8"<root>\n");
    fprintf_s(pFile, u8"    <source path=\"%s\" />\n", hlslpath);

    if (!m_RasterizerStates.empty())
    {
        for(auto& itr : m_RasterizerStates)
        {
            auto& state = itr.second;
            fprintf_s(pFile, u8"    <rasterizer_state name=\"%s\" ", itr.first.c_str());
            fprintf_s(pFile, u8"polygon_mode=\"%s\" ", ToString(state.PolygonMode));
            fprintf_s(pFile, u8"cull_mode=\"%s\" ", ToString(state.CullMode));
            fprintf_s(pFile, u8"front_ccw=\"%s\" ", state.FrontCCW ? "true" : "false");
            fprintf_s(pFile, u8"depth_bias=\"%s\" ", std::to_string(state.DepthBias).c_str());
            fprintf_s(pFile, u8"depth_bias_clamp=\"%s\" ", std::to_string(state.DepthBiasClamp).c_str());
            fprintf_s(pFile, u8"depth_clip_enable=\"%s\" ", state.DepthClipEnable ? "true" : "false");
            fprintf_s(pFile, u8"enable_consevative_raster=\"%s\" ", state.EnableConservativeRaster ? "true" : "false");
            fprintf_s(pFile, u8"/>\n");
        }
    }

    if (!m_DepthStencilStates.empty())
    {
        for(auto& itr : m_DepthStencilStates)
        {
            auto& state = itr.second;
            fprintf_s(pFile, u8"    <depthsencil_state name=\"%s\" ", itr.first.c_str());
            fprintf_s(pFile, u8"depth_enable=\"%s\" ", state.DepthEnable ? "true" : "false");
            fprintf_s(pFile, u8"depth_write_mask=\"%s\" ", ToString(state.DepthWriteMask));
            fprintf_s(pFile, u8"depth_func=\"%s\" ", ToString(state.DepthFunc));
            fprintf_s(pFile, u8"stencil_enable=\"%s\" ", state.StencilEnable ? "true" : "false");
            fprintf_s(pFile, u8"stencil_read_mask=\"0x%x\" ", state.StencilReadMask);
            fprintf_s(pFile, u8"stencil_write_mask=\"0x%x\" ", state.StencilWriteMask);
            fprintf_s(pFile, u8"front_face_stencil_fail=\"%s\" ", ToString(state.FrontFaceStencilFail));
            fprintf_s(pFile, u8"front_face_stencil_depth_fail=\"%s\" ", ToString(state.FrontFaceStencilDepthFail));
            fprintf_s(pFile, u8"front_face_stencil_pass=\"%s\" ", ToString(state.FrontFaceStencilPass));
            fprintf_s(pFile, u8"back_face_stencil_fail=\"%s\" ", ToString(state.BackFaceStencilFail));
            fprintf_s(pFile, u8"back_face_stencil_depth_fail=\"%s\" ", ToString(state.BackFaceStencilDepthFail));
            fprintf_s(pFile, u8"back_face_stencil_pass=\"%s\" ", ToString(state.BackFaceStencilPass));
            fprintf_s(pFile, u8"back_face_stencil_func=\"%s\" ", ToString(state.BackFaceStencilFunc));
            fprintf_s(pFile, u8"/>\n");
        }
    }

    if (!m_BlendStates.empty())
    {
        for(auto& itr : m_BlendStates)
        {
            auto& state = itr.second;
            fprintf_s(pFile, u8"    <blend_state name=\"%s\" ", itr.first.c_str());
            fprintf_s(pFile, u8"alpha_to_coverage_enable=\"%s\" ", state.AlphaToCoverageEnable ? "true" : "false");
            fprintf_s(pFile, u8"blend_enable=\"%s\" ", state.BlendEnable ? "true" : "false");
            fprintf_s(pFile, u8"src_blend=\"%s\" ", ToString(state.SrcBlend));
            fprintf_s(pFile, u8"dst_blend=\"%s\" ", ToString(state.DstBlend));
            fprintf_s(pFile, u8"blend_op=\"%s\" ", ToString(state.BlendOp));
            fprintf_s(pFile, u8"src_blend_alpha=\"%s\" ", ToString(state.SrcBlendAlpha));
            fprintf_s(pFile, u8"dst_blend_alpha=\"%s\" ", ToString(state.DstBlendAlpha));
            fprintf_s(pFile, u8"blend_op_alpha=\"%s\" ", ToString(state.BlendOpAlpha));
            fprintf_s(pFile, u8"render_target_write_mask=\"0x%x\" ", state.RenderTargetWriteMask);
            fprintf_s(pFile, u8"/>\n");
        }
    }

    if (!m_ValueProperties.empty() || !m_TextureProperties.empty())
    {
        fprintf_s(pFile, u8"    <properties>\n");
        for(auto& itr : m_ValueProperties)
        {
            auto& prop = itr.second;
            switch(prop.Type)
            {
            case PROPERTY_TYPE_BOOL:
                {
                    fprintf_s(pFile, u8"        <bool name=\"%s\" display_tag=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.DefaultValue0.c_str());
                }
                break;

            case PROPERTY_TYPE_INT:
                {
                    fprintf_s(pFile, u8"        <int name=\"%s\" display_tag=\"%s\" step=\"%s\" min=\"%s\" max=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        std::to_string(prop.Step).c_str(),
                        std::to_string(prop.Min).c_str(),
                        std::to_string(prop.Max).c_str(),
                        prop.DefaultValue0.c_str());
                }
                break;

            case PROPERTY_TYPE_FLOAT:
                {
                    fprintf_s(pFile, u8"        <float name=\"%s\" display_tag=\"%s\" step=\"%s\" min=\"%s\" max=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        std::to_string(prop.Step).c_str(),
                        std::to_string(prop.Min).c_str(),
                        std::to_string(prop.Max).c_str(),
                        prop.DefaultValue0.c_str());
                }
                break;

            case PROPERTY_TYPE_FLOAT2:
                {
                    fprintf_s(pFile, u8"        <float2 name=\"%s\" display_tag=\"%s\" step=\"%s\" min=\"%s\" max=\"%s\" x=\"%s\" y=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        std::to_string(prop.Step).c_str(),
                        std::to_string(prop.Min).c_str(),
                        std::to_string(prop.Max).c_str(),
                        prop.DefaultValue0.c_str(),
                        prop.DefaultValue1.c_str());
                }
                break;

            case PROPERTY_TYPE_FLOAT3:
                {
                    fprintf_s(pFile, u8"        <float3 name=\"%s\" display_tag=\"%s\" step=\"%s\" min=\"%s\" max=\"%s\" x=\"%s\" y=\"%s\" z=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        std::to_string(prop.Step).c_str(),
                        std::to_string(prop.Min).c_str(),
                        std::to_string(prop.Max).c_str(),
                        prop.DefaultValue0.c_str(),
                        prop.DefaultValue1.c_str(),
                        prop.DefaultValue2.c_str());
                }
                break;

            case PROPERTY_TYPE_FLOAT4:
                {
                    fprintf_s(pFile, u8"        <float4 name=\"%s\" display_tag=\"%s\" step=\"%s\" min=\"%s\" max=\"%s\" x=\"%s\" y=\"%s\" z=\"%s\" w=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        std::to_string(prop.Step).c_str(),
                        std::to_string(prop.Min).c_str(),
                        std::to_string(prop.Max).c_str(),
                        prop.DefaultValue0.c_str(),
                        prop.DefaultValue1.c_str(),
                        prop.DefaultValue2.c_str(),
                        prop.DefaultValue3.c_str());
                }
                break;

            case PROPERTY_TYPE_COLOR3:
                {
                    fprintf_s(pFile, u8"        <color3 name=\"%s\" display_tag=\"%s\" r=\"%s\" g=\"%s\" b=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.DefaultValue0.c_str(),
                        prop.DefaultValue1.c_str(),
                        prop.DefaultValue2.c_str());
                }
                break;

            case PROPERTY_TYPE_COLOR4:
                {
                    fprintf_s(pFile, u8"        <color4 name=\"%s\" display_tag=\"%s\" r=\"%s\" g=\"%s\" b=\"%s\" a=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.DefaultValue0.c_str(),
                        prop.DefaultValue1.c_str(),
                        prop.DefaultValue2.c_str(),
                        prop.DefaultValue3.c_str());
                }
                break;
            }
        }

        for(auto& itr : m_TextureProperties)
        {
            auto& prop = itr.second;
            switch(prop.Type)
            {
            case PROPERTY_TYPE_TEXTURE1D:
                {
                    fprintf_s(pFile, u8"        <map1d name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.SRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case PROPERTY_TYPE_TEXTURE1D_ARRAY:
                {
                    fprintf_s(pFile, u8"        <map1darray name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.SRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case PROPERTY_TYPE_TEXTURE2D:
                {
                    fprintf_s(pFile, u8"        <map2d name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.SRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case PROPERTY_TYPE_TEXTURE2D_ARRAY:
                {
                    fprintf_s(pFile, u8"        <map2darray name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.SRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case PROPERTY_TYPE_TEXTURE3D:
                {
                    fprintf_s(pFile, u8"        <map3d name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.SRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case PROPERTY_TYPE_TEXTURECUBE:
                {
                    fprintf_s(pFile, u8"        <mapcube name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.SRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case PROPERTY_TYPE_TEXTURECUBE_ARRAY:
                {
                    fprintf_s(pFile, u8"        <mapcubearray name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.SRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;
            }
        }
        fprintf_s(pFile, u8"    </properties>\n\n");
    }

    for(size_t i=0; i<m_Technieues.size(); ++i)
    {
        auto& technique = m_Technieues[i];

        fprintf_s(pFile, u8"    <technique name=\"%s\">\n", technique.Name.c_str());

        for(size_t j=0; j<m_Technieues[i].Pass.size(); ++j)
        {
            auto& pass = technique.Pass[j];

            fprintf_s(pFile, u8"        <pass name=\"%s\">\n", pass.Name.c_str());
            for(size_t k=0; k<pass.Shaders.size(); ++k)
            {
                auto& shader = pass.Shaders[k];
                fprintf_s(pFile, u8"            <shader type=\"%s\" profile=\"%s\" name=\"%s\"/>\n", ShaderTypeString(shader.Type), shader.Profile.c_str(), shader.EntryPoint.c_str());
            }
            if (!pass.RS.empty())
            {
                fprintf_s(pFile, u8"            <rs name=\"%s\"/>\n", pass.RS.c_str());
            }
            if (!pass.DSS.empty())
            {
                fprintf_s(pFile, u8"            <dss name=\"%s\"/>\n", pass.DSS.c_str());
            }
            if (!pass.BS.empty())
            {
                fprintf_s(pFile, u8"            <bs name=\"%s\"/>\n", pass.BS.c_str());
            }

            fprintf_s(pFile, u8"        </pass>\n");
        }

        fprintf_s(pFile, u8"    </technique>\n\n");
    }

    fprintf_s(pFile, u8"</root>\n");
    fclose(pFile);

    return true;
}

//-------------------------------------------------------------------------------------------------
//      ソースコードを出力します.
//-------------------------------------------------------------------------------------------------
bool FxParser::WriteSourceCode(const char* filename)
{
    FILE* pFile;

    auto err = fopen_s(&pFile, filename, "w");
    if ( err != 0 )
    {
        ELOG( "Error : File Open Failed. filename = %s", filename );
        return false;
    }

    fprintf_s(pFile,"%s", m_SourceCode.c_str());
    fclose(pFile);

    return true;
}

//-----------------------------------------------------------------------------
//      ソースコードを取得します.
//-----------------------------------------------------------------------------
const char* FxParser::GetSourceCode() const
{ return m_SourceCode.c_str(); }

//-----------------------------------------------------------------------------
//      ソースコードサイズを取得します.
//-----------------------------------------------------------------------------
size_t FxParser::GetSourceCodeSize() const
{ return m_SourceCode.size(); }

//-----------------------------------------------------------------------------
//      ブレンドステートを取得します.
//-----------------------------------------------------------------------------
const std::map<std::string, BlendState>& FxParser::GetBlendStates() const
{ return m_BlendStates; }

//-----------------------------------------------------------------------------
//      ラスタライザーステートを取得します.
//-----------------------------------------------------------------------------
const std::map<std::string, RasterizerState>& FxParser::GetRasterizerStates() const
{ return m_RasterizerStates; }

//-----------------------------------------------------------------------------
//      深度ステンシルステートを取得します.
//-----------------------------------------------------------------------------
const std::map<std::string, DepthStencilState>& FxParser::GetDepthStencilStates() const
{ return m_DepthStencilStates; }

//-----------------------------------------------------------------------------
//      定数バッファを取得します.
//-----------------------------------------------------------------------------
const std::map<std::string, ConstantBuffer>& FxParser::GetConstantBuffers() const
{ return m_ConstantBuffers; }

//-----------------------------------------------------------------------------
//      構造体を取得します.
//-----------------------------------------------------------------------------
const std::map<std::string, Structure>& FxParser::GetStructures() const
{ return m_Structures; }

//-----------------------------------------------------------------------------
//      リソースを取得します.
//-----------------------------------------------------------------------------
const std::map<std::string, Resource>& FxParser::GetResources() const
{ return m_Resources; }

//-----------------------------------------------------------------------------
//      テクニックを取得します.
//-----------------------------------------------------------------------------
const std::vector<Technique>& FxParser::GetTechniques() const
{ return m_Technieues; }

} // namespace asura
