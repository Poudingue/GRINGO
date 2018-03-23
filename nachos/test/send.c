/* send.c
 *	Simple send program
 *
//  Copyright (c) 2018 Myself.
//  All rights reserved.
//  no claim for any copyright notice or limitation involved
//  liability and disclaimer of warranty provisions:
//  USE AT YOUR OWN RISK.
 */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int
main()
{
    n_printf("On va essayer d'envoyer la chaîne \"0123456789abcdefghijklmnopqrstuvwxyz\"\n");

    char * buff = "0123456789abcdefghijklmnopqrstuvwxyz"; //37 carac car 36 + '\0'
    //int count = TtySend(buff);
    int count = TtySend(buff);
    n_printf("fait avec %d caracteres envoyes\n", count);

    return 0;
}
