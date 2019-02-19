/*
 * asn2quickder output for KXOVER -- automatically generated
 *
 * Read more about Quick `n' Easy DER on https://github.com/vanrein/quick-der
 *
 */


#ifndef QUICK_DER_KXOVER_H
#define QUICK_DER_KXOVER_H


#include <quick-der/api.h>


#include <quick-der/rfc4556.h>
#include <quick-der/rfc5280.h>
#include <quick-der/rfc4120.h>


typedef DER_OVLY_rfc4556_id_pkinit_san DER_OVLY_KXOVER_id_pkinit_san;
typedef DER_OVLY_rfc4556_KRB5PrincipalName DER_OVLY_KXOVER_KRB5PrincipalName;
typedef DER_OVLY_rfc4120_PrincipalName DER_OVLY_KXOVER_PrincipalName;
typedef DER_OVLY_rfc4120_Realm DER_OVLY_KXOVER_Realm;
typedef DER_OVLY_rfc4120_Authenticator DER_OVLY_KXOVER_Authenticator;
typedef DER_OVLY_rfc4120_KerberosTime DER_OVLY_KXOVER_KerberosTime;
typedef DER_OVLY_rfc4120_Int32 DER_OVLY_KXOVER_Int32;
typedef DER_OVLY_rfc4120_UInt32 DER_OVLY_KXOVER_UInt32;
typedef DER_OVLY_rfc5280_Certificate DER_OVLY_KXOVER_Certificate;
typedef DER_OVLY_rfc5280_AlgorithmIdentifier DER_OVLY_KXOVER_AlgorithmIdentifier;
typedef DER_OVLY_rfc5280_SubjectPublicKeyInfo DER_OVLY_KXOVER_SubjectPublicKeyInfo;
typedef DER_OVLY_rfc5280_AuthorityKeyIdentifier DER_OVLY_KXOVER_AuthorityKeyIdentifier;
typedef DER_OVLY_rfc5280_SubjectAltName DER_OVLY_KXOVER_SubjectAltName;
typedef DER_OVLY_rfc5280_OtherName DER_OVLY_KXOVER_OtherName;


#define DER_PIMP_KXOVER_id_pkinit_san(implicit_tag) DER_PIMP_rfc4556_id_pkinit_san(implicit_tag)

#define DER_PACK_KXOVER_id_pkinit_san DER_PACK_rfc4556_id_pkinit_san
#define DER_PIMP_KXOVER_KRB5PrincipalName(implicit_tag) DER_PIMP_rfc4556_KRB5PrincipalName(implicit_tag)

#define DER_PACK_KXOVER_KRB5PrincipalName DER_PACK_rfc4556_KRB5PrincipalName
#define DER_PIMP_KXOVER_PrincipalName(implicit_tag) DER_PIMP_rfc4120_PrincipalName(implicit_tag)

#define DER_PACK_KXOVER_PrincipalName DER_PACK_rfc4120_PrincipalName
#define DER_PIMP_KXOVER_Realm(implicit_tag) DER_PIMP_rfc4120_Realm(implicit_tag)

#define DER_PACK_KXOVER_Realm DER_PACK_rfc4120_Realm
#define DER_PIMP_KXOVER_Authenticator(implicit_tag) DER_PIMP_rfc4120_Authenticator(implicit_tag)

#define DER_PACK_KXOVER_Authenticator DER_PACK_rfc4120_Authenticator
#define DER_PIMP_KXOVER_KerberosTime(implicit_tag) DER_PIMP_rfc4120_KerberosTime(implicit_tag)

#define DER_PACK_KXOVER_KerberosTime DER_PACK_rfc4120_KerberosTime
#define DER_PIMP_KXOVER_Int32(implicit_tag) DER_PIMP_rfc4120_Int32(implicit_tag)

#define DER_PACK_KXOVER_Int32 DER_PACK_rfc4120_Int32
#define DER_PIMP_KXOVER_UInt32(implicit_tag) DER_PIMP_rfc4120_UInt32(implicit_tag)

#define DER_PACK_KXOVER_UInt32 DER_PACK_rfc4120_UInt32
#define DER_PIMP_KXOVER_Certificate(implicit_tag) DER_PIMP_rfc5280_Certificate(implicit_tag)

#define DER_PACK_KXOVER_Certificate DER_PACK_rfc5280_Certificate
#define DER_PIMP_KXOVER_AlgorithmIdentifier(implicit_tag) DER_PIMP_rfc5280_AlgorithmIdentifier(implicit_tag)

#define DER_PACK_KXOVER_AlgorithmIdentifier DER_PACK_rfc5280_AlgorithmIdentifier
#define DER_PIMP_KXOVER_SubjectPublicKeyInfo(implicit_tag) DER_PIMP_rfc5280_SubjectPublicKeyInfo(implicit_tag)

#define DER_PACK_KXOVER_SubjectPublicKeyInfo DER_PACK_rfc5280_SubjectPublicKeyInfo
#define DER_PIMP_KXOVER_AuthorityKeyIdentifier(implicit_tag) DER_PIMP_rfc5280_AuthorityKeyIdentifier(implicit_tag)

#define DER_PACK_KXOVER_AuthorityKeyIdentifier DER_PACK_rfc5280_AuthorityKeyIdentifier
#define DER_PIMP_KXOVER_SubjectAltName(implicit_tag) DER_PIMP_rfc5280_SubjectAltName(implicit_tag)

#define DER_PACK_KXOVER_SubjectAltName DER_PACK_rfc5280_SubjectAltName
#define DER_PIMP_KXOVER_OtherName(implicit_tag) DER_PIMP_rfc5280_OtherName(implicit_tag)

#define DER_PACK_KXOVER_OtherName DER_PACK_rfc5280_OtherName




/* Overlay structures with ASN.1 derived nesting and labelling */

typedef DER_OVLY_KXOVER_Int32 DER_OVLY_KXOVER_EncryptionType;


typedef dernode DER_OVLY_KXOVER_KRB5RealmSet;

typedef DER_OVLY_KXOVER_Realm DER_OVLY_KXOVER_KRB5RealmSet_0;


typedef DER_OVLY_KXOVER_UInt32 DER_OVLY_KXOVER_KeyVersionNumber;


typedef struct DER_OVLY_KXOVER_KX_OFFER_EXTENSION {
	dercursor oid; // [0] OBJECT IDENTIFIER
	dercursor critical; // [1] BOOLEAN
	dercursor value; // [2] OCTET STRING
} DER_OVLY_KXOVER_KX_OFFER_EXTENSION;


typedef struct DER_OVLY_KXOVER_KX_OFFER {
	DER_OVLY_KXOVER_KerberosTime request_time; // [0] KerberosTime
	dercursor salt; // [1] OCTET STRING
	DER_OVLY_KXOVER_KRB5PrincipalName kx_name; // [2] KRB5PrincipalName
	DER_OVLY_KXOVER_KeyVersionNumber kvno; // [3] KeyVersionNumber
	dernode etypes; // [4] SEQUENCE OF EncryptionType
	DER_OVLY_KXOVER_KerberosTime from; // [5] KerberosTime
	DER_OVLY_KXOVER_KerberosTime till; // [6] KerberosTime
	DER_OVLY_KXOVER_KRB5PrincipalName my_name; // [7] KRB5PrincipalName
	dernode extensions; // [8] SEQUENCE OF KX-OFFER-EXTENSION
} DER_OVLY_KXOVER_KX_OFFER;

typedef DER_OVLY_KXOVER_EncryptionType DER_OVLY_KXOVER_KX_OFFER_etypes;

typedef DER_OVLY_KXOVER_KX_OFFER_EXTENSION DER_OVLY_KXOVER_KX_OFFER_extensions;


typedef struct DER_OVLY_KXOVER_KX_REP_MSG {
	dercursor pvno; // [0] INTEGER (5)
	dercursor msg_type; // [1] INTEGER (19)
	DER_OVLY_KXOVER_KX_OFFER offer; // [2] KX-OFFER
	/* ...ASN.1 extensions... */
} DER_OVLY_KXOVER_KX_REP_MSG;


typedef DER_OVLY_KXOVER_KX_REP_MSG DER_OVLY_KXOVER_KX_REP;


typedef struct DER_OVLY_KXOVER_KX_REQ_MSG {
	dercursor pvno; // [0] INTEGER (5)
	dercursor msg_type; // [1] INTEGER (18)
	DER_OVLY_KXOVER_KX_OFFER offer; // [2] KX-OFFER
	/* ...ASN.1 extensions... */
} DER_OVLY_KXOVER_KX_REQ_MSG;


typedef DER_OVLY_KXOVER_KX_REQ_MSG DER_OVLY_KXOVER_KX_REQ;


typedef struct DER_OVLY_KXOVER_KXOVER_KEY_INFO {
	DER_OVLY_KXOVER_KRB5PrincipalName kx_name; // [0] KRB5PrincipalName
	DER_OVLY_KXOVER_KRB5PrincipalName req_name; // [1] KRB5PrincipalName
	DER_OVLY_KXOVER_KRB5PrincipalName rep_name; // [2] KRB5PrincipalName
	DER_OVLY_KXOVER_KerberosTime from; // [3] KerberosTime
	DER_OVLY_KXOVER_KerberosTime till; // [4] KerberosTime
	DER_OVLY_KXOVER_KeyVersionNumber kvno; // [5] KeyVersionNumber
	DER_OVLY_KXOVER_EncryptionType etype; // [6] EncryptionType
	dercursor req_salt; // [7] OCTET STRING
	dercursor rep_salt; // [8] OCTET STRING
	dernode extension_info; // [9] SEQUENCE OF KX-OFFER-EXTENSION
} DER_OVLY_KXOVER_KXOVER_KEY_INFO;

typedef DER_OVLY_KXOVER_KX_OFFER_EXTENSION DER_OVLY_KXOVER_KXOVER_KEY_INFO_extension_info;




/* Parser definitions in terms of ASN.1 derived bytecode instructions */

#define DER_PIMP_KXOVER_EncryptionType(implicit_tag) \
	DER_PIMP_KXOVER_Int32(implicit_tag)

#define DER_PACK_KXOVER_EncryptionType \
	DER_PACK_KXOVER_Int32

#define DER_PIMP_KXOVER_KRB5RealmSet(implicit_tag) \
	DER_PACK_STORE | implicit_tag

#define DER_PACK_KXOVER_KRB5RealmSet \
	DER_PACK_STORE | DER_TAG_SET

#define DER_PIMP_KXOVER_KeyVersionNumber(implicit_tag) \
	DER_PIMP_KXOVER_UInt32(implicit_tag)

#define DER_PACK_KXOVER_KeyVersionNumber \
	DER_PACK_KXOVER_UInt32

#define DER_PIMP_KXOVER_KX_OFFER_EXTENSION(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_OBJECTIDENTIFIER, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_BOOLEAN, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PACK_KXOVER_KX_OFFER_EXTENSION \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_OBJECTIDENTIFIER, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_BOOLEAN, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PIMP_KXOVER_KX_OFFER(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_KXOVER_KeyVersionNumber, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(4), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(5), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(6), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(7), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(8), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PACK_KXOVER_KX_OFFER \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_KXOVER_KeyVersionNumber, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(4), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(5), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(6), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(7), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(8), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PIMP_KXOVER_KX_REP_MSG(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KX_OFFER, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_KXOVER_KX_REP_MSG \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KX_OFFER, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_KXOVER_KX_REP(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_APPLICATION(19), \
	DER_PACK_KXOVER_KX_REP_MSG, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PACK_KXOVER_KX_REP \
	DER_PACK_ENTER | DER_TAG_APPLICATION(19), \
	DER_PACK_KXOVER_KX_REP_MSG, \
	DER_PACK_LEAVE

#define DER_PIMP_KXOVER_KX_REQ_MSG(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KX_OFFER, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_KXOVER_KX_REQ_MSG \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KX_OFFER, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_KXOVER_KX_REQ(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_APPLICATION(18), \
	DER_PACK_KXOVER_KX_REQ_MSG, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PACK_KXOVER_KX_REQ \
	DER_PACK_ENTER | DER_TAG_APPLICATION(18), \
	DER_PACK_KXOVER_KX_REQ_MSG, \
	DER_PACK_LEAVE

#define DER_PIMP_KXOVER_KXOVER_KEY_INFO(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(4), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(5), \
	DER_PACK_KXOVER_KeyVersionNumber, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(6), \
	DER_PACK_KXOVER_EncryptionType, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(7), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(8), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(9), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PACK_KXOVER_KXOVER_KEY_INFO \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_KXOVER_KRB5PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(4), \
	DER_PACK_KXOVER_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(5), \
	DER_PACK_KXOVER_KeyVersionNumber, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(6), \
	DER_PACK_KXOVER_EncryptionType, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(7), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(8), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(9), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE



/* Recursive parser-sub definitions in support of SEQUENCE OF and SET OF */

#define DEFINE_DER_PSUB_KXOVER_KRB5RealmSet \
	const derwalk DER_PACK_KXOVER_KRB5RealmSet_0 [] = { \
		DER_PACK_KXOVER_Realm, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_KXOVER_KRB5RealmSet_0 [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_KXOVER_KRB5RealmSet [] = { \
		{ 0, \
		  DER_ELEMSZ (KXOVER,KRB5RealmSet,0), \
		  DER_PACK_KXOVER_KRB5RealmSet_0, \
		  DER_PSUB_KXOVER_KRB5RealmSet_0 }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_KXOVER_KX_OFFER \
	const derwalk DER_PACK_KXOVER_KX_OFFER_etypes [] = { \
		DER_PACK_KXOVER_EncryptionType, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_KXOVER_KX_OFFER_etypes [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const derwalk DER_PACK_KXOVER_KX_OFFER_extensions [] = { \
		DER_PACK_KXOVER_KX_OFFER_EXTENSION, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_KXOVER_KX_OFFER_extensions [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_KXOVER_KX_OFFER [] = { \
		{ DER_OFFSET (KXOVER,KX_OFFER,kx_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KX_OFFER,etypes), \
		  DER_ELEMSZ (KXOVER,KX_OFFER,etypes), \
		  DER_PACK_KXOVER_KX_OFFER_etypes, \
		  DER_PSUB_KXOVER_KX_OFFER_etypes }, \
		{ DER_OFFSET (KXOVER,KX_OFFER,my_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KX_OFFER,extensions), \
		  DER_ELEMSZ (KXOVER,KX_OFFER,extensions), \
		  DER_PACK_KXOVER_KX_OFFER_extensions, \
		  DER_PSUB_KXOVER_KX_OFFER_extensions }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_KXOVER_KX_REP_MSG \
	const struct der_subparser_action DER_PSUB_KXOVER_KX_REP_MSG [] = { \
		{ DER_OFFSET (KXOVER,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,kx_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,etypes), \
		  DER_ELEMSZ (kxover,KX_OFFER,etypes), \
		  DER_PACK_kxover_KX_OFFER_etypes, \
		  DER_PSUB_kxover_KX_OFFER_etypes }, \
		{ DER_OFFSET (KXOVER,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,my_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,extensions), \
		  DER_ELEMSZ (kxover,KX_OFFER,extensions), \
		  DER_PACK_kxover_KX_OFFER_extensions, \
		  DER_PSUB_kxover_KX_OFFER_extensions }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_KXOVER_KX_REP \
	const struct der_subparser_action DER_PSUB_KXOVER_KX_REP [] = { \
		{ DER_OFFSET (kxover,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,kx_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (kxover,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,etypes), \
		  DER_ELEMSZ (kxover,KX_OFFER,etypes), \
		  DER_PACK_kxover_KX_OFFER_etypes, \
		  DER_PSUB_kxover_KX_OFFER_etypes }, \
		{ DER_OFFSET (kxover,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,my_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (kxover,KX_REP_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,extensions), \
		  DER_ELEMSZ (kxover,KX_OFFER,extensions), \
		  DER_PACK_kxover_KX_OFFER_extensions, \
		  DER_PSUB_kxover_KX_OFFER_extensions }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_KXOVER_KX_REQ_MSG \
	const struct der_subparser_action DER_PSUB_KXOVER_KX_REQ_MSG [] = { \
		{ DER_OFFSET (KXOVER,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,kx_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,etypes), \
		  DER_ELEMSZ (kxover,KX_OFFER,etypes), \
		  DER_PACK_kxover_KX_OFFER_etypes, \
		  DER_PSUB_kxover_KX_OFFER_etypes }, \
		{ DER_OFFSET (KXOVER,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,my_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,extensions), \
		  DER_ELEMSZ (kxover,KX_OFFER,extensions), \
		  DER_PACK_kxover_KX_OFFER_extensions, \
		  DER_PSUB_kxover_KX_OFFER_extensions }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_KXOVER_KX_REQ \
	const struct der_subparser_action DER_PSUB_KXOVER_KX_REQ [] = { \
		{ DER_OFFSET (kxover,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,kx_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (kxover,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,etypes), \
		  DER_ELEMSZ (kxover,KX_OFFER,etypes), \
		  DER_PACK_kxover_KX_OFFER_etypes, \
		  DER_PSUB_kxover_KX_OFFER_etypes }, \
		{ DER_OFFSET (kxover,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,my_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (kxover,KX_REQ_MSG,offer) \
		+ DER_OFFSET (kxover,KX_OFFER,extensions), \
		  DER_ELEMSZ (kxover,KX_OFFER,extensions), \
		  DER_PACK_kxover_KX_OFFER_extensions, \
		  DER_PSUB_kxover_KX_OFFER_extensions }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_KXOVER_KXOVER_KEY_INFO \
	const derwalk DER_PACK_KXOVER_KXOVER_KEY_INFO_extension_info [] = { \
		DER_PACK_KXOVER_KX_OFFER_EXTENSION, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_KXOVER_KXOVER_KEY_INFO_extension_info [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_KXOVER_KXOVER_KEY_INFO [] = { \
		{ DER_OFFSET (KXOVER,KXOVER_KEY_INFO,kx_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KXOVER_KEY_INFO,req_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KXOVER_KEY_INFO,rep_name) \
		+ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ DER_OFFSET (KXOVER,KXOVER_KEY_INFO,extension_info), \
		  DER_ELEMSZ (KXOVER,KXOVER_KEY_INFO,extension_info), \
		  DER_PACK_KXOVER_KXOVER_KEY_INFO_extension_info, \
		  DER_PSUB_KXOVER_KXOVER_KEY_INFO_extension_info }, \
		{ 0, 0, NULL, NULL } \
	};



#endif /* QUICK_DER_KXOVER_H */


/* asn2quickder output for KXOVER ends here */
