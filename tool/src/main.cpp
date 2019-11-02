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

    if (!parser.WriteVariationInfo(variationPath.c_str(), args.OutFxName.c_str()))
    {
        fprintf_s(stderr, "Error : ShaderVariation Info Write Failed. path = %s\n", variationPath.c_str());
        return -1;
    }

    if (!parser.WriteSourceCode(sourcePath.c_str()))
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