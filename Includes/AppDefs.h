
#ifndef SECOM_APP_DEFS_H
#define SECOM_APP_DEFS_H

enum HSType {
	HSTYPE_DUMMY  /**< default */ = 0
	, HSTYPE_NOT_CSR /**< HS which are not Capricorn HS */
	, HSTYPE_CSR_IMMEDIATE /**< The capricorn HS which are immediate devices */
	, HSTYPE_CSR_NESTED /**< The capricorn HS which are nested devices */
};

#endif