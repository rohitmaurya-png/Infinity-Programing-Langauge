//
//Created by Rohit Maurya 7/10/2020.
//

#ifndef infinity_TERMIOS_H
#define infinity_TERMIOS_H

#include "../../stdimports.h"

#ifdef POSIX_
extern char getch(void);

/* Read 1 character with echo */
extern char getche(void);

extern bool _kbhit(void);

void get_original_console_settings();

void reset_original_console_settings();
#endif

#endif //infinity_TERMIOS_H
