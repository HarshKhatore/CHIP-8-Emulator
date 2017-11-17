class chip8 {
 	
 	public:
 		//Functions
 		chip8();
 		void initialize();
 		void emulateCycle();
 		bool loadGame(const char * file_name);
 		
 		//Variables
 		bool  		   drawFlag;			//to display or not
 		unsigned char  gfx[64 * 32];		//graphics screen ( 2048 bytes )
 		unsigned char  key[16];				//keyboard mapping	 	
 	
 	private:						
 		//Variables
 		unsigned short opcode;				//to store current opcode ( 35 in number )
 		unsigned char  memory[4096];		//to emulate RAM ( 4096 bytes )
 		unsigned char  V[16];				//16 registers( 8-bit )
 		unsigned short I;					//Index register ( special register 16-bit )
 		unsigned short pc;					//Program counter ( 16-bit )
 		unsigned char  delay_timer;			//DT freq = 60Hz
 		unsigned char  sound_timer;			//ST freq = 60Hz
 		unsigned short stack[16];			//System stack( 16-bit )( 16 levels )
 		unsigned short sp;					//stack pointer
};
