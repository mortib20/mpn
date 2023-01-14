#include "../include/mpn.h"

/* Initialize a new server session */
mpn_session mpn_server_new(void)
{
    return NULL;
}

/* Initialize a new client session */
mpn_session mpn_client_new(void)
{
    return NULL;
}

/* Closing the session */
void mpn_close(mpn_session session)
{

}

/* Free mpn_session object */
void mpn_free(mpn_session session)
{

}

/* Set Options of session */
int mpn_options_set(mpn_session session, enum mpn_options_e option, const void *value)
{
    return 0;
}