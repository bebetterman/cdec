#pragma once

// -------------------------------------------------------------------------- //

enum CurlOption
{
	CCO_None = 0,
	CCO_ResponseHeaders	= 1,
};

enum CurlProxyType
{
	CPT_SOCKS4 = 0,
	CPT_SOCKS5 = 1,
};

enum CurlExceptionCode
{
	ECNS_CURL	= 0x101,

	EC_CURL_ST = MAKE_EX_CODE(ECNS_CURL, 0),
	EC_CURL_UNSUPPORTED_PROTOCOL,    /* 1 */
	EC_CURL_FAILED_INIT,             /* 2 */
	EC_CURL_URL_MALFORMAT,           /* 3 */
	EC_CURL_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
									7.17.0, reused in April 2011 for 7.21.5] */
	EC_CURL_COULDNT_RESOLVE_PROXY,   /* 5 */
	EC_CURL_COULDNT_RESOLVE_HOST,    /* 6 */
	EC_CURL_COULDNT_CONNECT,         /* 7 */
	EC_CURL_FTP_WEIRD_SERVER_REPLY,  /* 8 */
	EC_CURL_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
									due to lack of access - when login fails
									this is not returned. */
	EC_CURL_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
									7.15.4, reused in Dec 2011 for 7.24.0]*/
	EC_CURL_FTP_WEIRD_PASS_REPLY,    /* 11 */
	EC_CURL_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
									[was obsoleted in August 2007 for 7.17.0,
									reused in Dec 2011 for 7.24.0]*/
	EC_CURL_FTP_WEIRD_PASV_REPLY,    /* 13 */
	EC_CURL_FTP_WEIRD_227_FORMAT,    /* 14 */
	EC_CURL_FTP_CANT_GET_HOST,       /* 15 */
	EC_CURL_OBSOLETE16,              /* 16 - NOT USED */
	EC_CURL_FTP_COULDNT_SET_TYPE,    /* 17 */
	EC_CURL_PARTIAL_FILE,            /* 18 */
	EC_CURL_FTP_COULDNT_RETR_FILE,   /* 19 */
	EC_CURL_OBSOLETE20,              /* 20 - NOT USED */
	EC_CURL_QUOTE_ERROR,             /* 21 - quote command failure */
	EC_CURL_HTTP_RETURNED_ERROR,     /* 22 */
	EC_CURL_WRITE_ERROR,             /* 23 */
	EC_CURL_OBSOLETE24,              /* 24 - NOT USED */
	EC_CURL_UPLOAD_FAILED,           /* 25 - failed upload "command" */
	EC_CURL_READ_ERROR,              /* 26 - couldn't open/read from file */
	EC_CURL_OUT_OF_MEMORY,           /* 27 */
	/* Note: EC_CURL_OUT_OF_MEMORY may sometimes indicate a conversion error
			instead of a memory allocation error if CURL_DOES_CONVERSIONS
			is defined
	*/
	EC_CURL_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
	EC_CURL_OBSOLETE29,              /* 29 - NOT USED */
	EC_CURL_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
	EC_CURL_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
	EC_CURL_OBSOLETE32,              /* 32 - NOT USED */
	EC_CURL_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
	EC_CURL_HTTP_POST_ERROR,         /* 34 */
	EC_CURL_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
	EC_CURL_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
	EC_CURL_FILE_COULDNT_READ_FILE,  /* 37 */
	EC_CURL_LDAP_CANNOT_BIND,        /* 38 */
	EC_CURL_LDAP_SEARCH_FAILED,      /* 39 */
	EC_CURL_OBSOLETE40,              /* 40 - NOT USED */
	EC_CURL_FUNCTION_NOT_FOUND,      /* 41 */
	EC_CURL_ABORTED_BY_CALLBACK,     /* 42 */
	EC_CURL_BAD_FUNCTION_ARGUMENT,   /* 43 */
	EC_CURL_OBSOLETE44,              /* 44 - NOT USED */
	EC_CURL_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
	EC_CURL_OBSOLETE46,              /* 46 - NOT USED */
	EC_CURL_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
	EC_CURL_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
	EC_CURL_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */
	EC_CURL_OBSOLETE50,              /* 50 - NOT USED */
	EC_CURL_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
										wasn't verified fine */
	EC_CURL_GOT_NOTHING,             /* 52 - when this is a specific error */
	EC_CURL_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
	EC_CURL_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
									default */
	EC_CURL_SEND_ERROR,              /* 55 - failed sending network data */
	EC_CURL_RECV_ERROR,              /* 56 - failure in receiving network data */
	EC_CURL_OBSOLETE57,              /* 57 - NOT IN USE */
	EC_CURL_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
	EC_CURL_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
	EC_CURL_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
	EC_CURL_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
	EC_CURL_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
	EC_CURL_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
	EC_CURL_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
	EC_CURL_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
									that failed */
	EC_CURL_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
	EC_CURL_LOGIN_DENIED,            /* 67 - user, password or similar was not
									accepted and we failed to login */
	EC_CURL_TFTP_NOTFOUND,           /* 68 - file not found on server */
	EC_CURL_TFTP_PERM,               /* 69 - permission problem on server */
	EC_CURL_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
	EC_CURL_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
	EC_CURL_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
	EC_CURL_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
	EC_CURL_TFTP_NOSUCHUSER,         /* 74 - No such user */
	EC_CURL_CONV_FAILED,             /* 75 - conversion failed */
	EC_CURL_CONV_REQD,               /* 76 - caller must register conversion
									callbacks using curl_easy_setopt options
									CURLOPT_CONV_FROM_NETWORK_FUNCTION,
									CURLOPT_CONV_TO_NETWORK_FUNCTION, and
									CURLOPT_CONV_FROM_UTF8_FUNCTION */
	EC_CURL_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
									or wrong format */
	EC_CURL_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
	EC_CURL_SSH,                     /* 79 - error from the SSH layer, somewhat
									generic so the error message will be of
									interest when this has happened */

	EC_CURL_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
									connection */
	EC_CURL_AGAIN,                   /* 81 - socket is not ready for send/recv,
									wait till it's ready and try again (Added
									in 7.18.2) */
	EC_CURL_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
									wrong format (Added in 7.19.0) */
	EC_CURL_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
									7.19.0) */
	EC_CURL_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
	EC_CURL_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
	EC_CURL_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
	EC_CURL_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
	EC_CURL_CHUNK_FAILED,            /* 88 - chunk callback reported error */
	EC_CURL_LAST /* never use! */
};

struct CurlException: Exception
{
	CurlException(int code): Exception(code) {}
};

// -------------------------------------------------------------------------- //
