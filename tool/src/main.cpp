//-----------------------------------------------------------------------------
// File : main.cpp
// Desc : Main Entry Point.
// Copyright(c) Project Asura. All right reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "FxParser.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")


//-----------------------------------------------------------------------------
// Constant Values.
//-----------------------------------------------------------------------------
static const char* kShaderPrefix[] = {
    "vs",
    "ds",
    "gs",
    "hs",
    "ps",
    "cs"
};

///////////////////////////////////////////////////////////////////////////////
// Argument
///////////////////////////////////////////////////////////////////////////////
struct Argument
{
    std::string InputPath;
    std::string OutputDir;
    std::string OutFxName   = "input_source.fx";
    std::string OutXmlName  = "variation.xml";
    bool        Compile     = false;
};

//-----------------------------------------------------------------------------
//      外部プロセスを実行します.
//-----------------------------------------------------------------------------
bool RunProcess(const char* cmd, bool wait)
{
    STARTUPINFOA        startup_info = {};
    PROCESS_INFORMATION process_info = {};

    DWORD flag = NORMAL_PRIORITY_CLASS;
    startup_info.cb = sizeof(STARTUPINFOA);

    // 成功すると0以外, 失敗すると0が返る.
    auto ret = CreateProcessA(
        nullptr,
        const_cast<char*>(cmd), // 実害はないはず...
        nullptr,
        nullptr,
        FALSE,
        flag,
        nullptr,
        nullptr,
        &startup_info,
        &process_info);

    if (ret == 0)
    {
        fprintf_s(stderr, "Error : プロセス起動に失敗. コマンド = %s\n", cmd);
        CloseHandle(process_info.hProcess);
        CloseHandle(process_info.hThread);
        return false;
    }

    if (wait)
    { WaitForSingleObject(process_info.hProcess, INFINITE); }

    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    return true;
}

//-----------------------------------------------------------------------------
//      コンパイルして，シェーダバイナリを出力します.
//-----------------------------------------------------------------------------
bool CompileAndOutputShader
(
    const char* source,
    size_t      sourceSize,
    const char* entryPoint,
    const char* profile,
    const char* outPath
)
{
    ID3DBlob* pBinary = nullptr;
    ID3DBlob* pError  = nullptr;

    auto ret = D3DCompile(
        source,
        sourceSize,
        nullptr,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint,
        profile,
        0,
        0,
        &pBinary,
        &pError);

    if (FAILED(ret))
    {
        fprintf_s(stderr, "Error : D3DCompile() Failed. errcode = 0x%x, message = %s\n",
                  ret, static_cast<char*>(pError->GetBufferPointer()));
        if (pBinary != nullptr)
        {
            pBinary->Release();
            pBinary = nullptr;
        }

        if (pError != nullptr)
        {
            pError->Release();
            pError = nullptr;
        }

        return false;
    }

    if (pError != nullptr)
    {
        pError->Release();
        pError = nullptr;
    }

    FILE* pFile;
    auto err = fopen_s(&pFile, outPath, "wb");
    if (err != 0)
    {
        fprintf_s(stderr, "Error : fopen_s() Failed. retcode = 0x%x\n", err);
        if (pBinary != nullptr)
        {
            pBinary->Release();
            pBinary = nullptr;
        }

        return false;
    }

    fwrite(pBinary->GetBufferPointer(), pBinary->GetBufferSize(), 1, pFile);
    fclose(pFile);

    if (pBinary != nullptr)
    {
        pBinary->Release();
        pBinary = nullptr;
    }

    if (FAILED(ret))
    {
        fprintf_s(stderr, "Error : D3DWriteBlobToFile() Failed. errcode = 0x%x\n", ret);
        return false;
    }

    return true;
}


//-----------------------------------------------------------------------------
//      バリエーション情報を書き出します.
//-----------------------------------------------------------------------------
bool WriteVariationInfo(const asura::FxParser& parser, const char* xmlpath, const char* hlslpath)
{
    FILE* pFile;

    auto err = fopen_s(&pFile, xmlpath, "w");
    if ( err != 0 )
    {
        fprintf_s(stderr, "Error : File Open Failed. filename = %s", xmlpath );
        return false;
    }

    fprintf_s(pFile, u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    fprintf_s(pFile, u8"<root>\n");
    fprintf_s(pFile, u8"    <source path=\"%s\" />\n", hlslpath);

    if (!parser.GetRasterizerStates().empty())
    {
        for(auto& itr : parser.GetRasterizerStates())
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

    if (!parser.GetDepthStencilStates().empty())
    {
        for(auto& itr : parser.GetDepthStencilStates())
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

    if (!parser.GetBlendStates().empty())
    {
        for(auto& itr : parser.GetBlendStates())
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

    if (!parser.GetProperties().Values.empty() || !parser.GetProperties().Textures.empty())
    {
        fprintf_s(pFile, u8"    <properties>\n");
        for(auto& prop : parser.GetProperties().Values)
        {
            switch(prop.Type)
            {
            case asura::PROPERTY_TYPE_BOOL:
                {
                    fprintf_s(pFile, u8"        <bool name=\"%s\" display_tag=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.DefaultValue0.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_INT:
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

            case asura::PROPERTY_TYPE_FLOAT:
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

            case asura::PROPERTY_TYPE_FLOAT2:
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

            case asura::PROPERTY_TYPE_FLOAT3:
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

            case asura::PROPERTY_TYPE_FLOAT4:
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

            case asura::PROPERTY_TYPE_COLOR3:
                {
                    fprintf_s(pFile, u8"        <color3 name=\"%s\" display_tag=\"%s\" r=\"%s\" g=\"%s\" b=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.DefaultValue0.c_str(),
                        prop.DefaultValue1.c_str(),
                        prop.DefaultValue2.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_COLOR4:
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

        for(auto& prop : parser.GetProperties().Textures)
        {
            switch(prop.Type)
            {
            case asura::PROPERTY_TYPE_TEXTURE1D:
                {
                    fprintf_s(pFile, u8"        <map1d name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.EnableSRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_TEXTURE1D_ARRAY:
                {
                    fprintf_s(pFile, u8"        <map1darray name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.EnableSRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_TEXTURE2D:
                {
                    fprintf_s(pFile, u8"        <map2d name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.EnableSRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_TEXTURE2D_ARRAY:
                {
                    fprintf_s(pFile, u8"        <map2darray name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.EnableSRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_TEXTURE3D:
                {
                    fprintf_s(pFile, u8"        <map3d name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.EnableSRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_TEXTURECUBE:
                {
                    fprintf_s(pFile, u8"        <mapcube name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.EnableSRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;

            case asura::PROPERTY_TYPE_TEXTURECUBE_ARRAY:
                {
                    fprintf_s(pFile, u8"        <mapcubearray name=\"%s\" display_tag=\"%s\" srgb=\"%s\" default=\"%s\" />\n",
                        prop.Name.c_str(),
                        prop.DisplayTag.c_str(),
                        prop.EnableSRGB ? "true" : "false",
                        prop.DefaultValue.c_str());
                }
                break;
            }
        }
        fprintf_s(pFile, u8"    </properties>\n\n");
    }


    auto techniques = parser.GetTechniques();
    for(size_t i=0; i<techniques.size(); ++i)
    {
        auto& technique = techniques[i];

        fprintf_s(pFile, u8"    <technique name=\"%s\">\n", technique.Name.c_str());

        for(size_t j=0; j<technique.Pass.size(); ++j)
        {
            auto& pass = technique.Pass[j];

            fprintf_s(pFile, u8"        <pass name=\"%s\">\n", pass.Name.c_str());
            for(size_t k=0; k<pass.Shaders.size(); ++k)
            {
                auto& shader = pass.Shaders[k];
                fprintf_s(pFile, u8"            <shader type=\"%s\" profile=\"%s\" name=\"%s\"/>\n", ToString(shader.Type), shader.Profile.c_str(), shader.EntryPoint.c_str());
            }
            if (!pass.RasterizerState.empty())
            {
                fprintf_s(pFile, u8"            <rs name=\"%s\"/>\n", pass.RasterizerState.c_str());
            }
            if (!pass.DepthStencilState.empty())
            {
                fprintf_s(pFile, u8"            <dss name=\"%s\"/>\n", pass.DepthStencilState.c_str());
            }
            if (!pass.BlendState.empty())
            {
                fprintf_s(pFile, u8"            <bs name=\"%s\"/>\n", pass.BlendState.c_str());
            }

            fprintf_s(pFile, u8"        </pass>\n");
        }

        fprintf_s(pFile, u8"    </technique>\n\n");
    }

    fprintf_s(pFile, u8"</root>\n");
    fclose(pFile);

    return true;
}

//-----------------------------------------------------------------------------
//      ソースコードを出力します.
//-----------------------------------------------------------------------------
bool WriteSourceCode(const asura::FxParser& parser, const char* filename)
{
    FILE* pFile;

    auto err = fopen_s(&pFile, filename, "w");
    if ( err != 0 )
    {
        fprintf_s(stderr, "Error : File Open Failed. filename = %s", filename );
        return false;
    }

    fprintf_s(pFile,"%s", parser.GetSourceCode());
    fclose(pFile);

    return true;
}


//-----------------------------------------------------------------------------
//      コマンドライン引数を解析します.
//-----------------------------------------------------------------------------
void ParseArg(int argc, char** argv, Argument& result)
{
    result.InputPath = argv[1];
    for(auto i=2; i<argc; ++i)
    {
        if (_stricmp(argv[i], "-o") == 0)
        {
            i++;
            result.OutputDir = argv[i];
        }

        if (_stricmp(argv[i], "-c") == 0)
        {
            result.Compile = true;
        }
    }
}

//-----------------------------------------------------------------------------
//      メインエントリーポイントです.
//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    if (argc <= 1)
    {
        printf_s("asfxc.exe input_path -o output_dir [-c]\n");
        return 0;
    }

    Argument args;
    ParseArg(argc, argv, args);
    if (args.InputPath.empty() || args.OutputDir.empty())
    {
        fprintf_s(stderr, "Error : Invalid Arguments.\n");
        return -1;
    }

    asura::FxParser parser;
    
    if (!parser.Parse(args.InputPath.c_str()))
    {
        fprintf_s(stderr, "Error : Shader Parse Failed. path = %s\n", argv[1]);
        return -1;
    }

    auto variationPath = args.OutputDir + "\\" + args.OutXmlName;
    auto sourcePath    = args.OutputDir + "\\" + args.OutFxName;

    if (!WriteVariationInfo(parser, variationPath.c_str(), args.OutFxName.c_str()))
    {
        fprintf_s(stderr, "Error : ShaderVariation Info Write Failed. path = %s\n", variationPath.c_str());
        return -1;
    }

    if (!WriteSourceCode(parser, sourcePath.c_str()))
    {
        fprintf_s(stderr, "Error : Source Code Write Failed. path = %s\n", sourcePath.c_str());
        return -1;
    }

    if (args.Compile)
    {
        auto& techniques = parser.GetTechniques();
        for(size_t i=0; i<techniques.size(); ++i)
        {
            auto& tech = techniques[i];
            for(size_t j=0; j<tech.Pass.size(); ++j)
            {
                auto& pass = tech.Pass[j];
                for(size_t k=0; k<pass.Shaders.size(); ++k)
                {
                    auto& shader = pass.Shaders[k];

                    std::string path = args.OutputDir + "\\";
                    path += tech.Name;
                    path += "_";
                    path += pass.Name;
                    path += "_";
                    path += kShaderPrefix[shader.Type];
                    path += ".hlsl";

                    if (!CompileAndOutputShader(
                        parser.GetSourceCode(),
                        parser.GetSourceCodeSize(),
                        shader.EntryPoint.c_str(),
                        shader.Profile.c_str(),
                        path.c_str()))
                    {
                        return -1;
                    }
                }
            }
        }
    }

    return 0;
}