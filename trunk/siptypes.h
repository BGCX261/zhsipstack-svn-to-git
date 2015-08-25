#ifndef SIPTYPES_H
#define SIPTYPES_H

	/**
	 * Enumeration for transaction state.
	 * Those states are extracted from the diagram found in rfc3261.txt
	 *
	 */
	typedef enum _eTransactionState
	{
		/* STATES for invite client transaction */
		ICT_PRE_CALLING,
		ICT_CALLING,
		ICT_PROCEEDING,
		ICT_COMPLETED,
		ICT_TERMINATED,
		/* STATES for invite server transaction */
		IST_PRE_PROCEEDING,
		IST_PROCEEDING,
		IST_COMPLETED,
		IST_CONFIRMED,
		IST_TERMINATED,
		/* STATES for NON-invite client transaction */
		NICT_PRE_TRYING,
		NICT_TRYING,
		NICT_PROCEEDING,
		NICT_COMPLETED,
		NICT_TERMINATED,
		/* STATES for NON-invite server transaction */
		NIST_PRE_TRYING,
		NIST_TRYING,
		NIST_PROCEEDING,
		NIST_COMPLETED,
		NIST_TERMINATED,
		#ifndef DOXYGEN
			DIALOG_EARLY,
			DIALOG_CONFIRMED,
			DIALOG_CLOSE				/* ?? */
		#endif
	} eTransactionState;


	/**
	 * Enumeration for event type.
	 * <BR>The list of values that you need to know is reduced to this:
	 * <BR> RCV_REQINVITE,
	 * <BR> RCV_REQACK,
	 * <BR> RCV_REQUEST,
	 * <BR> RCV_STATUS_1XX,
	 * <BR> RCV_STATUS_2XX,
	 * <BR> RCV_STATUS_3456XX,
	 *<BR>
	 * <BR> SND_REQINVITE,
	 * <BR> SND_REQACK,
	 * <BR> SND_REQUEST,
	 * <BR> SND_STATUS_1XX,
	 * <BR> SND_STATUS_2XX,
	 * <BR> SND_STATUS_3456XX,
	 */
	typedef enum _eSipEventType
	{
		/* TIMEOUT EVENTS for ICT */
		TIMEOUT_A,
		/**< Timer A */
		TIMEOUT_B,
		/**< Timer B */
		TIMEOUT_D,
		/**< Timer D */

		/* TIMEOUT EVENTS for NICT */
		TIMEOUT_E,
		/**< Timer E */
		TIMEOUT_F,
		/**< Timer F */
		TIMEOUT_K,
		/**< Timer K */

		/* TIMEOUT EVENTS for IST */
		TIMEOUT_G,
		/**< Timer G */
		TIMEOUT_H,
		/**< Timer H */
		TIMEOUT_I,
		/**< Timer I */

		/* TIMEOUT EVENTS for NIST */
		TIMEOUT_J,
		/**< Timer J */

		/* FOR INCOMING MESSAGE */
		RCV_REQINVITE,
		/**< Event is an incoming INVITE request */
		RCV_REQACK,
		/**< Event is an incoming ACK request */
		RCV_REQUEST,
		/**< Event is an incoming NON-INVITE and NON-ACK request */
		RCV_STATUS_1XX,
		/**< Event is an incoming informational response */
		RCV_STATUS_2XX,
		/**< Event is an incoming 2XX response */
		RCV_STATUS_3456XX,
		/**< Event is an incoming final response (not 2XX) */

		/* FOR OUTGOING MESSAGE */
		SND_REQINVITE,
		/**< Event is an outgoing INVITE request */
		SND_REQACK,
		/**< Event is an outgoing ACK request */
		SND_REQUEST,
		/**< Event is an outgoing NON-INVITE and NON-ACK request */
		SND_STATUS_1XX,
		/**< Event is an outgoing informational response */
		SND_STATUS_2XX,
		/**< Event is an outgoing 2XX response */
		SND_STATUS_3456XX,
		/**< Event is an outgoing final response (not 2XX) */

		KILL_TRANSACTION,
		/**< Event to 'kill' the transaction before termination */
		UNKNOWN_EVT
	} eSipEventType;

	/**
	 * Enumeration for transaction type.
	 * A transaction can be either of:
	 *  ICT,
	 *  IST,
	 *  NICT,
	 *  NIST,
	 */
	typedef enum _eSipFsmType
	{
		ICT,
		/**< Invite Client (outgoing) Transaction */
		IST,
		/**< Invite Server (incoming) Transaction */
		NICT,
		/**< Non-Invite Client (outgoing) Transaction */
		NIST /**< Non-Invite Server (incoming) Transaction */
	} eSipFsmType;

	#ifndef DEFAULT_T1
		/**
		 * You can re-define the default value for T1. (T1 is defined in rfcxxxx)
		 * The default value is 500ms.
		 */
		#define DEFAULT_T1 500  		/* 500 ms */
	#endif

	#ifndef DEFAULT_T2
		/**
		 * You can re-define the default value for T2. (T2 is defined in rfcxxxx)
		 * The default value is 4000ms.
		 */
		#define DEFAULT_T2 4000 		/* 4s */
	#endif

	#ifndef DEFAULT_T4
		/**
		 * You can re-define the default value for T4. (T1 is defined in rfcxxxx)
		 * The default value is 5000ms.
		 */
		#define DEFAULT_T4 5000 		/* 5s */
	#endif


	/**
	 * Structure for INVITE CLIENT TRANSACTION (outgoing INVITE transaction).
	 * @var osip_ict_t
	 */
	typedef struct osip_ict osip_ict_t;

	/**
	 * Structure for INVITE CLIENT TRANSACTION.
	 * @struct osip_ict
	 */
	struct osip_ict
	{
		
		int		Init();

		int		SetDestination(char* destination, int port)
		{
			if (this->destination != NULL)
			{
				mm_free(this->destination);
			}
			this->destination = destination;
			this->port = port;
			return 0;
		}
		int timer_a_length; 		  /**@internal A=T1, A=2xT1... (unreliable only) */
		struct timeval timer_a_start; /**@internal */
		int timer_b_length; 		  /**@internal B = 64* T1 */
		struct timeval timer_b_start; /**@internal fire when transaction timeouts */
		int timer_d_length; 		  /**@internal D >= 32s for unreliable tr (or 0) */
		struct timeval timer_d_start; /**@internal should be equal to timer H */
		char* destination;  		  /**@internal url used to send requests */
		int port;   				  /**@internal port of next hop */
	};

		/**
	 * Structure for NON-INVITE CLIENT TRANSACTION (outgoing NON-INVITE transaction).
	 * @var osip_nict_t
	 */
	typedef struct osip_nict osip_nict_t;

	/**
	 * Structure for NON-INVITE CLIENT TRANSACTION.
	 * @struct osip_nict
	 */
	struct osip_nict
	{
		int SetDestination(char* destination, int port)
		{
			if (this->destination != NULL)
			{
				mm_free(this->destination);
			}
			this->destination = destination;
			this->port = port;
			return 0;
		}

		int timer_e_length; 		  /**@internal A=T1, A=2xT1... (unreliable only) */
		struct timeval timer_e_start; /**@internal */
		int timer_f_length; 		  /**@internal B = 64* T1 */
		struct timeval timer_f_start; /**@internal fire when transaction timeouts */
		int timer_k_length; 		  /**@internal K = T4 (else = 0) */
		struct timeval timer_k_start; /**@internal */
		char* destination;  		  /**@internal url used to send requests */
		int port;   				  /**@internal port of next hop */
	};

	/**
	 * Structure for INVITE SERVER TRANSACTION (incoming INVITE transaction).
	 * @var osip_ist_t
	 */
	typedef struct osip_ist osip_ist_t;

	/**
	 * Structure for INVITE SERVER TRANSACTION.
	 * @struct osip_ist
	 */
	struct osip_ist
	{

		int timer_g_length; 	 /**@internal G=MIN(T1*2,T2) for unreliable trans. */
		struct timeval timer_g_start; /**@internal 0 when reliable transport is used */
		int timer_h_length; 		  /**@internal H = 64* T1 */
		struct timeval timer_h_start; /**@internal fire when if no ACK is received */
		int timer_i_length; 		  /**@internal I = T4 for unreliable (or 0) */
		struct timeval timer_i_start; /**@internal absorb all ACK */
	};

	/**
	 * Structure for NON-INVITE SERVER TRANSACTION (incoming SERVER transaction).
	 * @var osip_nist_t
	 */
	typedef struct osip_nist osip_nist_t;

	/**
	 * Structure for NON-INVITE SERVER TRANSACTION.
	 * @struct osip_nist
	 */
	struct osip_nist
	{
		int timer_j_length; 		   /**@internal J = 64*T1 (else 0) */
		struct timeval timer_j_start;  /**@internal */
	};

		/**
	 * Structure for SRV record entry.
	 * @var osip_srv_entry_t
	 */
	typedef struct osip_srv_entry osip_srv_entry_t;

	/**
	 * Structure for SRV record entry.
	 * @struct osip_srv_entry
	 */

	struct osip_srv_entry
	{
		char srv[512];
		int priority;
		int weight;
		int rweight;
		int port;
	};

	/**
	 * Structure for SRV record.
	 * @var osip_srv_record_t
	 */
	typedef struct osip_srv_record osip_srv_record_t;

	/**
	 * Structure for SRV record entry.
	 * @struct osip_srv_record
	 */
	struct osip_srv_record
	{
		char name[512];
		char protocol[64];
		struct osip_srv_entry srventry[10];
	};

#endif