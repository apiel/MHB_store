
#ifndef __RELAY_H__
#define __RELAY_H__

#include "config.h"

class Store {
    public:
        Store();
        void stop();
        void open();
        void close();
        void toggle();

    protected:
        int _mode;
        void start(int mode);
};

extern Store store;

#endif
