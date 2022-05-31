//
// Created by Peer De bakker on 5/31/22.
//

#ifndef NM_ERROR_CODES_H
#define NM_ERROR_CODES_H

typedef enum {
	SUCCESS = 0,
	NO_SYMBOLS,
	DIFFERENT_ENDIAN,
	INVALID_VERSION,
	INVALID_FILE,
	NO_MEMORY
}	e_error;

#endif //NM_ERROR_CODES_H
