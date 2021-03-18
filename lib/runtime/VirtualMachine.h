//
//Created by Rohit Maurya 2/15/2020.
//

#ifndef infinity_infinity_H
#define infinity_infinity_H

#include "../../stdimports.h"
#include "symbols/ClassObject.h"
#include "symbols/string.h"
#include "Thread.h"
#include "symbols/Method.h"
#ifdef WIN32_
#include "../Modules/std.os.gui/win/Gui.h"
#endif
#include "Manifest.h"
#include "Library.h"

struct infinityObject;

enum VMState {
    VM_CREATED = 0,
    VM_RUNNING = 1,
    VM_SHUTTING_DOWN = 2,
    VM_TERMINATED = 3
};

class VirtualMachine {
public:
    VirtualMachine()
    :
        state(VM_CREATED),
        exitVal(0),
#ifdef WIN32_
        gui(NULL),
#endif
        methods(NULL),
        strings(NULL),
        classes(NULL),
        staticHeap(NULL),
        manifest(),
        metaData(),
        Throwable(NULL),
        StackOverflowExcept(NULL),
        RuntimeExcept(NULL),
        ThreadStackExcept(NULL),
        IndexOutOfBoundsExcept(NULL),
        NullptrExcept(NULL),
        ClassCastExcept(NULL),
        libs(),
        mutex()
    {
    }
    recursive_mutex mutex;
    void destroy();

    static
#ifdef WIN32_
    DWORD WINAPI
#endif
#ifdef POSIX_
    void*
#endif
    InterpreterThreadStart(void *arg);
    Method* getMainMethod();
    ClassObject* resolveClass(std::string fullName);
    Object* resolveField(std::string name, infinityObject *classObject);
    void setFieldVar(std::string name, infinityObject *classObject, Int index, double value);
    void setFieldClass(std::string name, infinityObject *classObject, ClassObject* klass);
    string stringValue(infinityObject *);
    double numberValue(Int, infinityObject *);
    bool isStaticObject(infinityObject *object);

    void shutdown();
    static void sysInterrupt(int64_t i);
    bool catchException();
    static void fillStackTrace(infinityObject *frameInfo, infinityObject *stackTrace);
    static void fillStackTrace(native_string &str);
    static void fillStackTrace(Object *methods, Object *pcList, Object *data);
    static string getPrettyErrorLine(long line, long sourceFile);
    static void getFrameInfo(Object *frameInfo);
    static void getStackTrace();
    void locateBridgeAndCross(Method*);
    bool link(native_string&, native_string&);



    static void fillMethodCall(Method* func, Int pc, stringstream &ss);
    static void __snprintf(int cfmt, double val, int precision);
    double isType(Object *obj, int32_t type);

    Library* getLib(std::string name);
    int freeLib(std::string name);

    /**
     * Frequently used classes
     */
    ClassObject *Throwable;
    ClassObject *StackOverflowExcept;
    ClassObject *RuntimeExcept;
    ClassObject *ThreadStackExcept;
    ClassObject *IndexOutOfBoundsExcept;
    ClassObject *NullptrExcept;
    ClassObject *ClassCastExcept;
    ClassObject *OutOfMemoryExcept;
    ClassObject *InvalidOperationExcept;
    ClassObject *UnsatisfiedLinkExcept;
    ClassObject *IllStateExcept;
    ClassObject *StringClass;
    ClassObject *StackSate;
    ClassObject *ThreadClass;
    ClassObject *ExceptionClass;
    ClassObject *ErrorClass;

    Object outOfMemoryExcept;
    Meta metaData;
    Manifest manifest;
    Object* staticHeap;
    ClassObject* classes;
    runtime::String* strings;
    double *constants;
    Method* methods;
    Symbol* nativeSymbols;
    Method* funcPtrSymbols;
#ifdef WIN32_
    Gui* gui;
#endif
    _List<Library> libs;
    int exitVal;
    short state;
};

#define AUX_CLASSES 7

extern VirtualMachine vm;

int CreateVirtualMachine(string&);
fptr executeMethod(int64_t address, Thread* thread, bool inJit = false);
bool returnMethod(Thread* thread);
void invokeDelegate(int64_t address, int32_t args, Thread* thread, bool isStatic);
CXX11_INLINE
void setupMethodStack(int64_t address, Thread* thread, bool inJit);

#endif //infinity_infinity_H
