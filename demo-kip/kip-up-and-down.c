/* KIP client -- a simple commandline utility for "kip up" and "kip down".
 *
 * This is a simple command to protect a file with "kip up", and to remove
 * it with "kip down".  It can be expanded in a variety of ways, including
 * with modules for upload and download.  Its main function however, is to
 * add and remove encryption.
 *
 * From: Rick van Rein <rick@openfortress.nl>
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <arpa/inet.h>

#include <arpa2/kip.h>

#include <krb5.h>

#include <errno.h>
#include <com_err.h>
#include <kiperr.h>



bool prefix_fwrite (uint8_t *blk, uint32_t blklen, FILE *outfile) {
	uint8_t lenbuf [4];
	* (uint32_t *) lenbuf = htonl( blklen );
	if (fwrite (lenbuf, 4, 1, outfile) != 1) {
		errno = EBADF;
		return false;
	}
	if (fwrite (blk, 1, blklen, outfile) != blklen) {
		errno = EBADF;
		return false;
	}
	return true;
}


bool prefix_fread (uint8_t *blk, uint32_t blklen, FILE *infile, uint32_t *readsz) {
	uint8_t lenbuf [4];
	if (fread (lenbuf, 4, 1, infile) != 1) {
		errno = 0;
		return false;
	}
	uint32_t actlen = ntohl( * (uint32_t *) lenbuf );
	if (actlen > blklen) {
		errno = EINVAL;
		return false;
	}
	if (actlen > 0) {
		if (fread (blk, 1, actlen, infile) != actlen) {
			errno = EPIPE;
			return false;
		}
	}
	*readsz = actlen;
	return true;
}


bool pump_up (char *progname, FILE *infile, FILE *outfile) {
	//
	// Open a context for KIP
	kipt_ctx kip;
	assert (kipctx_open (&kip));
	//
	// Create a session key
	kipt_keyid seskey;
	assert (kipkey_generate (kip, ENCTYPE_AES128_CTS_HMAC_SHA256_128, 13, &seskey));
	//
	// Load the master key into the first context
	uint8_t  keymud [2048];
	uint32_t keymudlen = 0;
	assert (kipkey_toservice (kip, seskey, sizeof (keymud), keymud, &keymudlen));
	if (!prefix_fwrite (keymud, keymudlen, outfile)) {
		return false;
	}
	//
	// Loop over the blocks of input, encrypt them and send them out
	while (true) {
		//
		// Read as much as possible from infile
		uint8_t plain [4000];
		int32_t plainlen;
		plainlen = fread (plain, 1, sizeof (plain), infile);
		if (plainlen > 0) {
			/* continue */
			;
		} else if (ferror (infile)) {
			perror ("Failed to read");
			goto close_fail;
		} else if (feof (infile)) {
			break;
		}
		//
		// Encrypt what we've just read
		uint8_t crypt [4200];
		uint32_t cryptlen;
		assert (kipdata_up (kip, seskey, plain, plainlen, crypt, sizeof (crypt), &cryptlen));
		prefix_fwrite (crypt, cryptlen, outfile);
	}
	//
	// Write 4 zero bytes, and another packet with the signature
	uint8_t sig [512];
	uint32_t siglen;
	if (kipsum_sign (kip, seskey, sizeof (sig), &siglen, sig)) {
		prefix_fwrite (sig, 0, outfile);
		prefix_fwrite (sig, siglen, outfile);
	}
	//
	// Close the KIP context
	kipctx_close (kip);
	//
	// Successful return
	return true;
	//
	// Return with error
close_fail:
	kipctx_close (kip);
fail:
	return false;
}


bool pump_down (char *progname, FILE *infile, FILE *outfile) {
	//
	// Open a context for KIP
	kipt_ctx kip;
	assert (kipctx_open (&kip));
	//
	// Use a shared buffer -- and load the mudkey into it
	uint8_t crypt [8092];
	uint32_t cryptlen;
	assert (prefix_fread (crypt, sizeof (crypt), infile, &cryptlen));
	//
	// Load the master key into the context
	kipt_keyid seskey;
	assert (kipkey_fromservice (kip, crypt, cryptlen, &seskey));
	//
	// Now loop to retrieve encrypted blocks, and decrypt those
	while (true) {
		if (!prefix_fread (crypt, sizeof (crypt), infile, &cryptlen)) {
			if ((errno == 0) && feof (infile)) {
				fprintf (stderr, "Preliminary end of file -- integrity not checked\n");
				goto close_fail;
			}
			perror ("Data failed to read");
			goto close_fail;
		}
		if (cryptlen == 0) {
			/* Looks like 4 zero bytes to introduce the signature */
			break;
		}
		uint8_t plain [sizeof (crypt)];
		uint32_t plainlen;
		assert (kipdata_down (kip, seskey, crypt, cryptlen, plain, sizeof (plain), &plainlen));
		if (fwrite (plain, 1, plainlen, outfile) != plainlen) {
			errno = EPIPE;
			goto close_fail;
		}
	}
	//
	// Expect to read the signature, and validate it
	uint8_t sig [512];
	uint32_t siglen;
	if (!prefix_fread (sig, sizeof (sig), infile, &siglen)) {
		fprintf (stderr, "Reading error in signature\n");
		goto close_fail;
	} else if (!kipsum_verify (kip, seskey, siglen, sig)) {
		fprintf (stderr, "Integrity check failed\n");
		goto close_fail;
	}
	//SILENT// fprintf (stderr, "Integrity check successful\n");
	//
	// Successful end
	kipctx_close (kip);
	return true;
	//
	// Unsuccessful end
close_fail:
	kipctx_close (kip);
fail:
	return false;
}


int main (int argc, char *argv []) {
	//
	// Commandline parsing
	char *progname = argv [0];
	//
	// Options after "kip" go here, upping argv, downing argc
	//
	// Distinguish "up" and "down" variants
	if (argc < 4) {
		fprintf (stderr, "Usage: %s ... up|down <in> <out> [<recipient>...]\n", progname);
		exit (1);
	}
	bool up   = (strcmp (argv [1], "up"  ) == 0);
	bool down = (strcmp (argv [1], "down") == 0);
	if (! ( up || down ) ) {
		fprintf (stderr, "Usage: %s ... up|down ...\n", progname);
		exit (1);
	}
	argc -= 1;
	argv += 1;
	//
	// Open the input and output files
	char * input = argv [1];
	char *output = argv [2];
	argv += 2;
	argc -= 2;
	FILE *infile = fopen (input, "r");
	if (infile == NULL) {
		perror ("Failed to open input file");
		exit (1);
	}
	FILE *outfile = fopen (output, "w");
	if (outfile == NULL) {
		perror ("Failed to open output file");
		exit (1);
	}
	//
	// Start pumping up or down, as requested
	bool ok;
	if (up) {
		ok = pump_up   (progname, infile, outfile);
	} else {
		ok = pump_down (progname, infile, outfile);
	}
	//
	// Close off
	fclose (infile);
	fclose (outfile);
	if (!ok) {
		com_err (progname, errno, "Failed during kip");
	}
	exit (ok ? 0 : 1);
}
