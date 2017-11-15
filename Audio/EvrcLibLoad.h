#pragma once


	class EvrcLibLoad
	{
	public:
		EvrcLibLoad();
		~EvrcLibLoad();
		void pcmToEvrcHalf(const unsigned char * raw, const  unsigned int & rawLen, unsigned char * &out, unsigned int & outLen);
		void pcmToEvrcAll(const unsigned char * raw, const unsigned int & rawLen, unsigned char *& out, unsigned int & outLen);
		void evrcToPcm(const unsigned char * raw, const unsigned int & rawLen, unsigned char * out, unsigned int & outLen);
		void uninitDecoder();
		void uninitEncoder();
	private:

		
	};

