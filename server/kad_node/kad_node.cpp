// NetAppFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "kad_node_netunit.h"
#include "task_mgr.h"

#ifdef _MSC_VER
#include <direct.h>
#include "conio.h"
#include <process.h>
#else
#include <sys/resource.h>
#endif

DEFINE_LOG_CATEGORY(NAF_Log)
#ifdef WIN32
BOOL WINAPI ShellHandler(DWORD ctrl_type);
#else
void ShellHandler(int sig);
#endif

int main(int argc, char* argv[])
{
    STACK_TRACE;
    //
    //注册信号
#ifdef _MSC_VER
    SetConsoleCtrlHandler(&ShellHandler, TRUE);
#else
    struct sigaction sact;
    sigemptyset(&sact.sa_mask);
    sact.sa_flags = 0;
    sact.sa_handler = ShellHandler;
    //
    // for handle kill
    sigaction(SIGINT, &sact, NULL);
    sigaction(SIGTSTP, &sact, NULL);
    sigaction(SIGTERM, &sact, NULL);
#endif

    //
    oo::Log::Instance()->start("Log.conf");

    printf("would startup with %d thread", 4);
    oo::MsgPortTaskManager::instance().startup(4);        // 开启四个线程用以逻辑运算

    oo::kad_node_netunit::instance().start();
    // test
    //naf::MsgPort* pNew = new naf::MsgPort;
    //pNew->connectServer("localhost", "http");
    {
        oo::Joint::instance().run();
    }

    printf("app over!wait worker...");
    oo::MsgPortTaskManager::instance().stop();

    printf("ok, Any Key, Quit....");
    getch();

    return 0;
}

void OnConsoleCmd()
{
    // ctrl-c
    printf("input command:\n");
    printf("usage : input 'q' for quit:\n");
    printf("\t input 'dofile <filename>' for do lua file\n");
    printf("\t input 'dostr <string> ' for do lua string\n");
    std::string cmd;
    std::string str;
    std::cin >> cmd;
    if(cmd.compare("q") == 0)
    {
        oo::Joint::instance().stop();
    }
    else if(cmd.compare("dofile") == 0)
    {
        std::cin >> str;
    }
    else if(cmd.compare("dostr") == 0)
    {
        std::cin >> str;
    }
}

#ifdef WIN32
BOOL WINAPI ShellHandler(DWORD ctrl_type)
{
    switch (ctrl_type)
    {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        naf::Joint::instance().interrupt(&OnConsoleCmd);
        return TRUE;
    default:
        return FALSE;
    }
}
#else
void ShellHandler(int sig)
{
    switch(sig)
    {
    case SIGSEGV:
    case SIGABRT:
    case SIGBUS:
        {
            /* Reinstall default handler to prevent race conditions */
            signal(sig, SIG_DFL);
            /* And exit because we may have corrupted the internal
            * memory allocation lists. Use abort() if we want to
            * generate a core dump. */
            exit(-1);
        }
        break;
    case SIGTSTP:
        ///ctrl-z
        ///put into backgroud
        ///可以不处理
        break;
    case SIGTERM:
        ///程序脱离任何终端，在后台运行，那么它能接收到SIGTERM信号
        ///kill,killall 默认信号
        /// -9 时是SIGKILL无法捕获，无法做清理操作
        break;
    case SIGINT:
        ///ctrl-c
        ///这里可以做重启操作
        break;
    default:
        break;
    }
}
#endif