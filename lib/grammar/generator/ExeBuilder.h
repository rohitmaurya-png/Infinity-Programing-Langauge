//
// Created by Rohit Maurya on 5/15/2020.
//

#ifndef infinity_EXEBUILDER_H
#define infinity_EXEBUILDER_H

class Compiler;
class ClassObject;
class Method;
class ExeBuilder {
public:
    ExeBuilder(Compiler *compiler)
    :
        compiler(compiler)
    {
        allMethods.init();
        allClasses.init();
        functionPointers.init();
    }

    ~ExeBuilder() {
        allMethods.free();
        allClasses.free();
        functionPointers.free();
    }

    void build();

private:
    List<Method*> allMethods;
    List<ClassObject*> allClasses;
    List<Method*> functionPointers;
    Compiler *compiler;
    stringstream buf;
    stringstream dataSec;

    void createDumpFile();
    void createNativeSourceCode();
    void createNativeHeaderFile();
    void createNativeSourceFile();
    void createinfinityMappingSourceFile();
    void appendinfinityMappingSourceFile(ClassObject*, stringstream&);
    void appendClassHeaderFunctions(ClassObject*, stringstream&);
    void createProcAddrFunc(stringstream&);
    void appendProcAddrFunctions(ClassObject*, stringstream&, bool&);
    void createMainFunc(stringstream&);
    void appendMainFunctions(ClassObject*, stringstream&);
    void createCallFunc(stringstream&);
    void appendCallFunctions(ClassObject*, stringstream&);
    void buildExe();
    void dumpClassInfo(ClassObject*);
    void addClass(ClassObject*);
    string registerToString(int64_t r);
    string codeToString(Method*);
    string getNote(Meta &meta);

    void buildHeader();

    void buildManifest();

    void buildSymbolSection();

    void buildFieldData(Field *field);
    void buildFieldData(ClassObject *pObject);
    void buildMethodData(ClassObject *pObject);
    void buildInterfaceData(ClassObject *pObject);

    void buildStringSection();
    void buildConstantSection();
    void addFunctionPointers();

    void buildDataSection();
    void putSymbol(Utype *utype, stringstream &buf);

    void putMethodData(Method *fun);

    void putMethodCode(Method *fun);

    void buildMetaDataSection();

    int32_t getFpOffset(Method *fun);

    int32_t getSpOffset(Method *fun);
    int32_t getSecondarySpOffset(Method *fun);
};


#endif //infinity_EXEBUILDER_H
