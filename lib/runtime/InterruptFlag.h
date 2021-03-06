//
// Created by Rohit Maurya on 4/4/2020.
//

#ifndef infinity_INTERRUPTFLAG_H
#define infinity_INTERRUPTFLAG_H

enum interruptFlag
{
               OP_NOP     = 0x0,
            OP_PRINTF     = 0x1,
           OP_OS_TIME     = 0x2,
       OP_GC_EXPLICIT     = 0x3,
            OP_GC_LOW     = 0x4,
        OP_GC_COLLECT     = 0x5,
          OP_GC_SLEEP     = 0x6,
         OP_GC_WAKEUP     = 0x7,
           OP_GC_KILL     = 0x8,
          OP_GC_STATE     = 0x9,
               OP_GUI     = 0xa,
          OP_NAN_TIME     = 0xb,
      OP_THREAD_START     = 0xc,
       OP_THREAD_JOIN     = 0xd,
  OP_THREAD_INTERRUPT     = 0xe,
     OP_THREAD_CREATE     = 0x10,
   OP_THREAD_PRIORITY     = 0x11,
    OP_THREAD_SUSPEND     = 0x12,
  OP_THREAD_UNSUSPEND     = 0x13,
  OP_THREAD_SUSPEND_FOR   = 0x14,
    OP_THREAD_CURRENT     = 0x15,
       OP_THREAD_ARGS     = 0x16,
  OP_THREAD_SET_CURRENT   = 0x17,
              OP_MATH     = 0x18,
        OP_RANDOM_INT     = 0x19,
        OP_SYSTEM_EXE     = 0x1a,
             OP_KBHIT     = 0x1b,
             OP_YIELD     = 0x1c,
              OP_EXIT     = 0x1d,
      OP_MEMORY_LIMIT     = 0x1e,
            OP_MEMORY     = 0x1f,
          OP_ABS_PATH     = 0x20,
          OP_MEM_COPY     = 0x21,
            OP_INVERT     = 0x22,
           OP_REALLOC     = 0x23,
       OP_FILE_ACCESS     = 0x24,
        OP_FILE_ATTRS     = 0x25,
    OP_FILE_UPDATE_TM     = 0x26,
         OP_FILE_SIZE     = 0x27,
       OP_FILE_CREATE     = 0x28,
       OP_FILE_DELETE     = 0x29,
         OP_GET_FILES     = 0x2a,
        OP_CREATE_DIR     = 0x2b,
        OP_DELETE_DIR     = 0x2c,
    OP_UPDATE_FILE_TM     = 0x2d,
             OP_CHMOD     = 0x2e,
         OP_READ_FILE     = 0x2f,
       OP_RENAME_FILE     = 0x30,
        OP_WRITE_FILE     = 0x31,
        OP_DISK_SPACE     = 0x32,
            OP_SIZEOF     = 0x33,
             OP_FLUSH     = 0x34,
    OP_GET_FRAME_INFO     = 0x35,
   OP_GET_STACK_TRACE     = 0x36,
              OP_COPY     = 0x37,
             OP_CORES     = 0x38,
      OP_LOAD_LIBRARY     = 0x39,
      OP_FREE_LIBRARY     = 0x3a,
         OP_LINK_FUNC     = 0x3b,
              OP_WAIT     = 0x3c,
            OP_NOTIFY     = 0x3d,
        OP_NOTIFY_FOR     = 0x3e,
 OP_RANDOM_INT_RANGED     = 0x3f,
      OP_RANDOM_DOUBLE     = 0x40,
 OP_RANDOM_DOUBLE_RANGED     = 0x41,
       OP_RANDOM_SEED     = 0x42,
            OP_STRTOD     = 0x43,
};

#endif //infinity_INTERRUPTFLAG_H
