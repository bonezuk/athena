//-------------------------------------------------------------------------------------------
#ifndef __OMEGA_AUDIOIO_ASIOSYS_H
#define __OMEGA_AUDIOIO_ASIOSYS_H
//-------------------------------------------------------------------------------------------

	#if defined(_WIN32) || defined(_WIN64)
		#undef MAC 
		#define PPC 0
		#define WINDOWS 1
		#define SGI 0
		#define SUN 0
		#define LINUX 0
		#define BEOS 0

		#define NATIVE_INT64 0
		#define IEEE754_64FLOAT 1
	
	#elif BEOS
		#define MAC 0
		#define PPC 0
		#define WINDOWS 0
		#define PC 0
		#define SGI 0
		#define SUN 0
		#define LINUX 0
		
		#define NATIVE_INT64 0
		#define IEEE754_64FLOAT 1
		
		#ifndef DEBUG
			#define DEBUG 0
		 	#if DEBUG
		 		void DEBUGGERMESSAGE(char *string);
		 	#else
		  		#define DEBUGGERMESSAGE(a)
			#endif
		#endif

	#elif SGI
		#define MAC 0
		#define PPC 0
		#define WINDOWS 0
		#define PC 0
		#define SUN 0
		#define LINUX 0
		#define BEOS 0
		
		#define NATIVE_INT64 0
		#define IEEE754_64FLOAT 1
		
		#ifndef DEBUG
			#define DEBUG 0
		 	#if DEBUG
		 		void DEBUGGERMESSAGE(char *string);
		 	#else
		  		#define DEBUGGERMESSAGE(a)
			#endif
		#endif

	#else	// MAC

		#define MAC 1
		#define PPC 1
		#define WINDOWS 0
		#define PC 0
		#define SGI 0
		#define SUN 0
		#define LINUX 0
		#define BEOS 0

		#define NATIVE_INT64 0
		#define IEEE754_64FLOAT 1

		#ifndef DEBUG
			#define DEBUG 0
			#if DEBUG
				void DEBUGGERMESSAGE(char *string);
			#else
				#define DEBUGGERMESSAGE(a)
			#endif
		#endif
	#endif

//-------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------
