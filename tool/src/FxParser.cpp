//-------------------------------------------------------------------------------------------------
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

    m_SourceCode.reserve( m_BufferSize );

    m_Tokenizer.SetSeparator( " \t\r\n" );
    m_Tokenizer.SetCutOff( "{}()=#<>" );
    m_Tokenizer.SetBuffer( m_pBuffer );

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
        if (output)
        {
            auto size = ptr - cur;
            m_SourceCode.append(cur, size);

        #if 0 // デバッグのための視認用.
            for(auto i=0; i<size; ++i)
            { putchar(cur[i]); }
        #endif
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
    auto ret = fopen_s( &pFile, filename, "rb" );
    if ( ret != 0 )
    {
        ELOG( "Error : File Open Failed." );
        return false;
    }

    // ファイルサイズを算出.
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
    memset(m_pBuffer, 0, sizeof(char) * (m_BufferSize + 1));

    // 一括読み込み.
    fread(m_pBuffer, sizeof(char) * m_BufferSize, 1, pFile);

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

//-------------------------------------------------------------------------------------------------
//      トークンをシェーダタイプとして取得します.
//-------------------------------------------------------------------------------------------------
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
bool FxParser::WriteVariationInfo(const char* filename)
{
    FILE* pFile;

    auto err = fopen_s(&pFile, filename, "w");
    if ( err != 0 )
    {
        ELOG( "Error : File Open Failed. filename = %s", filename );
        return false;
    }

    fprintf_s(pFile, u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    fprintf_s(pFile, u8"<root>\n");
    fprintf_s(pFile, u8"    <source path=\"%s\" />\n", filename);

    if (!m_RasterizerStates.empty())
    {
        for(auto& itr : m_RasterizerStates)
        {
            fprintf_s(pFile, u8"    <rasterizer_state name=\"%s\" ", itr.first.c_str());
            fprintf_s(pFile, u8"    </rasterizer_state>\n");
        }
    }

    if (!m_DepthStencilStates.empty())
    {
        for(auto& itr : m_DepthStencilStates)
        {
            fprintf_s(pFile, u8"    <depthsencil_state name=\"%s\" ", itr.first.c_str());
            fprintf_s(pFile, u8"    </depthstencil_state>\n");
        }
    }

    if (!m_BlendStates.empty())
    {
        for(auto& itr : m_BlendStates)
        {
            fprintf_s(pFile, u8"    <blend_state name=\"%s\" ", itr.first.c_str());
            fprintf_s(pFile, u8"    </blend_state>\n");
        }
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

        fprintf_s(pFile, u8"    </technique>\n");
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
//      テクニックを取得します.
//-----------------------------------------------------------------------------
const std::vector<Technique>& FxParser::GetTechniques() const
{ return m_Technieues; }

} // namespace asura