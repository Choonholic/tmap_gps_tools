/*===========================================================================
                             GPSAPIEXT Header file
DESCRIPTION
  GPS Extended APIs 

EXTERNALIZED FUNCTIONS
  

INITIALIZATION AND SEQUENCING REQUIREMENTS
  
  
  Copyright (c) 2006 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                             Edit History
                             
$Header: //source/qcom/qct/platform/winmobile/Src/Drivers/CGPS/main/latest/INC/gpsapiext.h#16 $
$DateTime: 2008/08/13 14:11:31 $
$Author: weic $

===========================================================================*/
#ifndef _GPSAPIEXT_H_
#define _GPSAPIEXT_H_

// LGE_DOM_UPDATE_S hbl1522@lge.com 2009/06/11
#define FEATURE_LGE_GPS_TEST
// LGE_DOM_UPDATE_E hbl1522@lge.com 2009/06/11

// ********************************************************
// GPS Intermediate Driver Extended APIs
// ********************************************************

#define GPS_VERSION_QCOM_CURRENT                GPS_VERSION_2 
#define GPS_OPEN_HARDWARE_INIT							0		// Flag to indicate existing mode of GPSID

// Additional paramName supported through GPSSetDeviceParam or GPSGetDeviceParam
// The following four are related to GPS PDE addresses, gps lock and aiding data
#define GPS_CONFIG_CUSTOM_1X_PDE                  (GPS_CONFIG_CUSTOM + 886)
#define GPS_CONFIG_CUSTOM_UMTS_PDE                (GPS_CONFIG_CUSTOM + 887)
#define GPS_CONFIG_CUSTOM_1X_MPC                  (GPS_CONFIG_CUSTOM + 888)       
#define GPS_CONFIG_CUSTOM_LOCAL_PDE               (GPS_CONFIG_CUSTOM + 889)
#define GPS_CONFIG_CUSTOM_WRITE_GPS_LOCK          (GPS_CONFIG_CUSTOM + 890)
#define GPS_CONFIG_CUSTOM_READ_GPS_LOCK           (GPS_CONFIG_CUSTOM + 891)
#define GPS_CONFIG_CUSTOM_DELETE_AIDING_DATA      (GPS_CONFIG_CUSTOM + 892)
#define GPS_CONFIG_CUSTOM_ON_DEMAND_READY_MODE     (GPS_CONFIG_CUSTOM + 893) // for both set/get, 0-disable, 1-enable, BOOL
#define GPS_CONFIG_CUSTOM_ON_DEMAND_LOW_POWER_MODE (GPS_CONFIG_CUSTOM + 894) // for both set/get, 0-disable, 1-enable, BOOL
#define GPS_CONFIG_CUSTOM_SBAS                     (GPS_CONFIG_CUSTOM + 895) // Satellite augumentation system,
                                                                             // for set: 0-disable, 1-enable, BOOL
                                                                             // for get: GPS_SBAS_STATUS
#define GPS_CONFIG_CUSTOM_DPO                      (GPS_CONFIG_CUSTOM + 896) // Dynamic power optimization, 
                                                                             // for both set/get, 0-disable, 1-enable, BYTE
#define GPS_CONFIG_CUSTOM_INJECT_COARSE_POSITION   (GPS_CONFIG_CUSTOM + 897) // Course position injection, only for set

// The following four are related to GPS security
#define GPS_CONFIG_CUSTOM_WRITE_SSD                    (GPS_CONFIG_CUSTOM + 1000)
#define GPS_CONFIG_CUSTOM_WRITE_SECURITY_UPDATE_RATE   (GPS_CONFIG_CUSTOM + 1001)
#define GPS_CONFIG_CUSTOM_ENABLE_SECURITY              (GPS_CONFIG_CUSTOM + 1002)
#define GPS_CONFIG_CUSTOM_WRITE_SECURITY_CONFIG        (GPS_CONFIG_CUSTOM + 1003)
#define GPS_CONFIG_CUSTOM_READ_SECURITY_CONFIG         (GPS_CONFIG_CUSTOM + 1004)
#define GPS_CONFIG_CUSTOM_READ_ENCRYPT_CONFIG          (GPS_CONFIG_CUSTOM + 1005)
#define GPS_CONFIG_CUSTOM_READ_ENCRYPT_DATA            (GPS_CONFIG_CUSTOM + 1006)
#define GPS_CONFIG_CUSTOM_READ_FIX_ERROR               (GPS_CONFIG_CUSTOM + 1007)

// Register to receive additional GPS-related information, eg: PPM
#define GPS_CONFIG_CUSTOM_REGISTER_PPM_UPDATE           (GPS_CONFIG_CUSTOM + 1100)
#define GPS_CONFIG_CUSTOM_READ_PPM_INFO                 (GPS_CONFIG_CUSTOM + 1101)
#define GPS_CONFIG_CUSTOM_REGISTER_GPS_LOCK_UPDATE      (GPS_CONFIG_CUSTOM + 1102) 

#define GPS_MAX_SERVER_URL_NAME							256	// Maximum size, in characters, of data for server URL address.
#define GPS_MAX_SECURITY_DATA							20  // Maximum size, in bytes, of security data.

// Custom GPS Modes
// Optimized for speed using a combination of MSB and MSA. MSB is preferred, only swith to MSA if MSB fails
#define	GPS_QCOM_FIX_MODE_SPEED_OPTIMAL        (GPS_FIX_MODE_CUSTOM + 1) 
// Optimized for accuracy using a combination of MSB and MSA. MSA is prefered, only switch to MSB if MSA fails.
#define	GPS_QCOM_FIX_MODE_ACCURACY_OPTIMAL     (GPS_FIX_MODE_CUSTOM + 2) 
// Optimized to reduce data transferred from the network.  MSB is preferred, only  use minimal network access. 
// Data demodulation is preferred over download from network.
#define	GPS_QCOM_FIX_MODE_DATA_OPTIMAL         (GPS_FIX_MODE_CUSTOM + 3) 
// GPS hardware performs a MSB position and forces it to request Reference position from server.
#define	GPS_QCOM_FIX_MODE_FORCE_REF_POS_REQ	   (GPS_FIX_MODE_CUSTOM + 4) 

// for LGT GeoTagging
#if	0	//defined(LGE_GPS_LGT)
#define	GPS_LGE_GEOTAG_ASSIST	  				(GPS_FIX_MODE_CUSTOM + 5) 
#define	GPS_LGE_GEOTAG_BASED			  		(GPS_FIX_MODE_CUSTOM + 6) 
#endif


#define  GPS_QCOM_FIX_MODE_MIN                  (GPS_FIX_MODE_CUSTOM + 1)
#if	0	//defined(LGE_GPS_LGT)
#define  GPS_QCOM_FIX_MODE_MAX                  (GPS_FIX_MODE_CUSTOM + 6)
#else
#define  GPS_QCOM_FIX_MODE_MAX                  (GPS_FIX_MODE_CUSTOM + 4)
#endif

// GPS Session Lock
#define GPS_QCOM_LOCK_OFF                       0   // All GPS session is allowed
#define GPS_QCOM_LOCK_ALL                       1	 // E911 only
#define GPS_QCOM_LOCK_MI                        2   // MI session is not allowed
#define GPS_QCOM_LOCK_MT                        3   // MT session is not allowed
#define GPS_QCOM_LOCK_ON                        GPS_QCOM_LOCK_ALL        

// LGE_DOM_UPDATE_S hbl1522@lge.com 2009/06/11
#ifdef FEATURE_LGE_GPS_TEST
typedef enum {
  GPS_MOLR_ASSISTANCE_DATA,
  GPS_MOLR_LOCATION_ESTIMATE
} GPS_MOLR_E_TYPE;

typedef enum{
  GPS_AIRTEST_SEARCH_TYPE_DWELL,
  GPS_AIRTEST_SEARCH_TYPE_1SV,
  GPS_AIRTEST_SEARCH_TYPE_DWELL_CONT,
  GPS_AIRTEST_SEARCH_TYPE_1SV_CONT,
  GPS_AIRTEST_SEARCH_TYPE_END = 255
} GPS_AIRTEST_SEARCH_E_TYPE ;

typedef struct _GPS_AIRTEST_S_TYPE {
  DWORD test_num;
  GPS_AIRTEST_SEARCH_E_TYPE search_type;
} GPS_AIRTEST_S_TYPE;

typedef enum {
  GPS_TEST_TYPE_UNKNOWN = 0,
  GPS_TEST_TYPE_MAIN,
  GPS_TEST_TYPE_THIRDPARTY,				
  GPS_TEST_TYPE_AIR,        
  GPS_TEST_TYPE_FIELD,		  
  GPS_TEST_TYPE_MAX
} GPS_TEST_E_TYPE;

typedef enum
{
  MO_METHOD_CP,  /* use control plane */
  MO_METHOD_UP   /* use user plane */
} GPS_MO_METHOD_E_TYPE;

#define GPS_LGE_CONFIG_CUSTOM						(GPS_CONFIG_CUSTOM + 2000)
#define GPS_LGE_CONFIG_MT_ANSWER					(GPS_LGE_CONFIG_CUSTOM + 1) 	// boolean, enable / disable
#define GPS_LGE_CONFIG_EXT_ADDR						(GPS_LGE_CONFIG_CUSTOM + 2) 	// GPS_URL_ADDRESS type¡íc¢¯e
#define GPS_LGE_CONFIG_POS_SUPPORT					(GPS_LGE_CONFIG_CUSTOM + 3)		// unsigned long
#define GPS_LGE_CONFIG_2G_MOLR						(GPS_LGE_CONFIG_CUSTOM + 4)		// GPS_MOLR_E_TYPE
#define GPS_LGE_CONFIG_3G_MOLR						(GPS_LGE_CONFIG_CUSTOM + 5)		// GPS_MOLR_E_TYPE
#define GPS_LGE_CONFIG_USE_SECURITY					(GPS_LGE_CONFIG_CUSTOM + 6)		// boolean, tls
#define GPS_LGE_CONFIG_TEST_TYPE					(GPS_LGE_CONFIG_CUSTOM + 7)		// GPS_TEST_E_TYPE,  Main, Airtest, Fieldtest, thirdparty
#define GPS_LGE_CONFIG_AIRTEST						(GPS_LGE_CONFIG_CUSTOM + 8)		// GPS_AIRTEST_S_TYPE , seach type, test num
#define GPS_LGE_CONFIG_COLDSTART					(GPS_LGE_CONFIG_CUSTOM + 9)		// boolean 
#define GPS_LGE_CONFIG_HSLPURL						(GPS_LGE_CONFIG_CUSTOM + 10)	// GPS_URL_ADDRESS type¡íc¢¯e
#define GPS_LGE_CONFIG_MO_METHOD					(GPS_LGE_CONFIG_CUSTOM + 11)	// byte 
#endif
// LGE_DOM_UPDATE_E hbl1522@lge.com 2009/06/11
// Valid Server config field
#define GPS_VALID_SERVER_IPADDRESS_V4           0x00000001
#define GPS_VALID_SERVER_IPADDRESS_V6           0x00000002
#define GPS_VALID_SERVER_URL                    0x00000003
#define GPS_MAX_SERVER_URL_NAME                 256	// Maximum size, in characters, of data for server URL address.

// Valid satellite measurement mask 
#define GPS_VALID_SATELLITE_MEASUREMENTS_MASK           (GPS_DEV_VALID_EPH_SV_MASK | GPS_DEV_VALID_ALM_SV_MASK |  \
                                                         GPS_DEV_VALID_SAT_IN_VIEW_PRN | GPS_DEV_VALID_SAT_IN_VIEW_CARRIER_TO_NOISE_RATIO)


// Encryption scheme used to secure the position data
#define	GPS_ENCRYPT_AES_128_LAT_LONG					0
// Algorithms used to generate the hash digest for security authentication by GPS hardware and security client.
#define GPS_HASH_SHA1									0
// Maximum size, in bytes, of security data.
#define GPS_MAX_SECURITY_DATA							20  

// Write Aiding data mask
#define GPS_QCOM_AIDING_DATA_EPH_DELETE               0x00000001
#define GPS_QCOM_AIDING_DATA_ALM_DELETE               0x00000002
#define GPS_QCOM_AIDING_DATA_POS_DELETE               0x00000004
#define GPS_QCOM_AIDING_DATA_TIME_DELETE              0x00000008
#define GPS_QCOM_AIDING_DATA_IONO_DELETE              0x00000010
#define GPS_QCOM_AIDING_DATA_UTC_DELETE               0x00000020
#define GPS_QCOM_AIDING_DATA_HEALTH_DELETE            0x00000040
#define GPS_QCOM_AIDING_DATA_SVDIR_DELETE             0x00000080
#define GPS_QCOM_AIDING_DATA_SVSTEER_DELETE           0x00000100
#define GPS_QCOM_AIDING_DATA_SADATA_DELETE            0x00000200
#define GPS_QCOM_AIDING_DATA_RTI_DELETE               0x00000400
#define GPS_QCOM_AIDING_DATA_CELLDB_INFO_DELETE       0x00000800
#define GPS_QCOM_AIDING_DATA_ALM_CORR_DELETE          0x00001000
#define GPS_QCOM_AIDING_FREQ_BIAS_EST_DELETE          0x00002000

#define GPS_QCOM_AIDING_DATA_DELETE_ALL               0x80000000

// Misc. defines
// GPS Fix session errors these are available via GPS_POSITION2 
#define GPS_ERR_BASE                               0x20000000          // User defined errors have bit 29 turned on
#define GPS_ERR_INSUFFICIENT_MEASUREMENTS          (GPS_ERR_BASE + 0)	// Insufficient measurements
#define GPS_ERR_AUTH_FAILED                        (GPS_ERR_BASE + 1)
#define GPS_ERR_REQ_PENDING_FOR_CLIENT             (GPS_ERR_BASE + 2) 
#define GPS_ERR_SECURITY_DATA_NOT_AVAILABLE        (GPS_ERR_BASE + 3)
#define GPS_ERR_ENCRYPT_INFO_NOT_AVAILABLE         (GPS_ERR_BASE + 4)
#define GPS_ERR_ENCRYPT_DATA_NOT_AVAILABLE         (GPS_ERR_BASE + 5)
#define GPS_ERR_PHONE_OFFLINE                      (GPS_ERR_BASE + 6)

// OEM API error will start at 0x21000000
#define GPS_ERR_OEM_API_BASE                       0x21000000

// GPS Lat/Lon scaling factor used when decrypting Lat/Lon by application
#define GPS_LATLONG_SCALE                          (100000000)

typedef DWORD GPS_LOCK;

// 
// GPS_SERVER_CONFIG contains the GPS Server configuration parameters.
//
typedef struct _GPS_SERVER_CONFIG {
  HANDLE            hGPSDevice;     // Handle returned by a call to GPSOpenDevice. Server configuration is applicable to this handle 
	DWORD				dwValidFields;	// Indicates which field is valid in this structure either pGPSServerAddr or PGPSServerUrl.
	SOCKADDR_STORAGE	GPSServerAddr;	// Pointer to GPS server IP address and TCP port that will be used to exchange aiding data
										// information for MSA or MSB modes. Currently, the GPS Engine only accepts IPv4 addrss.
	WCHAR				szGPSServerUrl[GPS_MAX_SERVER_URL_NAME];	// Pointer to GPS server URL for Domain Name Resolution.
	DWORD				dwFlags;		// Reserved, must be 0
} GPS_SERVER_CONFIG, *PGPS_SERVER_CONFIG;


//
// GPS_SESSION_LOCK contains the lock information configuration
//
typedef struct _GPS_SESSION_LOCK {
	GPS_LOCK			dwSessionLock;		//Session Lock State
}  GPS_SESSION_LOCK;


//
// GPS_SECURITY contains the security data returned by GPS hardware or a hash digest from the client.
//
typedef struct _GPS_SECURITY {
	BYTE			bSecDataId;				// Contains the identification tag associated with this random number challenge and corresponding hash digest response.
	BYTE			bDataLen;				// Size of pbData in bytes.
	BYTE			bData[GPS_MAX_SECURITY_DATA];	// Data that will contain the random number for authentication mechanism.
} GPS_SECURITY, *PGPS_SECURITY;

//
// GPS_WRITE_SECURITY contains the security configuration that is used to authenticate the client.
//
typedef struct _GPS_WRITE_SECURITY {
	HANDLE				hGPSDevice;			// Handle returned by a call to GPSOpenDevice. The parameter cannot be NULL and must be a valid handle.
	GPS_SECURITY		SecurityData;		// Contains the hash digest generated by the client.
} GPS_WRITE_SECURITY, *PGPS_WRITE_SECURITY;

//
// GPS_READ_SECURITY contains the security information from the GPS hardware.
//
typedef struct _GPS_READ_SECURITY {
	DWORD				dwHashAlg;
	GPS_SECURITY  SecurityData;
} GPS_READ_SECURITY, *PGPS_READ_SECURITY;

//
// GPS_ENCRYPT contains information about the encryption algorithm.
//
typedef struct _GPS_ENCRYPT {
   DWORD          dwEncryptAlg;		// Identifieds the algorithm used to encrypt the position data.
   SYSTEMTIME     stUTCTime;			// Timestamp used to correlate encryption data to the correct GPS_POSITION2 structure.
   BYTE           bDataLen;			// Size of the data, in bytes, of pbData
   BYTE           bData[GPS_MAX_SECURITY_DATA];	            // The data will contain a random number generated by GPS hardware used to decrypt the position data
} GPS_ENCRYPT, *PGPS_ENCRYPT;


//
// GPS_ENCRYPT_DATA contains information about the encrypted latitude and longitude.
//
typedef struct _GPS_ENCRYPT_DATA {
   SYSTEMTIME     stUTCTime;			 // Timestamp used to correlate encryption data to the correct GPS_POSITION2 structure.
   BYTE           bEncryptedDataLen; // Size of the data, in bytes, of bData
   BYTE           bEncryptedData[GPS_MAX_SECURITY_DATA]; // The encrypted position latitude and longitude
} GPS_ENCRYPT_DATA, *PGPS_ENCRYPT_DATA;

//
// GPS_AIDING_DATA_INIT structure is used to allow applications to initialize the aiding data information. This is only 
// supported by Qualcomm GPS hardware.
//
typedef struct _GPS_AIDING_DATA_INIT {
	DWORD				dwAidingDataInitMask;				// Aiding data (Ephemeris, Almanac, Position) to delete.
	DWORD				dwFlags;							// Reserved, must be 0
} GPS_AIDING_DATA_INIT;

//
// The following data types are for the Privacy notification and response
//

// The following types are specific to the Notify-Verify (privacy) notification and response
#define GPS_MAX_NOTIFICATION_EXT_CLIENT_ADDRS   20
#define GPS_MAX_NOTIFICATION_TEXT_SIZE          256
#define GPS_MAX_REQUESTOR_ID_LEN                63
#define GPS_MAX_SUPL_REQUESTOR_ID_LEN           63   // deprecated, replaced by GPS_MAX_REQUESTOR_ID_LEN, leave for backward compatibility
#define GPS_MAX_SUPL_CLIENT_NAME_LEN            63
#define GPS_MAX_SUPL_CODEWORD_LEN               20
#define GPS_MAX_VX_REQUESTER_ID_LEN             200

// Flags to indicate which fields in the Supl request are valid
#define  GPS_REQ_CLIENT_NAME_PRESENT         0x0001
#define  GPS_REQ_CLIENT_EXTADDR_PRESENT      0x0002
#define  GPS_REQ_SS_INVOKE_ID_PRESENT        0x0004
#define  GPS_REQ_URL_PRESENT                 0x0008    
#define  GPS_REQ_DEF_LOCATION_TYPE_PRESENT   0x0010
#define  GPS_REQ_REQUESTOR_ID_PRESENT        0x0020
#define  GPS_REQ_CODEWORD_PRESENT            0x0040
#define  GPS_REQ_SERVICE_TYPE_ID_PRESENT     0x0080
#define  GPS_REQ_ENCODING_TYPE_PRESENT	      0x0100

typedef enum {
  GPS_NOTIFY_VERIFY_EVENT_SUPL_REQ,
  GPS_NOTIFY_VERIFY_EVENT_UMTS_CP_REQ,
  GPS_NOTIFY_VERIFY_EVENT_VX_REQ
} GPS_NOTIFY_VERIFY_EVENT;

typedef enum {
  GPS_NO_NOTIFY_NO_VERIFY					=0,
  GPS_USER_NOTIFY_ONLY 						=1,
  GPS_USER_NOTIFY_VERIFY_ALLOW_NO_RESP		=2,	//n/w goes ahead with call flow. ACCEPT.
  GPS_USER_NOTIFY_VERIFY_NOT_ALLOW_NO_RESP	=3,	//n/w doesn't allow call flow. DENY
  GPS_PRIVACY_OVERRIDE=5						// override any privacy
} GPS_NOTIFY_VERIFY;

typedef enum {
  GPS_LOCATIONTYPE_CURRENT_LOCATION,
  GPS_LOCATIONTYPE_CURRENT_OR_LAST_KNOWN_LOCATION,
  GPS_LOCATIONTYPE_INITIAL_LOCATION
} GPS_LOCATION_TYPE;

typedef struct _GPS_SESSION_ID
{
  DWORD dwCounter;
  DWORD dwMccMnc; /* from high nibble down: mcc1, mcc2; mcc3, mnc_bitmap; 
                     mnc1, mnc2; mnc3, not-used */
  DWORD dwType;
} GPS_SESSION_ID;

typedef union _GPS_SESSION_INVOKE_ID
{
  BYTE 				bInvokeId;
  GPS_SESSION_ID 	NetworkSessionId; 
} GPS_SESSION_INVOKE_ID;

typedef struct _GPS_DEFERRED_LOCATION
{
   BYTE     bUnusedBits;
   BYTE     bMsAvailable;  
} GPS_DEFERRED_LOCATION;

typedef struct _GPS_REQUESTER_ID
{
   BYTE     bDataCodingScheme;
   BYTE     bRequestorIDString[GPS_MAX_REQUESTOR_ID_LEN];  
   BYTE     bStringLen;
} GPS_REQUESTER_ID;

typedef struct _GPS_CLIENT_NAME
{
   BYTE     bDataCodingScheme;
   BYTE     bClientNameString[GPS_MAX_SUPL_CLIENT_NAME_LEN];  
   BYTE     bStringLen;
} GPS_CLIENT_NAME;

typedef struct _GPS_CODEWORD_STRING
{
   BYTE     bDataCodingScheme;
   BYTE     bCodewordString[GPS_MAX_SUPL_CODEWORD_LEN]; 
   BYTE     bStringLen;
} GPS_CODEWORD_STRING;

typedef struct _GPS_SERVICE_ID
{
  BYTE      bServiceTypeID;
} GPS_SERVICE_ID;

typedef enum _GPS_NOTIFY_VERIFY_DATACODING_SCHEME
{
    GPS_PRESUPL_ISO646IRV = 0,
    GPS_PRESUPL_ISO8859 = 1,
    GPS_PRESUPL_UTF8 = 2,
    GPS_PRESUPL_UTF16 = 3,
    GPS_PRESUPL_UCS2 = 4,
    GPS_PRESUPL_GSM_DEFAULT = 5,
    GPS_PRESUPL_SHIFT_JIS = 6,
    GPS_PRESUPL_JIS = 7,
    GPS_PRESUPL_EUC = 8,
    GPS_PRESUPL_GB2312 = 9,
    GPS_PRESUPL_CNS11643 = 10,
    GPS_PRESUPL_KSC1001 = 11,
    
    GPS_SS_GERMAN = 12,
    GPS_SS_ENGLISH = 13,
    GPS_SS_ITALIAN = 14,
    GPS_SS_FRENCH = 15,
    GPS_SS_SPANISH = 16,
    GPS_SS_DUTCH = 17,
    GPS_SS_SWEDISH = 18,
    GPS_SS_DANISH = 19,
    GPS_SS_PORTUGUESE = 20,
    GPS_SS_FINNISH = 21,
    GPS_SS_NORWEGIAN = 22,
    GPS_SS_GREEK = 23,
    GPS_SS_TURKISH = 24,
    GPS_SS_HUNGARIAN = 25,
    GPS_SS_POLISH = 26,
    GPS_SS_LANGUAGE_UNSPEC = 27,

    GPS_SUPL_UTF8 = 28,
    GPS_SUPL_UCS2 = 29,
    GPS_SUPL_GSM_DEFAULT = 30,
    GPS_NOTIF_DISPLAYENCODINGTYPE_UNKNOWN_ENUMERATOR = 2147483647,

} GPS_NOTIFY_VERIFY_DATACODING_SCHEME;

typedef enum _GPS_POS_METHOD
{
  GPS_POS_METHOD_AGPS_SETASSISTED = 0,
  GPS_POS_METHOD_AGPS_SETBASED = 1,
  GPS_POS_METHOD_AGPS_SETASSISTED_PREF = 2,
  GPS_POS_METHOD_AGPS_SETBASED_PREF = 3,
  GPS_POS_METHOD_AUTONOMOUS_GPS = 4,
  GPS_POS_METHOD_AFLT = 5,
  GPS_POS_METHOD_ECID = 6,
  GPS_POS_METHOD_EOTD = 7,
  GPS_POS_METHOD_OTDOA = 8,
  GPS_POS_METHOD_NO_POSITION = 9
} GPS_POS_METHOD;  

typedef enum _GPS_SERVER_ADDRESS_TYPE {
  GPS_SERVER_ADDR_TYPE_MIN = -1,
  GPS_SERVER_TYPE_IPV4_ADDRESS,
  GPS_SERVER_TYPE_URL_ADDRESS,
  GPS_SERVER_TYPE_IPV6_ADDRESS,
  GPS_SERVER_ADDR_TYPE_MAX = 0x1000000
} GPS_SERVER_ADDRESS_TYPE;

typedef struct {
  DWORD   serverAddress;  /* IPV4 server address */
  DWORD   portID;      /* IPV4 Port Id */
} GPS_IPV4_ADDRESS;     /* IPV4 server address */

typedef struct {
  WORD  serverAddress[8]; /* IPV6 server address */
  DWORD   portID;        /* IPV6 port id */
} GPS_IPV6_ADDRESS;  /* IP V6 server address */

typedef struct {
  BYTE   urlLength; /* URL Length */
  BYTE   url[255];  /* URL data */
} GPS_URL_ADDRESS; /* Server URL address */

typedef union {
   GPS_IPV4_ADDRESS serverV4;  /* IPV4 address */ 
   GPS_IPV6_ADDRESS serverV6;  /* IPV6 address */ 
   GPS_URL_ADDRESS  serverURL;  /* URL address */ 
} GPS_SERVER_ADDRESS;

typedef struct _GPS_SUPL_SLP_ADDRESS
{
  GPS_SERVER_ADDRESS_TYPE serverAddressChoice;
  GPS_SERVER_ADDRESS      suplSlpAddress;
} GPS_SUPL_SLP_ADDRESS;

#define GPS_SUPL_SLP_SESSION_ID_BYTE_LENGTH 4
typedef struct _GPS_SUPL_SESSION_ID
{
  BOOL                  bPresence; 
  BYTE                  bSessionID[GPS_SUPL_SLP_SESSION_ID_BYTE_LENGTH];   
  GPS_SUPL_SLP_ADDRESS  slpAddress;
} GPS_SUPL_SESSION_ID;

#define GPS_SUPL_QOP_VALID 0x01
#define GPS_SUPL_QOP_VERACC_VALID 0x02
#define GPS_SUPL_QOP_MAXAGE_VALID 0x04
#define GPS_SUPL_QOP_DELAY_VALID 0x08

typedef struct _GPS_SUPL_QOP_TYPE
{
  BYTE      bitMask; /* bit mask indicating which fields are valid in this struct */
  BYTE      horAcc;
  BYTE      verAcc;  /* optional; set in bit_mask supl_veracc_present if present */
  WORD      maxLocAge;  /* optional; set in bit_mask supl_maxLocAge_present if present */
  BYTE      delay;  /* optional; set in bit_mask supl_delay_present if  present */
} GPS_SUPL_QOP_TYPE;

#define GPS_SUPL_HASH_LEN  (8)
typedef struct _GPS_SUPL_REQ 
{
   GPS_NOTIFY_VERIFY       NotificationType; /* Notification Type */
   WORD                    bFlags;           /* Indicates if ext client add, notification text, url, a SS Invoke ID are present. */ 
   GPS_SUPL_SESSION_ID     suplSessionId;
   BYTE                    suplHash[GPS_SUPL_HASH_LEN]; 
   
   GPS_NOTIFY_VERIFY_DATACODING_SCHEME   dwDatacodingScheme; /* Type of Data encoding scheme for the text */
   
   GPS_POS_METHOD          posMethod;
   GPS_REQUESTER_ID        RequestorId;      /* Requestor ID */
   GPS_CLIENT_NAME         ClientName;       /* Client Name */
   CHAR                    szUrl[GPS_MAX_SERVER_URL_NAME];  /* mandatory SUPL field */
   GPS_SUPL_QOP_TYPE       suplQop;             /* supl qop */
   WORD                    wUserResponseTimer;  /* Time before which a user has to respond. Not used currently */
   
} GPS_SUPL_REQ;

typedef struct _GPS_UMTS_CP_REQ {
   GPS_NOTIFY_VERIFY        NotificationType; /* Notification Type */
   GPS_SESSION_INVOKE_ID    SessionInvokeId;       /* SS Invoke Id or the Network Session ID */
   WORD                     bFlags;           /* Indicates if ext client add, notification text, url, a SS Invoke ID are present. */ 
   BYTE                     bNotificationLength;
   CHAR                     szNotificationText[GPS_MAX_NOTIFICATION_TEXT_SIZE];   /* Notification Text valid only if the flags indicate notification text present */
   GPS_NOTIFY_VERIFY_DATACODING_SCHEME     dwDatacodingScheme; /* Type of Data encoding scheme for the text */
   BYTE                     bExtClientAddress[GPS_MAX_NOTIFICATION_EXT_CLIENT_ADDRS]; /* This field is valid only the flags indicate ext client id pres. Supported only for CP */ 
   BYTE                     bExtClientAddressLen;   /* External Client Address Length */
   GPS_LOCATION_TYPE        LocationType;          /* Location Type sent only for CP */
   GPS_DEFERRED_LOCATION    DeferredLocation;   /* Deferred Location - CP Only */
   GPS_REQUESTER_ID         RequestorId;      /* Requestor ID */
   GPS_CODEWORD_STRING      CodewordString;      /* Codeword String - CP Only */
   GPS_SERVICE_ID           ServiceId;         /* Service Type ID - CP Only */
   WORD                   wUserResponseTimer; /* Time before which a user has to respond. Not used currently */
} GPS_UMTS_CP_REQ;

typedef enum {
   GPS_MS_ASSISTED_ONLY					= 0,
   GPS_MS_BASED_ONLY 					= 1,
   GPS_MS_ASSISTED_PREF_MSBASED_ALLWD	= 2,
   GPS_MS_BASED_PREF_ASSISTED_ALLWD 	= 3,
   GPS_POS_MODE_MAX
} GPS_PRIVACY_MODE;

typedef enum {
    GPS_VX_OCTET						= 0,
    GPS_VX_EXN_PROTOCOL_MSG				= 1,
    GPS_VX_ASCII						= 2,
    GPS_VX_IA5							= 3,
    GPS_VX_UNICODE						= 4,
    GPS_VX_SHIFT_JIS					= 5,
    GPS_VX_KOREAN						= 6,
    GPS_VX_LATIN_HEBREW					= 7,
    GPS_VX_LATIN						= 8,
    GPS_VX_GSM							= 9,
    GPS_VX_ENCODING_TYPE_MAX  
} GPS_VX_REQUESTER_ID_ENCODING_SCHEME;

typedef struct _GPS_VX_REQESTER_ID {
  BYTE 	bRequesterIdLength;
  BYTE 	bRequesterId[GPS_MAX_VX_REQUESTER_ID_LEN];
} GPS_VX_REQUESTER_ID;



// The Vx Notify-Verify Request
typedef struct _GPS_VX_REQ {
  GPS_NOTIFY_VERIFY      NotificationType;
  BOOL                   bPosQosValid;
  BYTE                   bPosQos;
  DWORD                  dwNumFixes;
  DWORD                  dwTbf;
  GPS_PRIVACY_MODE       Mode;
  GPS_VX_REQUESTER_ID_ENCODING_SCHEME EncodingScheme;
  GPS_VX_REQUESTER_ID    RequesterId;
  DWORD                  dwUserRespTimerVal;
} GPS_VX_REQ;

typedef union _GPS_NOTIFY_VERIFY_INFO
{  
   GPS_UMTS_CP_REQ      CpReq;
   GPS_SUPL_REQ         SuplReq;
   GPS_VX_REQ           VxReq; 
} GPS_NOTIFY_VERIFY_INFO ;

typedef struct _GPS_READ_NOTIFY_VERIFY {
   GPS_NOTIFY_VERIFY_EVENT       NotifyVerifyEvent;
   GPS_NOTIFY_VERIFY_INFO        NotifyVerifyInfo;
} GPS_READ_NOTIFY_VERIFY, *PGPS_READ_NOTIFY_VERIFY;


typedef enum
{
  GPS_NOTIFY_VERIFY_ACCEPT    = 0,
  GPS_NOTIFY_VERIFY_DENY      = 1,
  GPS_NOTIFY_VERIFY_NORESP    = 2
} GPS_NOTIFICATION_RESPONSE;

typedef struct _GPS_WRITE_NOTIFY_VERIFY
{
  GPS_NOTIFICATION_RESPONSE         Resp;                //did the user accept or deny
  GPS_NOTIFY_VERIFY_EVENT           NotifyVerifyEvent;   //what event/ request is this in response to?
  GPS_NOTIFY_VERIFY_INFO            NotifyVerifyInfo;    //data associated with event/ request
} GPS_WRITE_NOTIFY_VERIFY, *PGPS_WRITE_NOTIFY_VERIFY; 

//
// The following data types are used for Time injection related information
//

// GPS_READ_TIME_INJECT structure is used to read the time information including server information which will then be used to download
// time injection information into the GPS System.
//
typedef struct _GPS_READ_TIME_INJECT {
   DWORD   dwOnewayDelayFailoverThresh;                    // one-way delay threshold for failover to backup servers in milliseconds
   CHAR    szTimeServerPrimary[GPS_MAX_SERVER_URL_NAME];   // URL of primary time server
   CHAR    szTimeServerSecondary[GPS_MAX_SERVER_URL_NAME]; // URL of secondary time server
   CHAR    szTimeServerTertiary[GPS_MAX_SERVER_URL_NAME];  // URL of tertiary (backup) time server
} GPS_READ_TIME_INJECT, *PGPS_READ_TIME_INJECT;

// GPS_WRITE_TIME_INJECT structure is used to inject time (write) the time information into the GPS system.
typedef struct _GPS_WRITE_TIME_INJECT {
  UINT64    uiTimeMsec;   // Time in msecs
  DWORD     dwTimeUncMsec; // Associated time uncertainty
  BOOL      fRefToUTCTime; // Flag to indicate whether time is referenced to UTC time - 1 indicates UTC time, 0 indicates GPS time
  BOOL      fForceAccept;  // Flag to indicate whether to force acceptance of data
} GPS_WRITE_TIME_INJECT, *PGPS_WRITE_TIME_INJECT;

//
// The following data structures are used for Assistance Data download data types.
//

// Flags to indicate which download information is valid is GPS_READ_ASSISTANCE_INFO
#define GPS_VALID_ASSISTANCE_DOWNLOAD_INFO     0x00000001
#define GPS_VALID_ASSISTANCE_DOWNLOAD_STATUS   0x00000002

// GPS_DOWNLOAD_INFO structure is used to specify the download information for example the download servers and the maximum file
// size that can be downloaded.
typedef struct _GPS_ASSISTANCE_DOWNLOAD_INFO {
  CHAR     szDownloadServerPrimary[GPS_MAX_SERVER_URL_NAME];   // URL of primary Assistance download server
  CHAR     szDownloadServerSecondary[GPS_MAX_SERVER_URL_NAME]; // URL of secondary (backup) Assistance download server
  CHAR     szDownloadServerTertiary[GPS_MAX_SERVER_URL_NAME];  //URL of tertiary (backup) Assistance download server
  DWORD    dwMaxFilePartSize;                                  // Maximum part size in bytes.
  DWORD    dwMaxFileSize;                                      // Maximum total size of file (for all parts) in bytes.
} GPS_ASSISTANCE_DOWNLOAD_INFO, *PGPS_ASSISTANCE_DOWNLOAD_INFO;

typedef enum {
   GPS_ASSISTANCE_DOWNLOAD_STATUS_SUCCESS,
   GPS_ASSISTANCE_DOWNLOAD_STATUS_BAD_CRC,
   GPS_ASSISTANCE_DOWNLOAD_STATUS_BAD_BUFFER_LENGTH,
   GPS_ASSISTANCE_DOWNLOAD_STATUS_TOA_BAD_RANGE,
   GPS_ASSISTANCE_DOWNLOAD_STATUS_FAILURE,
   GPS_ASSISTANCE_DOWNLOAD_STATUS_GPS_BUSY,
   GPS_ASSISTANCE_DOWNLOAD_STATUS_END_E911
} GPS_ASSISTANCE_DOWNLOAD_STATUS;

// GPS_READ_ASSISTANCE_DOWNLOAD structure is used to either read the download info (e.g. download servers and maximum file size that can
// download, or to read the download status when each part is sent to the GPS system, or data validity. A flag is used to determine which information is valid 
// in the structure

typedef struct _GPS_READ_ASSISTANCE_DOWNLOAD {
  	DWORD			              dwValidFields;  // Information about the data available; a combination of information on following fields
  	GPS_ASSISTANCE_DOWNLOAD_INFO          DownloadInfo;   // Download information
  	GPS_ASSISTANCE_DOWNLOAD_STATUS        DownloadStatus; // Status of a part that has been set into the GPS System.
} GPS_READ_ASSISTANCE_DOWNLOAD, *PGPS_READ_ASSISTANCE_DOWNLOAD;

// GPS_READ_ASSISTANCE_DOWNLOAD_DATA_VALIDITY structure contains the download validity information 
typedef struct _GPS_READ_ASSISTANCE_DOWNLOAD_DATA_VALIDITY { 
  DWORD                                     dwStartGPSWeek;       // Current download info is good starting this GPS week
  DWORD                                     dwStartGPSMinutes;    // Current download info is good starting this many minutes into the week
  DWORD                                     dwValidDurationHours; // Download info is valid for this many hours
} GPS_READ_ASSISTANCE_DOWNLOAD_DATA_VALIDITY, *PGPS_READ_ASSISTANCE_DOWNLOAD_DATA_VALIDITY;

// GPS_WRITE_ASSISTANCE_DOWNLOAD_PART structure is used to write a part of the downloaded data into the GPS System.
typedef struct _GPS_WRITE_ASSISTANCE_DOWNLOAD_PART {
   DWORD                                dwDataLen;   // Length of downloaded data to be written in this part
   BYTE                                 *pbData;      // Downloaded data part that will be written
   BYTE                                 bPartNumber; // Part number of total parts downloaed 
   BYTE                                 bTotalParts; // Total number of parts
} GPS_WRITE_ASSISTANCE_DOWNLOAD_PART, *PGPS_WRITE_ASSISTANCE_DOWNLOAD_PART;

// GPS_WRITE_AUTO_ASSISTANCE_DOWNLOAD_INFO structure contains the auto download information
typedef struct _GPS_WRITE_AUTO_ASSISTANCE_DOWNLOAD_INFO {
  BOOL    fEnableAutoDownload;   // Flag to indicate whether to enable auto download
  DWORD   dwDownloadInterval;    // Time interval in hours between downloads
} GPS_WRITE_AUTO_ASSISTANCE_DOWNLOAD_INFO, *PGPS_WRITE_AUTO_ASSITANCE_DOWNLOAD_INFO;

// The following data structures are related to PPM report
#define GPS_SRCH_MAX_PPM_RM_RPT_SAMPLES      25
typedef struct
{
   int       PilotPnPhase;
   u_char    PilotRmse;
   u_char    PilotEcio;
} GPS_PILOT_INFO_TYPE, *PGPS_PILOT_INFO_TYPE;

typedef struct 
{
   SYSTEMTIME     stUTCTime;   //  UTC according to GPS clock.
   WORD           RefPN; 
   WORD           CDMAFreq;
   WORD           BaseID;
   WORD           SystemID;
   WORD           NetworkID;
   u_char         NumPilots;
   u_char         RefPnEcio;
   u_char         BandClass;
   u_char         TotalRxPwr;
   GPS_PILOT_INFO_TYPE   PilotResults[GPS_SRCH_MAX_PPM_RM_RPT_SAMPLES];
} GPS_PPM_INFO, *PGPS_PPM_INFO;

typedef enum
{
  GPS_SBAS_STATUS_NOT_AVAILABLE,
  GPS_SBAS_STATUS_ENABLED,
  GPS_SBAS_STATUS_DISABLED
} GPS_SBAS_STATUS, *PGPS_SBAS_STATUS;

#define GPS_EXTERN_COARSE_POS_GPS_TIME_VALID 0x0001
#define GPS_EXTERN_COARSE_POS_LAT_LONG_VALID 0x0002
#define GPS_EXTERN_COARSE_POS_ALT_VALID      0x0004

typedef enum
{
   GPS_EXTERN_COARSE_POS_SOURCE_UNKNOWN,
   GPS_EXTERN_COARSE_POS_SOURCE_GPS,
   GPS_EXTERN_COARSE_POS_SOURCE_CELLID,
   GPS_EXTERN_COARSE_POS_SOURCE_ENH_CELLID, 
   GPS_EXTERN_COARSE_POS_SOURCE_WIFI,  
   GPS_EXTERN_COARSE_POS_SOURCE_TERRESTRIAL,     
   GPS_EXTERN_COARSE_POS_SOURCE_GPS_TERRESTRIAL_HYBRID,
   GPS_EXTERN_COARSE_POS_SOURCE_OTHER,
   GPS_EXTERN_COARSE_POS_SOURCE_COUNT
} GPS_EXTERN_COARSE_POS_SOURCE_TYPE;

typedef enum
{
  GPS_EXTERN_COARSE_POS_TIMESTAMP_GPS,
  GPS_EXTERN_COARSE_POS_TIMESTAMP_UTC,
  GPS_EXTERN_COARSE_POS_TIMESTAMP_AGE,
  GPS_EXTERN_COARSE_POS_TIMESTAMP_COUNT
} GPS_EXTERN_COARSE_POS_TIMESTAMP_TYPE; 

typedef enum
{
  GPS_EXTERN_COARSE_POS_ALTITUDE_HAE,  /* height above ellipsoid */
  GPS_EXTERN_COARSE_POS_ALTITUDE_MSL,  /* height above mean sea level */
  GPS_EXTERN_COARSE_POS_ALTITUDE_COUNT 
} GPS_EXTERN_COARSE_POS_ALTITUDE_TYPE; /* definition of altitude */ 


typedef struct
{
   /* indicating the presence/absence of the optional fields */
   DWORD                                 ValidOptFields; 

   /* info. of the time associated with the position. */
   GPS_EXTERN_COARSE_POS_TIMESTAMP_TYPE  TimeInfoType;
   DWORD                                 Timestamp; /* acc. sec*/
   
   GPS_EXTERN_COARSE_POS_SOURCE_TYPE     PosSource;

   double                                Latitude;  /* Degrees latitude.  North is positive */
   double                                Longitude; /* Degrees longitude.  East is positive */

   /* horizontal unc: circular shape, radial */
   DWORD                                 LocUncertaintyHorizontal;   /* horizontal location unc.: meters */

   /* confidence horizontal: percentage, as defined by  ETSI TS 101 109. 
     0 - no info. to be rejected; 100-127 not used; if 100 is
     received, re-interpret to 99 */
   BYTE                                  ConfidenceHorizontal;

   GPS_EXTERN_COARSE_POS_ALTITUDE_TYPE   AltType;   /* def. of altitude */
   INT                                   Altitude; /* alt: meters, positive height, negative depth  */ 

   /* vertical unc */
   WORD                                  LocUncertaintyVertical;   /* loc unc vertical: meters */

  /* confidence: percentage, as defined by  ETSI TS 101 109. 
     0 - no info. to be rejected; 100-127 not used; if 100 is
     received, re-interpret to 99 */
   BYTE                                  ConfidenceVertical; 

} GPS_EXTERNAL_CORASE_POS_INFO, *PGPS_EXTERNAL_CORASE_POS_INFO;   
   

#endif // _GPSAPIEXT_H_


