//
//Created by Rohit Maurya 2/10/2020.
//
#include <cstring>
#include <cstdio>
#include "../../stdimports.h"
#include "../util/File.h"
#include "symbols/string.h"
#include "List.h"
#include "main.h"
#include "symbols/Object.h"
#include "VirtualMachine.h"
#include "Thread.h"
#include "Exe.h"
#include "register.h"
#include "memory/GarbageCollector.h"
#include "Manifest.h"
#include "jit/_BaseAssembler.h"

options c_options;
int startApplication(string &e, std::list<string> &appArgs);
void pushArgumentsToStack(std::list<string>& appArgs);
void pushArgumentsToStack(Object *object, std::list<string> &appArgs);
uInt getMemBytes(const char *argv, string option);

void version() {
    cout << progname << " " << progvers << " build-" << BUILD_VERS << endl;
}

void error(string message) {
    cout << "infinity:  error: " << message << endl;
    exit(1);
}

void help() {
#ifndef infinity_PROF_
    std::cerr << "Usage: infinity" << " {OPTIONS} EXECUTABLE" << std::endl;
#endif
#ifdef infinity_PROF_
    std::cerr << "Usage: " << PROFILER_NAME << "  {OPTIONS} EXECUTABLE" << std::endl;
#endif
    cout << "Executable must be built with infinityc to be executed\n" << endl;
    cout << "[-options]\n\n    -V                     print the version number and exit" << endl;
#ifdef infinity_PROF_
    cout <<               "    -sort<id>              sort by time(tm), avg time(avgt), calls(calls), or ir(ir)." << endl;
#endif
    cout <<               "    -showversion           print the version number and continue." << endl;
    cout <<               "    -mem<size:type>        set the maximum memory allowed to the virtual machine." << endl;
    cout <<               "    -stack<size:type>      set the default physical stack size allowed to threads." << endl;
    cout <<               "    -istack<size:type>     set the default internal stack size allotted to the virtual machine." << endl;
    cout <<               "    -t<size:type>          set the minimum memory allowed to trigger the garbage collector." << endl;
    cout <<               "    -nojit                 disable runtime JIT compilation." << endl;
    cout <<               "    -slowboot              compile entire codebase at startup." << endl;
    cout <<               "    -h -?                 display this help message." << endl;
}

#define opt(v) strcmp(argv[i], v) == 0

int runtimeStart(int argc, const char* argv[])
{
    if (argc < 2) { // We expect at least 1 argument: the executable
        help();
        return 1;
    }

    string executable;
    std::list<string> appArgs;

    /**
     * We start off with allowing 64 megabytes of memory to be under
     * mangement
     */
    GarbageCollector::initilize();
    GarbageCollector::setMemoryLimit(GC_HEAP_LIMIT);
    GarbageCollector::setMemoryThreshold(MB_TO_BYTES(12));

    for (int i = 1; i < argc; ++i) {
        if(opt("-V")){
            version();
            exit(0);
        }
        else if(opt("-h") || opt("-?")){
            help();
            exit(0);
        }
        else if(opt("-showversion")){
            version();
        }
        else if(opt("-nojit")){
            c_options.jit = false;
        }
        else if(opt("-slowboot")){
            c_options.slowBoot = true;
        }
        else if(opt("-debug")) {
            c_options.debugMode = true;
        }
        else if(opt("-threshold") || opt("-t")) {
            if((i+1) >= argc)
                error("expected argument after option `" + string(argv[i]) + "`");
            GarbageCollector::setMemoryThreshold(getMemBytes(argv[i+1], argv[i]));
            i++;
        }
#ifdef infinity_PROF_
        else if(opt("-sort") || opt("-sortby")) {
            if((i+1) >= argc)
                error("expected argument after option `-sort`");

            i++;
            if(opt("tm")) {
                c_options.sortBy = profilerSort::tm;
            } else if(opt("avgt") || opt("avgtm")) {
                c_options.sortBy = avgt;
            } else if(opt("calls")) {
                c_options.sortBy = calls;
            } else if(opt("ir")) {
                c_options.sortBy = ir;
            } else {
                error("invalid argument after option `-sort`");
            }
        }
#endif
        else if(opt("-maxlmt") || opt("-mem")){
            if(i+1 >= argc)
                error("maximum memory limit required after option `" + string(argv[i]) + "`");
            else {
                GarbageCollector::setMemoryLimit(getMemBytes(argv[i+1], argv[i]));
                i++;
            }
        }
        else if(opt("-stack")){
            if(i+1 >= argc)
                error("maximum stack limit required after option `-stack`");
            else {
                size_t sz = getMemBytes(argv[i+1], argv[i]);
                i++;

                if(Thread::validStackSize(sz)) {
                    threadStackSize = sz;
                } else {
                    stringstream ss;
                    ss << "default stack size must be greater than " << STACK_MIN << " bytes \n";
                    error(ss.str());
                }
            }
        }
        else if(opt("-istack")){
            if(i+1 >= argc)
                error("internal stack limit required after option `-istack`");
            else {
                size_t memoryLimit = getMemBytes(argv[i+1], argv[i]);
                size_t stackSize = memoryLimit / sizeof(StackElement);
                i++;

                if(Thread::validInternalStackSize(memoryLimit)) {
                    internalStackSize = stackSize;
                } else {
                    stringstream ss;
                    ss << "default internal stack size must be at least " << INTERNAL_STACK_MIN << " bytes \n";
                    error(ss.str());
                }
            }
        }
        else if(string(argv[i]).at(0) == '-'){
            error("invalid option `" + string(argv[i]) + "`, try infinity -h");
        }
        else {
            executable = argv[i++];
            while(i < argc) {
                appArgs.emplace_back(argv[i++]);
            }
            break;
        }
    }

    if(executable.empty()){
        help();
        return 1;
    }

    if(!File::exists(executable.c_str())){
        error("file `" + executable + "` doesnt exist!");
    }

    return startApplication(executable, appArgs);
}

uInt getMemBytes(const char *str, string option) {
    string size = string(str);
    stringstream ss;
    bool parsedDigit = false;

    for(unsigned int i = 0; i < size.size(); i++) {
        if(isdigit(size.at(i))) {
            parsedDigit = true;
            ss << size.at(i);
        } else if(isalpha(size.at(i))) {
            string num = ss.str(); // 1028M
            unsigned long limit = strtoul(ss.str().c_str(), NULL, 0);
            switch(size.at(i)) {
                case 'k':
                case 'K':
                    return KB_TO_BYTES(limit);
                case 'm':
                case 'M':
                    return MB_TO_BYTES(limit);
                case 'G':
                case 'g':
                    return GB_TO_BYTES(limit);
                default:
                    error("expected postfix 'K', 'M', or 'G' after number with option `" + option + "`");
                    break;
            }
        } else {
            if(parsedDigit)
                error("expected postfix 'K', 'M', or 'G' after number with option `" + option + "`");
            else
                error("expected number option `" + option + "`");
        }
    }
    return 0;
}

int startApplication(string &exe, std::list<string>& appArgs) {
    int result;
    if((result = CreateVirtualMachine(exe)) != 0) {
        fprintf(stderr, "Could not start the infinity virtual machine. Failed with code: %d\n", result);
        goto bail;
    }

    pushArgumentsToStack(appArgs);
    Thread::start(main_threadid, 0);

    Thread *main;
    Thread::threads.get(main_threadid, main);
    Thread::threadjoin(main);

    result=vm.exitVal;
    return result;

    bail:
    vm.destroy();
    return result;
}

void pushArgumentsToStack(std::list<string>& appArgs) {
    Thread *main;
    Thread::threads.get(main_threadid, main);
    pushArgumentsToStack(&(++main->sp)->object, appArgs);
}

void pushArgumentsToStack(Object *object, std::list<string> &appArgs) {
    const short MIN_ARGS = 5;
    Int size = MIN_ARGS+appArgs.size();
    Int iter=0;

    stringstream ss;
    ss << vm.manifest.target;
    native_string str(ss.str());

    *object = gc.newObjectArray(size);

    gc.createStringArray(&object->object->node[iter++],vm.manifest.application);
    gc.createStringArray(&object->object->node[iter++],vm.manifest.version);
    gc.createStringArray(&object->object->node[iter++], str); /* target platform also the platform version */

#ifdef WIN32_
    str.set("win");
#endif
#ifdef POSIX_
    str.set("posix");
#endif
    gc.createStringArray(&object->object->node[iter++], str); /* operating system currently running on */
    infinityObject *mainThread = gc.newObject(vm.ThreadClass);
    object->object->node[iter++] = mainThread;

    vm.setFieldVar("native_handle", mainThread, 0, 0);
    vm.setFieldClass("name", mainThread, vm.StringClass);

    Object* field;
    if((field = vm.resolveField("name", mainThread)) != NULL && field->object) {
        str.set("main");
        gc.createStringArray(vm.resolveField("data", field->object), str);
    }

    vm.setFieldVar("started", mainThread, 0, 1);
    vm.setFieldVar("stack_size", mainThread, 0, threadStackSize);

    /*
     * Assign program args to be passed to main
     */
    for(unsigned int i = 0; i < appArgs.size(); i++) {
        runtime::String argStr(*std::next(appArgs.begin(), i));
        gc.createStringArray(&object->object->node[iter++],  argStr);
    }

    appArgs.clear();
}

