//
// Created by rohit  on 1/18/2020.
//

#ifndef infinity_KEYPAIR_H
#define infinity_KEYPAIR_H

template <class K, class T>
struct KeyPair
{
    KeyPair(K k, T v)
            :
            key(k),
            value(v)
    {
    }

    KeyPair()
    {
    }
    void set(K k, T val)
    {
        key = k;
        value = val;
    }

    void operator=(KeyPair<K, T> kp) {
        key = kp.key;
        value = kp.value;
    }

    K key;
    T value;
};

#endif //infinity_KEYPAIR_H
