#include "../include/mpn.h"

mpn_error mpn_error_new(void)
{
    mpn_error error = calloc(1, sizeof(mpn_error));

    mpn_error_set(error, MPN_ERROR_OK);

    return error;
}

void mpn_error_free(mpn_error error)
{
    free(error);
}

/* set error message based on error type */
void mpn_error_set(mpn_error error, enum mpn_error_types_e type)
{
	error->type = type;

	/* get system error */
	char* system_error = strerror(MPN_ERRNO);

	/* set text depending on error type */
	switch (type) {
		case MPN_ERROR_OK:
			asprintf(&error->text, "MPN_ERROR_OK: Success");
			break;
			
		case MPN_ERROR_SOCKET:
			asprintf(&error->text, "MPN_ERROR_SOCKET: %s", system_error);
			break;

		case MPN_ERROR_BIND:
			asprintf(&error->text, "MPN_ERROR_BIND: %s", system_error);
			break;
		
		case MPN_ERROR_CONNECT:
			asprintf(&error->text, "MPN_ERROR_CONNECT: %s", system_error);
			break;
	}
}

/* returns the last error text*/
char* mpn_error_get(mpn_error error)
{
	return error->text;
}