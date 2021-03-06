//
// Created by Rohit Maurya on 5/27/2020.
//
#include "memory.h"
#include "../../runtime/Thread.h"
#include "../../runtime/VirtualMachine.h"

void copy() {
    Int end = (thread_self->sp--)->var;
    Int start = (thread_self->sp--)->var;
    Object *arry = &thread_self->sp->object;
    infinityObject *oldArray = arry->object;
    Int copyLen = end - start, index=0;

    if(oldArray != NULL) {
        if(copyLen < 0 || end > oldArray->size || end < 0 || start < 0 || start >= oldArray->size) {
            stringstream ss;
            ss << "invalid call to copy() start: " << start << ", end: " << end
               << ", size: " << oldArray->size;
            throw Exception(vm.InvalidOperationExcept, ss.str());
        }

        if(TYPE(oldArray->info) == _stype_var) { // var[]
            *arry = gc.newObject(copyLen, oldArray->ntype);

            INC_REF(oldArray)
            for (Int i = start; i < end; i++) {
                arry->object->HEAD[index++] = oldArray->HEAD[i];
            }
            DEC_REF(oldArray)
        } else if(TYPE(oldArray->info) == _stype_struct) { // object? maybe...
            if(oldArray->node != NULL) {
                if(IS_CLASS(oldArray->info)) {
                    *arry = gc.newObjectArray(copyLen, &vm.classes[CLASS(oldArray->info)]);
                } else
                    *arry = gc.newObjectArray(copyLen);

                INC_REF(oldArray)
                for (Int i = start; i < end; i++) {
                    arry->object->node[index++] = oldArray->node[i];
                }
                DEC_REF(oldArray)
            }
        } else
            throw Exception(vm.NullptrExcept, "");

    } else
        throw Exception(vm.NullptrExcept, "");
}

void memcopy() {
    Int srcEnd = (thread_self->sp--)->var;
    Int srcStart = (thread_self->sp--)->var;
    Int destStart = (thread_self->sp--)->var;
    Int copyLen = srcEnd - srcStart;
    infinityObject *coppiedArray = (thread_self->sp--)->object.object;
    infinityObject *arry = (thread_self->sp--)->object.object;

    if(coppiedArray != NULL && arry != NULL) {
        if(srcEnd > coppiedArray->size || srcEnd < 0 || srcStart < 0 || srcStart >= coppiedArray->size
            || copyLen < 0) {
            stringstream ss;
            ss << "invalid call to memcopy(); start: " << srcStart << ", end: " << srcEnd
               << ", size: " << coppiedArray->size;
            throw Exception(vm.InvalidOperationExcept, ss.str());
        }

        if(destStart >= arry->size || destStart < 0) {
            stringstream ss;
            ss << "invalid call to memcopy(); dest start: " << destStart
               << ", dest size: " << arry->size;
            throw Exception(vm.InvalidOperationExcept, ss.str());
        }

        if( (destStart + copyLen) > arry->size) {
            stringstream ss;
            ss << "invalid call to memcopy() array is not large enough to receive all data; copy size: "
                << (copyLen) << ", starting at: " << destStart << ", dest len: " << arry->size;
            throw Exception(vm.InvalidOperationExcept, ss.str());
        }

        if(TYPE(coppiedArray->info) == _stype_var) { // var[]
            for (Int i = srcStart; i < srcEnd; i++) {
                arry->HEAD[destStart++] = coppiedArray->HEAD[i];
            }
        } else if(TYPE(coppiedArray->info) == _stype_struct) { // object? maybe...
            if(coppiedArray->node != NULL) {
                for (Int i = srcStart; i < srcEnd; i++) {
                    arry->node[destStart++] = coppiedArray->node[i];
                }
            }
        } else
            throw Exception(vm.NullptrExcept, "");

    } else
        throw Exception(vm.NullptrExcept, "");
}

void invert() {
    Int len = (thread_self->sp--)->var;
    Int start = (thread_self->sp--)->var;
    Object *arry = &thread_self->sp->object;
    infinityObject *oldArray = arry->object;

    if(oldArray != NULL) {
        if((start+len) > oldArray->size || len < 0 || start < 0 || start >= oldArray->size) {
            stringstream ss;
            ss << "invalid call to invert() start: " << start << ", len: " << len
               << ", size: " << oldArray->size;
            throw Exception(vm.InvalidOperationExcept, ss.str());
        }

        if(oldArray->size <= 1) return;

        if(TYPE(oldArray->info) == _stype_var) { // var[]
            *arry = gc.newObject(len, oldArray->ntype);
            Int iter = 0;
            for (Int i = (start + len) - 1; i >= start; i--) {
                arry->object->HEAD[iter++] = oldArray->HEAD[i];
            }
        } else if(TYPE(oldArray->info) == _stype_struct) { // object? maybe...
            if(oldArray->node != NULL) {
                if(IS_CLASS(oldArray->info)) {
                    *arry = gc.newObjectArray(len, &vm.classes[CLASS(oldArray->info)]);
                } else
                    *arry = gc.newObjectArray(len);

                for (Int i = (start + len) - 1; i >= start; i--) {
                    arry->object->node[i] = oldArray->node[i];
                }
            }
        } else
            throw Exception(vm.NullptrExcept, "");

    } else
        throw Exception(vm.NullptrExcept, "");
}
