//
//Created by Rohit Maurya 2/11/2020.
//

#include <random>
#include "GarbageCollector.h"
#include "../symbols/Object.h"
#include "../Thread.h"
#include "../symbols/Field.h"
#include "../../util/time.h"
#include "../../util/KeyPair.h"
#include "../Exe.h"
#include "../Manifest.h"
#include "../VirtualMachine.h"
#include <thread>
#include <algorithm>

uInt hbytes = 0;
GarbageCollector gc;

const Int MEMORY_POOL_SAMPLE_SIZE = 1000;
uInt memoryPoolResults[MEMORY_POOL_SAMPLE_SIZE];
Int samplesReceived =0;

void* __malloc(uInt bytes)
{
    void* ptr =nullptr;
    bool tried=false;
    alloc_bytes:
    if(gc.state >= RUNNING && !gc.spaceAvailable(bytes))
        goto lowmem;
    ptr=malloc(bytes);

    if(gc.state >= RUNNING && ptr == nullptr) {
        if(tried) {
            lowmem:
            throw Exception(vm.OutOfMemoryExcept, "out of memory");
        } else {
            tried=true;
            if(vm.state == VM_RUNNING && gc.state >= RUNNING) {
                gc.collect(GC_LOW);
                goto alloc_bytes;
            } else
                throw Exception(vm.OutOfMemoryExcept, "out of memory");
        }
    } else {
        return ptr;
    }
}
void* __calloc(uInt n, uInt bytes)
{
    void* ptr =nullptr;
    bool tried=false;
    alloc_bytes:
    if(gc.state >= RUNNING && !gc.spaceAvailable(n*bytes))
        goto lowmem;
    ptr=calloc(n, bytes);

    if(ptr == nullptr) {
        if(tried) {
            lowmem:
            throw Exception(vm.OutOfMemoryExcept, "out of memory");
        } else {
            tried=true;
            if(vm.state == VM_RUNNING && gc.state >= RUNNING) {
                gc.collect(GC_LOW);
                goto alloc_bytes;
            } else
                throw Exception(vm.OutOfMemoryExcept, "out of memory");
        }
    } else {
        return ptr;
    }
}
void* __realloc(void *ptr, uInt bytes, uInt old)
{
    void* rmap =nullptr;
    bool tried=false;
    alloc_bytes:
    if(gc.state >= RUNNING && !gc.spaceAvailable(bytes-old))
        goto lowmem;
    rmap=realloc(ptr, bytes);

    if(rmap == nullptr) {
        if(tried) {
            lowmem:
            throw Exception(vm.OutOfMemoryExcept, "out of memory");
        } else {
            tried=true;
            if(vm.state == VM_RUNNING && gc.state >= RUNNING) {
                gc.collect(GC_LOW);
                goto alloc_bytes;
            } else
                throw Exception(vm.OutOfMemoryExcept, "out of memory");
        }
    } else {
        return rmap;
    }
}

void GarbageCollector::initilize() {
    gc._Mheap = (infinityObject*)malloc(sizeof(infinityObject)); // HEAD
    if(gc._Mheap == NULL) throw Exception("out of memory");
    gc._Mheap->init(0, _stype_none);
    gc.tail = gc._Mheap; // set tail to self for later use
}

void GarbageCollector::releaseObject(Object *object) {
    if(object != nullptr && object->object != nullptr)
    {
        object->object->refCount--;
        object->object = nullptr;
    }
}

void GarbageCollector::shutdown() {
    if(gc.state != SHUTDOWN) {
        gc.state=SHUTDOWN;
#ifdef infinity_PROF_
        cout << "\nsize of object: " << sizeof(infinityObject) << endl;
        cout << "highest memory calculated: " << hbytes << endl;
        cout << "Objects Collected " << gc.x << endl;
        cout << "Total managed bytes left " << gc.managedBytes << endl;
        cout << "Objects left over young: " << gc.youngObjects << " adult: " << gc.adultObjects
             << " old: " << gc.oldObjects << endl;
        cout << "largest collection " << gc.largestCollectionTime << endl;
        cout << "total collections " << gc.collections << endl;
        cout << "total time spent collecting us: " << gc.timeSpentCollecting << endl;
        cout << "total time spent sleeping " << gc.timeSlept << endl;
        cout << "heap size: " << gc.heapSize << endl;
        cout << std::flush << endl;
#endif
        gc.locks.free();
    }
}

void GarbageCollector::startup() {
    auto* gcThread=(Thread*)malloc(
            sizeof(Thread)*1);
    gcThread->CreateDaemon("gc");
    Thread::startDaemon(
            GarbageCollector::threadStart, gcThread);
}


void GarbageCollector::collect(CollectionPolicy policy) {
    if(isShutdown())
        return;

    updateMemoryThreshold();

    if( policy == GC_LOW || policy == GC_EXPLICIT || policy == GC_CONCURRENT ) {
        Thread::suspendAllThreads(true);
        /**
         * In order to keep memory utilization low we must shutdown
         * the entire system to perform a colection, this will take
         * on average about 10us to complete
         */
        collectGarbage();
        if(tself->state != THREAD_KILLED)
           Thread::resumeAllThreads(true);
    }
}

/**
 * We want to update the allocation threshold as the program runs for
 * more efficent memory collection
 *
 */
void GarbageCollector::updateMemoryThreshold() {
    if (samplesReceived == MEMORY_POOL_SAMPLE_SIZE) {
        size_t total = 0, avg;
        for (long i = 0; i < MEMORY_POOL_SAMPLE_SIZE; i++) {
            total += memoryPoolResults[i];
        }

        avg = total / MEMORY_POOL_SAMPLE_SIZE;
        samplesReceived = 0;
        if((avg + (0.15 * memoryLimit)) < memoryLimit)
            memoryThreshold = avg + (0.15 * memoryLimit);
        else memoryThreshold = avg;
    } else {
        memoryPoolResults[samplesReceived++] = managedBytes;
    }
}

void GarbageCollector::collectGarbage() {
    mutex.lock();
    infinityObject *object = gc._Mheap->next, *prevObj = NULL, *end = tail;
#ifdef infinity_PROF_
    uInt past = Clock::realTimeInNSecs();
#endif
    mutex.unlock();

    while(object != NULL) {
        if(tself->state == THREAD_KILLED
           || object == end) {
            break;
        }

        if(GENERATION(object->info) <= gc_old) {
            // free object
            if(MARKED(object->info)) {
                if(object->refCount > 0) {
                    MARK(object->info, 0);
                } else {
                    object = sweep(object, prevObj);
                    continue;
                }
            }

            if(object->refCount <= 0){
                MARK(object->info, 1);
            } else {
                switch(GENERATION(object->info)) {
                    case gc_young:
                        youngObjects--;
                        adultObjects++;
                        SET_GENERATION(object->info, gc_adult);
                        break;
                    case gc_adult:
                        adultObjects--;
                        oldObjects++;
                        SET_GENERATION(object->info, gc_old);
                        break;
                }
            }
        }

        prevObj = object;
        object = object->next;
        if(object != NULL && object->next != NULL && (object->next->next == NULL || object->next->next == end))
            break;
    }

#ifdef infinity_PROF_
    uInt now = Clock::realTimeInNSecs();
    if(NANO_TOMICRO(now-past) > largestCollectionTime)
        largestCollectionTime = NANO_TOMICRO(now-past);
    timeSpentCollecting += NANO_TOMICRO(now-past);
    collections++;
#endif
}

void GarbageCollector::run() {
#ifdef infinity_PROF_
    tself->tprof = new Profiler();
    tself->tprof->init(2);
#endif

    for(;;) {
        sig:
        if(hasSignal(tself->signal, tsig_suspend))
            Thread::suspendSelf();
        if(tself->state == THREAD_KILLED) {
            return;
        }

        message:
        if(!messageQueue.empty()) {
            CollectionPolicy policy;
            {
                GUARD(mutex);
                policy = messageQueue.last();
                messageQueue.pop_back();
            }

            /**
             * We only want to run a concurrent collection
             * in the GC thread itsself
             */
            if(policy != GC_CONCURRENT)
                collect(policy);
        }

        do {

#ifdef infinity_PROF_
            if(managedBytes > hbytes)
                hbytes = managedBytes;
            timeSlept += 25;
#endif

            __os_yield();
#ifdef WIN32_
            Sleep(25);
#endif
#ifdef POSIX_
            usleep(25*999);
#endif
            if(state==SLEEPING) sedateSelf();
            if(!messageQueue.empty()) goto message;


            if(GC_COLLECT_MEM()) {
                /**
                 * Attempt to collect objects based on the appropriate
                 * conditions. This call does not guaruntee that any collections
                 * will happen
                 */
                collect(GC_CONCURRENT);
            }

            if(tself->signal)
                goto sig;

        } while(true);
    }
}

#ifdef WIN32_
DWORD WINAPI
#endif
#ifdef POSIX_
void*
#endif
GarbageCollector::threadStart(void *pVoid) {
    thread_self =(Thread*)pVoid;
    thread_self->state = THREAD_RUNNING;
    gc.tself = thread_self;
    gc.state = RUNNING;
    Thread::setPriority(thread_self, THREAD_PRIORITY_LOW);

    try {
        gc.run();
    } catch(Exception &e){
        /* Should never happen */
        sendSignal(thread_self->signal, tsig_except, 1);
    }

    /*
     * Check for uncaught exception in thread before exit
     */
    thread_self->exit();
#ifdef WIN32_
    return 0;
#endif
#ifdef POSIX_
    return nullptr;
#endif
}

void GarbageCollector::sendMessage(CollectionPolicy message) {
    GUARD(mutex);
    messageQueue.push_back(message);
}

double GarbageCollector::_sizeof(infinityObject *object) {
    double size =0;
    if(object != nullptr) {

        if(TYPE(object->info) == _stype_var) {
            size += sizeof(double)*object->size;
        } else if(TYPE(object->info) == _stype_struct) {
            for(uInt i = 0; i < object->size; i++) {
                infinityObject *o = object->node[i].object;

                /**
                 * If the object still has references we just drop it and move on
                 */
                if(o != nullptr) {
                    size += _sizeof(o);
                }
            }

            size += sizeof(Object)*object->size;
        }

        size += sizeof(infinityObject);
    }

    return size;
}

infinityObject* GarbageCollector::sweep(infinityObject *object, infinityObject *prevObj) {
    if(object != nullptr) {
        if(TYPE(object->info) == _stype_var) {
            managedBytes -= sizeof(double)*object->size;
            std::free(object->HEAD); object->HEAD = NULL;
        } else if(TYPE(object->info) == _stype_struct) {
            for(unsigned long i = 0; i < object->size; i++) {
                infinityObject *o = object->node[i].object;

                /**
                 * If the object still has references we just drop it and move on
                 */
                DEC_REF(o);
                if(o != NULL && o->refCount <= 0) MARK(o->info, 1);
            }

            managedBytes -= sizeof(Object)*object->size;
            std::free(object->node); object->node = NULL;
        }

        UPDATE_GC(object)

#ifdef infinity_PROF_
        x++;
#endif

        heapSize--;
        managedBytes -= sizeof(infinityObject);
        infinityObject* nextObj = object->next;

        GUARD(mutex)
        if(HAS_LOCK(object->info))
            dropLock(object);

        prevObj->next = object->next;
        std::free(object);
        return nextObj;
    }
    return NULL;
}

infinityObject *GarbageCollector::newObject(int64_t size, unsigned short ntype) {
    if(size<=0)
        return nullptr;

    infinityObject *object = (infinityObject*)__malloc(sizeof(infinityObject));
    object->init(size, _stype_var);

    object->HEAD = (double*)__calloc(size, sizeof(double));
    object->ntype = ntype % 9;

    /* track the allocation amount */
    GUARD(mutex);
    managedBytes += (sizeof(infinityObject)*1)+(sizeof(double)*size);
    PUSH(object);
    youngObjects++;

    return object;
}

infinityObject *GarbageCollector::newObjectUnsafe(int64_t size, unsigned short ntype) {
    if(size<=0)
        return nullptr;

    infinityObject *object = (infinityObject*)malloc(sizeof(infinityObject));
    if(object != NULL) {
        object->init(size, _stype_var);

        object->HEAD = (double *) calloc(size, sizeof(double));

        if(object->HEAD != NULL) {
            object->ntype = ntype % 9;
            SET_TYPE(object->info, _stype_var);

            /* track the allocation amount */
            GUARD(mutex);
            managedBytes += (sizeof(infinityObject) * 1) + (sizeof(double) * size);
            PUSH(object);
            youngObjects++;
        } else {
            std::free(object);
            return NULL;
        }
    }

    return object;
}

infinityObject *GarbageCollector::newObject(ClassObject *k, bool staticInit) {
    if(k != nullptr) {
        infinityObject *object = (infinityObject*)__malloc(sizeof(infinityObject));
        uint32_t size = staticInit ? k->staticFields : k->instanceFields;

        object->init(size, k);
        if(size > 0) {
            object->node = (Object*)__calloc(size, sizeof(struct Object));
            uInt fieldAddress =  staticInit ? k->instanceFields : 0;

            for(unsigned int i = 0; i < object->size; i++) {
                /**
                 * We want to set the class variables and arrays
                 * to null and initialize the var variables
                 */
                if(k->fields[fieldAddress].type <= VAR && !k->fields[fieldAddress].isArray) {
                    if(!staticInit || (staticInit && IS_STATIC(k->fields[fieldAddress].flags))) {
                        object->node[i] = newObject(1, k->type < FNPTR ? k->type : NTYPE_VAR);
                    }
                }

                fieldAddress++;
            }

        }

        GUARD(mutex);
        managedBytes += (sizeof(infinityObject)*1)+(sizeof(Object)*size);
        PUSH(object);
        youngObjects++;
        return object;
    }

    return nullptr;
}

infinityObject *GarbageCollector::newObjectUnsafe(ClassObject *k, bool staticInit) {
    if(k != nullptr) {
        infinityObject *object = (infinityObject*)malloc(sizeof(infinityObject));
        if(object != NULL) {
            uint32_t size = staticInit ? k->staticFields : k->instanceFields;

            object->init(size, k);
            if (size > 0) {
                object->node = (Object *) calloc(size, sizeof(struct Object));
                if (object->node != NULL) {
                    uInt fieldAddress = staticInit ? k->instanceFields : 0;

                    for (unsigned int i = 0; i < object->size; i++) {
                        /**
                         * We want to set the class variables and arrays
                         * to null and initialize the var variables
                         */
                        if (k->fields[fieldAddress].type <= VAR && !k->fields[fieldAddress].isArray) {
                            if (!staticInit || (staticInit && IS_STATIC(k->fields[fieldAddress].flags))) {
                                object->node[i] = newObjectUnsafe(1, k->type < FNPTR ? k->type : NTYPE_VAR);
                            }
                        }

                        fieldAddress++;
                    }

                    GUARD(mutex);
                    managedBytes += (sizeof(infinityObject) * 1) + (sizeof(Object) * size);
                    PUSH(object);
                    youngObjects++;
                } else {
                    free(object);
                    return NULL;
                }

            }
            return object;
        }
    }

    return nullptr;
}

infinityObject *GarbageCollector::newObjectArray(int64_t size) {
    if(size<=0)
        return nullptr;

    infinityObject *object = (infinityObject*)__malloc(sizeof(infinityObject));

    object->init(size, _stype_struct);
    object->node = (Object*)__calloc(size, sizeof(struct Object)*1);

    /* track the allocation amount */
    GUARD(mutex);
    managedBytes += (sizeof(infinityObject)*1)+(sizeof(Object)*size);
    PUSH(object);
    youngObjects++;

    return object;
}

infinityObject *GarbageCollector::newObjectArray(int64_t size, ClassObject *k) {
    if(k != nullptr && size > 0) {

        infinityObject *object = (infinityObject*)__malloc(sizeof(infinityObject)*1);
        object->init(size, k);

        if(size > 0)
            object->node = (Object*)__calloc(size, sizeof(struct Object));

        /* track the allocation amount */
        GUARD(mutex);
        managedBytes += (sizeof(infinityObject)*1)+(sizeof(Object)*size);
        PUSH(object);
        youngObjects++;

        return object;
    }

    return nullptr;
}

void GarbageCollector::createStringArray(Object *object, runtime::String& str) {
    if(object != nullptr) {
        *object = newObject(str.len, _INT8);

        if(object->object != NULL) {
            double *array = object->object->HEAD;
            for (unsigned long i = 0; i < str.len; i++) {
                *array = str.chars[i];
                array++;
            }
        }
    }
}

uInt GarbageCollector::getMemoryLimit() {
    return memoryLimit;
}

uInt GarbageCollector::getManagedMemory() {
    return managedBytes;
}

void GarbageCollector::realloc(infinityObject *o, size_t sz) {
    if(o != NULL && TYPE(o->info) == _stype_var) {
        o->HEAD = (double*)__realloc(o->HEAD, sizeof(double)*sz, sizeof(double)*o->size);

        GUARD(mutex);
        if(sz < o->size)
            managedBytes -= (sizeof(double)*(o->size-sz));
        else
            managedBytes += (sizeof(double)*(sz-o->size));

        if(sz > o->size) {
            for(Int i = o->size; i < sz; i++) {
                o->HEAD[i] = 0;
            }
        }
        o->size = sz;
    }
}

void GarbageCollector::reallocObject(infinityObject *o, size_t sz) {
    if(o != NULL && TYPE(o->info) == _stype_struct && o->node != NULL) {
        if(sz < o->size) {
            for(size_t i = sz; i < o->size; i++) {
                if(o->node[i].object != nullptr) {
                    DEC_REF(o->node[i].object)
                }
            }
        }

        o->node = (Object*)__realloc(o->node, sizeof(Object)*sz, sizeof(Object)*o->size);
        GUARD(mutex);
        if(sz < o->size)
            managedBytes -= (sizeof(Object)*(o->size-sz));
        else
            managedBytes += (sizeof(Object)*(sz-o->size));


        if(sz > o->size) {
            for(Int i = o->size; i < sz; i++) {
                o->node[i].object = NULL;
            }
        }

        o->size = sz;
    }
}

void GarbageCollector::kill() {
    mutex.lock();
    if(tself->state == THREAD_RUNNING) {
        tself->state = THREAD_KILLED;
        sendSignal(tself->signal, tsig_kill, 1);
        Thread::waitForThreadExit(tself);
    }

    mutex.unlock();
}

void GarbageCollector::sedateSelf() {
    Thread* self = thread_self;
    gc.state = SLEEPING;
    tself->state = THREAD_SUSPENDED;
    while(gc.state == SLEEPING) {

#ifdef infinity_PROF_
        if(managedBytes > hbytes)
            hbytes = managedBytes;
#endif
        __os_yield();
#ifdef WIN32_
        Sleep(30);
#endif
#ifdef POSIX_
        usleep(30*999);
#endif
        if(tself->state != THREAD_RUNNING)
            break;
    }

    // we don't want to shoot ourselves in the foot
    if(tself->state == THREAD_SUSPENDED)
        tself->state = THREAD_RUNNING;
    gc.state = RUNNING;
}

bool isLocker(void *o, mutex_t* mut) {
    return (infinityObject*)o == mut->object;
}

void GarbageCollector::printClassRefStatus() {
    gc.mutex.lock();
    infinityObject *object = _Mheap->next, *prevObj = NULL, *end = tail;
    while(object != NULL) {
        if(object == end) {
            break;
        }

        if(IS_CLASS(object->info)) {
            vm.classes[CLASS(object->info)].gcRefs++;
        }
    }

    cout << "\n\n Class gc ref count:\n";
    for(Int i = 0; i < vm.manifest.classes; i++) {
        cout << vm.classes[i].fullName.str() << " : " << vm.classes[i].gcRefs << endl;
    }
    cout << "\n\n";
    gc.mutex.unlock();

}

void GarbageCollector::lock(infinityObject *o, Thread* thread) {
    if(o) {
        mutex_t *mut;
        mutex.lock();
        long long idx = locks.indexof(isLocker, o);
        if(idx != -1)
            mut = locks.get(idx);
        else {
            managedBytes += sizeof(mutex_t)+sizeof(recursive_mutex);
            mut = new mutex_t(o, new recursive_mutex(), -1);
            locks.add(mut);
            SET_LOCK(o->info, 1);
        }
        mutex.unlock();
        mut->mutex->lock();

        if(mut->threadid != thread->id) {
            mut->threadid = thread->id;
        }
    }
}

void GarbageCollector::unlock(infinityObject *o, Thread* thread) {
    if(o) {
        mutex_t *mut=0;
        mutex.lock();
        long long idx = locks.indexof(isLocker, o);
        if(idx != -1)
            mut = locks.get(idx);
        mutex.unlock();
        if(mut) {
            if (mut->threadid == thread->id) {
                mut->threadid = -1;
            }

            mut->mutex->unlock();

        }
    }
}

void GarbageCollector::reconcileLocks(Thread* thread) {

    mutex.lock();
    for(long long i = 0; i < locks.size(); i++) {
        mutex_t *mut = locks.get(i);
        if(mut->threadid==thread->id) {
            mut->threadid = -1;
            mut->mutex->unlock();
        }
    }
    mutex.unlock();
}

void GarbageCollector::dropLock(infinityObject *o) {
    if(o) {
        mutex.lock();
        long long idx = locks.indexof(isLocker, o);
        if(idx != -1) {
            mutex_t *mut = locks.get(idx);
            if(mut->threadid!= -1)
                mut->mutex->unlock();
            managedBytes -= sizeof(mutex_t) + sizeof(recursive_mutex);
            delete mut->mutex;
            delete mut;
            locks.remove(idx);
        }
        mutex.unlock();
    }
}

void GarbageCollector::sedate() {
    mutex.lock();
    if(gc.state != SLEEPING && tself->state == THREAD_RUNNING) {
        gc.state = SLEEPING;
        Thread::suspendAndWait(Thread::getThread(gc_threadid));
    }
    mutex.unlock();
}

void GarbageCollector::wake() {
    mutex.lock();
    if(gc.state == SLEEPING) {
        gc.state = RUNNING;
        Thread::waitForThread(Thread::getThread(gc_threadid));
    }
    mutex.unlock();
}

int GarbageCollector::selfCollect() {
    if(gc.state == SLEEPING || tself->state == THREAD_KILLED) {
        collect(GC_EXPLICIT);
        return 0;
    }

    return -1;
}

bool GarbageCollector::isAwake() {
    return gc.state == RUNNING && tself->state == THREAD_RUNNING;
}
