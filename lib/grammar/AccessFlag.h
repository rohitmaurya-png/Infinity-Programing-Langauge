//
//Created by Rohit Maurya 2/2/2020.
//

#ifndef infinity_ACCESSMODIFIER_H
#define infinity_ACCESSMODIFIER_H

enum AccessFlag
{
    /*
     * Class, Interface, Field, and Method access modifiers
     */
    PUBLIC=0x001,
    PRIVATE=0x002,
    PROTECTED=0x004,

    LOCAL= 0x008, /* Only the file created can access the object */

    /*
     * Field access modifiers
     */
    flg_CONST=0x010,
    STATIC=0x020, /* Methods can use this modifier */

    /*
     * Flags for classes only
     */
    STABLE = 0x40,
    UNSTABLE = 0x80,
    EXTENSION = 0x100,

    NATIVE = 0x200,

    flg_UNDEFINED=0x1000
};

#endif //infinity_ACCESSMODIFIER_H
