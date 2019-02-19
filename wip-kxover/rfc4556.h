/*
 * asn2quickder output for KerberosV5-PK-INIT-SPEC -- automatically generated
 *
 * Read more about Quick `n' Easy DER on https://github.com/vanrein/quick-der
 *
 */


#ifndef QUICK_DER_rfc4556_H
#define QUICK_DER_rfc4556_H


#include <quick-der/api.h>


#include <quick-der/rfc3280.h>
#include <quick-der/rfc4120.h>


typedef DER_OVLY_rfc3280_SubjectPublicKeyInfo DER_OVLY_rfc4556_SubjectPublicKeyInfo;
typedef DER_OVLY_rfc3280_AlgorithmIdentifier DER_OVLY_rfc4556_AlgorithmIdentifier;
typedef DER_OVLY_rfc4120_KerberosTime DER_OVLY_rfc4556_KerberosTime;
typedef DER_OVLY_rfc4120_PrincipalName DER_OVLY_rfc4556_PrincipalName;
typedef DER_OVLY_rfc4120_Realm DER_OVLY_rfc4556_Realm;
typedef DER_OVLY_rfc4120_EncryptionKey DER_OVLY_rfc4556_EncryptionKey;
typedef DER_OVLY_rfc4120_Checksum DER_OVLY_rfc4556_Checksum;


#define DER_PIMP_rfc4556_SubjectPublicKeyInfo(implicit_tag) DER_PIMP_rfc3280_SubjectPublicKeyInfo(implicit_tag)

#define DER_PACK_rfc4556_SubjectPublicKeyInfo DER_PACK_rfc3280_SubjectPublicKeyInfo
#define DER_PIMP_rfc4556_AlgorithmIdentifier(implicit_tag) DER_PIMP_rfc3280_AlgorithmIdentifier(implicit_tag)

#define DER_PACK_rfc4556_AlgorithmIdentifier DER_PACK_rfc3280_AlgorithmIdentifier
#define DER_PIMP_rfc4556_KerberosTime(implicit_tag) DER_PIMP_rfc4120_KerberosTime(implicit_tag)

#define DER_PACK_rfc4556_KerberosTime DER_PACK_rfc4120_KerberosTime
#define DER_PIMP_rfc4556_PrincipalName(implicit_tag) DER_PIMP_rfc4120_PrincipalName(implicit_tag)

#define DER_PACK_rfc4556_PrincipalName DER_PACK_rfc4120_PrincipalName
#define DER_PIMP_rfc4556_Realm(implicit_tag) DER_PIMP_rfc4120_Realm(implicit_tag)

#define DER_PACK_rfc4556_Realm DER_PACK_rfc4120_Realm
#define DER_PIMP_rfc4556_EncryptionKey(implicit_tag) DER_PIMP_rfc4120_EncryptionKey(implicit_tag)

#define DER_PACK_rfc4556_EncryptionKey DER_PACK_rfc4120_EncryptionKey
#define DER_PIMP_rfc4556_Checksum(implicit_tag) DER_PIMP_rfc4120_Checksum(implicit_tag)

#define DER_PACK_rfc4556_Checksum DER_PACK_rfc4120_Checksum




/* Overlay structures with ASN.1 derived nesting and labelling */

typedef struct DER_OVLY_rfc4556_ExternalPrincipalIdentifier {
	dercursor subjectName; // [0] IMPLICIT OCTET STRING
	dercursor issuerAndSerialNumber; // [1] IMPLICIT OCTET STRING
	dercursor subjectKeyIdentifier; // [2] IMPLICIT OCTET STRING
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_ExternalPrincipalIdentifier;


typedef dernode DER_OVLY_rfc4556_AD_INITIAL_VERIFIED_CAS;

typedef DER_OVLY_rfc4556_ExternalPrincipalIdentifier DER_OVLY_rfc4556_AD_INITIAL_VERIFIED_CAS_0;


typedef struct DER_OVLY_rfc4556_PKAuthenticator {
	dercursor cusec; // [0] INTEGER (0..999999)
	DER_OVLY_rfc4556_KerberosTime ctime; // [1] KerberosTime
	dercursor nonce; // [2] INTEGER (0..4294967295)
	dercursor paChecksum; // [3] OCTET STRING
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_PKAuthenticator;


typedef dercursor DER_OVLY_rfc4556_DHNonce;


typedef struct DER_OVLY_rfc4556_AuthPack {
	DER_OVLY_rfc4556_PKAuthenticator pkAuthenticator; // [0] PKAuthenticator
	DER_OVLY_rfc4556_SubjectPublicKeyInfo clientPublicValue; // [1] SubjectPublicKeyInfo
	dernode supportedCMSTypes; // [2] SEQUENCE OF AlgorithmIdentifier
	DER_OVLY_rfc4556_DHNonce clientDHNonce; // [3] DHNonce
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_AuthPack;

typedef DER_OVLY_rfc4556_AlgorithmIdentifier DER_OVLY_rfc4556_AuthPack_supportedCMSTypes;


typedef struct DER_OVLY_rfc4556_DHRepInfo {
	dercursor dhSignedData; // [0] IMPLICIT OCTET STRING
	DER_OVLY_rfc4556_DHNonce serverDHNonce; // [1] DHNonce
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_DHRepInfo;


typedef struct DER_OVLY_rfc4556_KDCDHKeyInfo {
	dercursor subjectPublicKey; // [0] BIT STRING
	dercursor nonce; // [1] INTEGER (0..4294967295)
	DER_OVLY_rfc4556_KerberosTime dhKeyExpiration; // [2] KerberosTime
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_KDCDHKeyInfo;


typedef struct DER_OVLY_rfc4556_KRB5PrincipalName {
	DER_OVLY_rfc4556_Realm realm; // [0] Realm
	DER_OVLY_rfc4556_PrincipalName principalName; // [1] PrincipalName
} DER_OVLY_rfc4556_KRB5PrincipalName;


typedef struct DER_OVLY_rfc4556_PA_PK_AS_REP {
	DER_OVLY_rfc4556_DHRepInfo dhInfo; // [0] DHRepInfo
	dercursor encKeyPack; // [1] IMPLICIT OCTET STRING
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_PA_PK_AS_REP;


typedef struct DER_OVLY_rfc4556_PA_PK_AS_REQ {
	dercursor signedAuthPack; // [0] IMPLICIT OCTET STRING
	dernode trustedCertifiers; // [1] SEQUENCE OF ExternalPrincipalIdentifier
	dercursor kdcPkId; // [2] IMPLICIT OCTET STRING
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_PA_PK_AS_REQ;

typedef DER_OVLY_rfc4556_ExternalPrincipalIdentifier DER_OVLY_rfc4556_PA_PK_AS_REQ_trustedCertifiers;


typedef struct DER_OVLY_rfc4556_ReplyKeyPack {
	DER_OVLY_rfc4556_EncryptionKey replyKey; // [0] EncryptionKey
	DER_OVLY_rfc4556_Checksum asChecksum; // [1] Checksum
	/* ...ASN.1 extensions... */
} DER_OVLY_rfc4556_ReplyKeyPack;


typedef dernode DER_OVLY_rfc4556_TD_DH_PARAMETERS;

typedef DER_OVLY_rfc4556_AlgorithmIdentifier DER_OVLY_rfc4556_TD_DH_PARAMETERS_0;


typedef dernode DER_OVLY_rfc4556_TD_INVALID_CERTIFICATES;

typedef DER_OVLY_rfc4556_ExternalPrincipalIdentifier DER_OVLY_rfc4556_TD_INVALID_CERTIFICATES_0;


typedef dernode DER_OVLY_rfc4556_TD_TRUSTED_CERTIFIERS;

typedef DER_OVLY_rfc4556_ExternalPrincipalIdentifier DER_OVLY_rfc4556_TD_TRUSTED_CERTIFIERS_0;




/* Parser definitions in terms of ASN.1 derived bytecode instructions */

#define DER_PIMP_rfc4556_ExternalPrincipalIdentifier(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(0), \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(1), \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(2)/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_ExternalPrincipalIdentifier \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(0), \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(1), \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(2)/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_AD_INITIAL_VERIFIED_CAS(implicit_tag) \
	DER_PACK_STORE | implicit_tag

#define DER_PACK_rfc4556_AD_INITIAL_VERIFIED_CAS \
	DER_PACK_STORE | DER_TAG_SEQUENCE

#define DER_PIMP_rfc4556_PKAuthenticator(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_PKAuthenticator \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_KerberosTime, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_STORE | DER_TAG_OCTETSTRING, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_DHNonce(implicit_tag) \
	DER_PACK_STORE | implicit_tag

#define DER_PACK_rfc4556_DHNonce \
	DER_PACK_STORE | DER_TAG_OCTETSTRING

#define DER_PIMP_rfc4556_AuthPack(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_PKAuthenticator, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_SubjectPublicKeyInfo, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_rfc4556_DHNonce, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_AuthPack \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_PKAuthenticator, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_SubjectPublicKeyInfo, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(3), \
	DER_PACK_rfc4556_DHNonce, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_DHRepInfo(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_STORE | DER_TAG_CONTEXT(0), \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_DHNonce, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_DHRepInfo \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_STORE | DER_TAG_CONTEXT(0), \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_DHNonce, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_KDCDHKeyInfo(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_BITSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_rfc4556_KerberosTime, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_KDCDHKeyInfo \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_STORE | DER_TAG_BITSTRING, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_INTEGER, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(2), \
	DER_PACK_rfc4556_KerberosTime, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_KRB5PrincipalName(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_Realm, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_KRB5PrincipalName \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_Realm, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_PrincipalName, \
	DER_PACK_LEAVE, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_PA_PK_AS_REP(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_CHOICE_BEGIN, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_DHRepInfo, \
	DER_PACK_LEAVE, \
	DER_PACK_STORE | DER_TAG_CONTEXT(1)/* ...ASN.1 extensions... */, \
	DER_PACK_CHOICE_END, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_PA_PK_AS_REP \
	DER_PACK_CHOICE_BEGIN, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_DHRepInfo, \
	DER_PACK_LEAVE, \
	DER_PACK_STORE | DER_TAG_CONTEXT(1)/* ...ASN.1 extensions... */, \
	DER_PACK_CHOICE_END

#define DER_PIMP_rfc4556_PA_PK_AS_REQ(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_STORE | DER_TAG_CONTEXT(0), \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(2)/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_PA_PK_AS_REQ \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_STORE | DER_TAG_CONTEXT(0), \
	DER_PACK_OPTIONAL, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_STORE | DER_TAG_SEQUENCE, \
	DER_PACK_LEAVE, \
	DER_PACK_OPTIONAL, \
	DER_PACK_STORE | DER_TAG_CONTEXT(2)/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_ReplyKeyPack(implicit_tag) \
	DER_PACK_ENTER | implicit_tag, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_EncryptionKey, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_Checksum, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PACK_rfc4556_ReplyKeyPack \
	DER_PACK_ENTER | DER_TAG_SEQUENCE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(0), \
	DER_PACK_rfc4556_EncryptionKey, \
	DER_PACK_LEAVE, \
	DER_PACK_ENTER | DER_TAG_CONTEXT(1), \
	DER_PACK_rfc4556_Checksum, \
	DER_PACK_LEAVE/* ...ASN.1 extensions... */, \
	DER_PACK_LEAVE

#define DER_PIMP_rfc4556_TD_DH_PARAMETERS(implicit_tag) \
	DER_PACK_STORE | implicit_tag

#define DER_PACK_rfc4556_TD_DH_PARAMETERS \
	DER_PACK_STORE | DER_TAG_SEQUENCE

#define DER_PIMP_rfc4556_TD_INVALID_CERTIFICATES(implicit_tag) \
	DER_PACK_STORE | implicit_tag

#define DER_PACK_rfc4556_TD_INVALID_CERTIFICATES \
	DER_PACK_STORE | DER_TAG_SEQUENCE

#define DER_PIMP_rfc4556_TD_TRUSTED_CERTIFIERS(implicit_tag) \
	DER_PACK_STORE | implicit_tag

#define DER_PACK_rfc4556_TD_TRUSTED_CERTIFIERS \
	DER_PACK_STORE | DER_TAG_SEQUENCE



/* Recursive parser-sub definitions in support of SEQUENCE OF and SET OF */

#define DEFINE_DER_PSUB_rfc4556_AD_INITIAL_VERIFIED_CAS \
	const derwalk DER_PACK_rfc4556_AD_INITIAL_VERIFIED_CAS_0 [] = { \
		DER_PACK_rfc4556_ExternalPrincipalIdentifier, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_rfc4556_AD_INITIAL_VERIFIED_CAS_0 [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_rfc4556_AD_INITIAL_VERIFIED_CAS [] = { \
		{ 0, \
		  DER_ELEMSZ (rfc4556,AD_INITIAL_VERIFIED_CAS,0), \
		  DER_PACK_rfc4556_AD_INITIAL_VERIFIED_CAS_0, \
		  DER_PSUB_rfc4556_AD_INITIAL_VERIFIED_CAS_0 }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_rfc4556_AuthPack \
	const derwalk DER_PACK_rfc4556_AuthPack_supportedCMSTypes [] = { \
		DER_PACK_rfc4556_AlgorithmIdentifier, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_rfc4556_AuthPack_supportedCMSTypes [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_rfc4556_AuthPack [] = { \
		{ DER_OFFSET (rfc4556,AuthPack,supportedCMSTypes), \
		  DER_ELEMSZ (rfc4556,AuthPack,supportedCMSTypes), \
		  DER_PACK_rfc4556_AuthPack_supportedCMSTypes, \
		  DER_PSUB_rfc4556_AuthPack_supportedCMSTypes }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_rfc4556_KRB5PrincipalName \
	const struct der_subparser_action DER_PSUB_rfc4556_KRB5PrincipalName [] = { \
		{ DER_OFFSET (rfc4556,KRB5PrincipalName,principalName) \
		+ DER_OFFSET (rfc4120,PrincipalName,name_string), \
		  DER_ELEMSZ (rfc4120,PrincipalName,name_string), \
		  DER_PACK_rfc4120_PrincipalName_name_string, \
		  DER_PSUB_rfc4120_PrincipalName_name_string }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_rfc4556_PA_PK_AS_REQ \
	const derwalk DER_PACK_rfc4556_PA_PK_AS_REQ_trustedCertifiers [] = { \
		DER_PACK_rfc4556_ExternalPrincipalIdentifier, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_rfc4556_PA_PK_AS_REQ_trustedCertifiers [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_rfc4556_PA_PK_AS_REQ [] = { \
		{ DER_OFFSET (rfc4556,PA_PK_AS_REQ,trustedCertifiers), \
		  DER_ELEMSZ (rfc4556,PA_PK_AS_REQ,trustedCertifiers), \
		  DER_PACK_rfc4556_PA_PK_AS_REQ_trustedCertifiers, \
		  DER_PSUB_rfc4556_PA_PK_AS_REQ_trustedCertifiers }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_rfc4556_TD_DH_PARAMETERS \
	const derwalk DER_PACK_rfc4556_TD_DH_PARAMETERS_0 [] = { \
		DER_PACK_rfc4556_AlgorithmIdentifier, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_rfc4556_TD_DH_PARAMETERS_0 [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_rfc4556_TD_DH_PARAMETERS [] = { \
		{ 0, \
		  DER_ELEMSZ (rfc4556,TD_DH_PARAMETERS,0), \
		  DER_PACK_rfc4556_TD_DH_PARAMETERS_0, \
		  DER_PSUB_rfc4556_TD_DH_PARAMETERS_0 }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_rfc4556_TD_INVALID_CERTIFICATES \
	const derwalk DER_PACK_rfc4556_TD_INVALID_CERTIFICATES_0 [] = { \
		DER_PACK_rfc4556_ExternalPrincipalIdentifier, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_rfc4556_TD_INVALID_CERTIFICATES_0 [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_rfc4556_TD_INVALID_CERTIFICATES [] = { \
		{ 0, \
		  DER_ELEMSZ (rfc4556,TD_INVALID_CERTIFICATES,0), \
		  DER_PACK_rfc4556_TD_INVALID_CERTIFICATES_0, \
		  DER_PSUB_rfc4556_TD_INVALID_CERTIFICATES_0 }, \
		{ 0, 0, NULL, NULL } \
	};

#define DEFINE_DER_PSUB_rfc4556_TD_TRUSTED_CERTIFIERS \
	const derwalk DER_PACK_rfc4556_TD_TRUSTED_CERTIFIERS_0 [] = { \
		DER_PACK_rfc4556_ExternalPrincipalIdentifier, \
		DER_PACK_END }; \
	const struct der_subparser_action DER_PSUB_rfc4556_TD_TRUSTED_CERTIFIERS_0 [] = { \
		{ 0, 0, NULL, NULL } \
	}; \
	const struct der_subparser_action DER_PSUB_rfc4556_TD_TRUSTED_CERTIFIERS [] = { \
		{ 0, \
		  DER_ELEMSZ (rfc4556,TD_TRUSTED_CERTIFIERS,0), \
		  DER_PACK_rfc4556_TD_TRUSTED_CERTIFIERS_0, \
		  DER_PSUB_rfc4556_TD_TRUSTED_CERTIFIERS_0 }, \
		{ 0, 0, NULL, NULL } \
	};



#endif /* QUICK_DER_rfc4556_H */


/* asn2quickder output for KerberosV5-PK-INIT-SPEC ends here */
