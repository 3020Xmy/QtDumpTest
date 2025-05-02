// dump.cpp
#include <stdio.h>
#include <windows.h>
#include <dbghelp.h>
#include <stdlib.h>

#pragma comment(lib, "Dbghelp.lib")

#include <DbgHelp.h>

/*  生成MiniDump文件的核心函数
*   hFile:指定已有文件    pExceptionPointers:异常信息指针    pwAppName:应用程序名称
*/
int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers, PWCHAR pwAppName)
{
    HANDLE hDumpFile = hFile;   //使用的文件
	BOOL bOwnDumpFile = FALSE;  //是否创建了文件
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;    //异常信息结构

	typedef BOOL(WINAPI* MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);


	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
    if (hDbgHelp) {
        pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
    }

	if (pfnMiniDumpWriteDump){
        //创建新的转储文件
		if (hDumpFile == NULL || hDumpFile == INVALID_HANDLE_VALUE)
		{
			TCHAR szFileName[MAX_PATH] = { 0 }; //文件名
			TCHAR dwBufferSize = MAX_PATH;      //缓冲区大小
			SYSTEMTIME stLocalTime;             //本地时间
			GetLocalTime(&stLocalTime);
			CreateDirectory(szFileName, NULL);

            //生成带时间戳和进程信息的文件名
			wsprintf(szFileName, "%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
				"v1.0",
				stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
				stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
				GetCurrentProcessId(), GetCurrentThreadId());
            //创建转储文件
			hDumpFile = CreateFile(szFileName,
                GENERIC_READ | GENERIC_WRITE,       //读写权限
				FILE_SHARE_WRITE | FILE_SHARE_READ, //共享模式
                0,              //安全属性
                CREATE_ALWAYS,  //总是创建新文件
                0,              //文件属性
                0);             //模板文件
			bOwnDumpFile = TRUE;
			OutputDebugString(szFileName);
		}

		//写入转储文件
		if (hDumpFile != INVALID_HANDLE_VALUE){
			//设置异常信息
			ExpParam.ThreadId = GetCurrentThreadId();
			ExpParam.ExceptionPointers = pExceptionPointers;
			ExpParam.ClientPointers = FALSE;
			//调用dump生成函数
			pfnMiniDumpWriteDump(GetCurrentProcess(),	//当前进程
				GetCurrentProcessId(),					//进程ID
				hDumpFile,								//文件句柄
				MiniDumpWithDataSegs,					//转储类型
				(pExceptionPointers ? &ExpParam : NULL),//异常信息
				NULL,									//用户流信息
				NULL);									//回调信息
			if (bOwnDumpFile) {
				CloseHandle(hDumpFile);
			}
		}
	}
	//清理资源
	if (hDbgHelp != NULL) {
		FreeLibrary(hDbgHelp);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

/*	顶层异常过滤器函数
*	lpExceptionInfo:异常信息指针
*/
LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	//如果调试器存在，继续搜索异常处理程序
	if (IsDebuggerPresent()){
		return EXCEPTION_CONTINUE_SEARCH;
	}
	return GenerateMiniDump(NULL, lpExceptionInfo, PWCHAR("test"));
}