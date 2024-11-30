
#include <SoftwareSerial.h>

#define MAX1 512
#define MAX2 32

namespace ArduInterpreter
{

	SoftwareSerial Ser(2,3);
	namespace input_
	{
		void input__( char* data )
		{
			size_t index = 0;
			while(Ser.available())
			{
				data[index++] = Ser.read();
        if(!Ser.available()) delay(1);
			}
		}

	}
	
	namespace type
	{
		typedef unsigned char code;
		typedef int val;
    typedef unsigned short int shortval;
	}

	namespace mem
	{
		type::code codeBuffer[MAX1];
		type::val memoryVal[MAX2];
    type::shortval shortval[4];
	}

	struct __ms
	{
		unsigned char sequence = 0;
		unsigned short int oder = 0;
	} ms;

  void debug()
    {
      int i = 0;
      while( i < 512 )
      {
        Serial.print(mem::codeBuffer[i]);
        Serial.print(" ");
        i++;
      }
      Serial.print("\n");
    }

}

// #define Cod (short int)ArduInterpreter::mem::codeBuffer[++ArduInterpreter::ms.oder]
// #define Cod() (short int)ArduInterpreter::mem::codeBuffer[++ArduInterpreter::ms.oder]

inline char Cod() { 
	return ArduInterpreter::mem::codeBuffer[++ArduInterpreter::ms.oder];
}
#define numer_case(num, cp, op, a, b) case num: if(cp) c::mem::memoryVal[args[0]] = a op b; break  
#define _val(index) c::mem::memoryVal[args[index] & 31]

#define dex std::cout << "s";

namespace core
{
	namespace c = ArduInterpreter;

	char args[6];
	void Args(unsigned char count)
	{
		for (int i = 0; i < (6 <= count ? 6 : count); i++) args[i] = Cod();
	}
	void numer()
	{

		if (((31 < args[1]) && (31 < args[2])) || ((args[3] & 32) && (args[3] & 64))) switch (args[3] & 15)
		{
#define one args[1]
#define two args[2]
			numer_case(1, 1, +, one, two);
			numer_case(2, 1, -, one, two);
			numer_case(3, 1, *, one, two);
			numer_case(4, two != 0, / , one, two);
			numer_case(5, two != 0, %, one, two);
		}
		else if ((31 < args[2]) || (args[3] & 64)) switch (args[3] & 31)
		{
#define one _val(1)
#define two (int)args[2]
			numer_case(1, 1, +, one, two);
			numer_case(2, 1, -, one, two);
			numer_case(3, 1, *, one, two);
			numer_case(4, two != 0, / , one, two);
			numer_case(5, two != 0, %, one, two);
		}
		else if ((31 < args[1]) || (args[3] & 32)) switch (args[3] & 31)
		{
#define one (int)args[1]
#define two _val(2)
			numer_case(1, 1, +, one, two);
			numer_case(2, 1, -, one, two);
			numer_case(3, 1, *, one, two);
			numer_case(4, two != 0, / , one, two);
			numer_case(5, two != 0, %, one, two);
		} else switch (args[3] & 31)
		{
#define one _val(1)
#define two _val(2)
			numer_case(1, 1, +, one, two);
			numer_case(2, 1, -, one, two);
			numer_case(3, 1, *, one, two);
			numer_case(4, two != 0, / , one, two);
			numer_case(5, two != 0, %, one, two);
		}
#undef one
#undef two

	}

	void comper()
	{

		if ((31 < args[1] && 31 < args[2]) || ((args[3] & 32) && (args[3] & 64))) switch (args[3] & 31)
		{
#define one args[1]
#define two args[2]
			numer_case(1, 1, == , one, two);
			numer_case(2, 1, != , one, two);
			numer_case(3, 1, < , one, two);
			numer_case(4, 1, <= , one, two);
			numer_case(5, 1, > , one, two);
			numer_case(6, 1, >= , one, two);
		case 7:
			c::mem::memoryVal[args[0]] = (c::mem::memoryVal[args[1]]) ? 0 : 1;
			break;
		}
		else if ((31 < args[2]) || (args[3] & 64)) switch (args[3] & 31)
		{
#define one _val(1)
#define two args[2]
			numer_case(1, 1, == , one, two);
			numer_case(2, 1, != , one, two);
			numer_case(3, 1, < , one, two);
			numer_case(4, 1, <= , one, two);
			numer_case(5, 1, > , one, two);
			numer_case(6, 1, >= , one, two);
		case 7:
			c::mem::memoryVal[args[0]] = (c::mem::memoryVal[args[1]]) ? 0 : 1;
			break;
		}
		else if ((31 < args[1]) || (args[3] & 32)) switch (args[3] & 31)
		{
#define one args[1]
#define two _val(2)
			numer_case(1, 1, == , one, two);
			numer_case(2, 1, != , one, two);
			numer_case(3, 1, < , one, two);
			numer_case(4, 1, <= , one, two);
			numer_case(5, 1, > , one, two);
			numer_case(6, 1, >= , one, two);
		case 7:
			c::mem::memoryVal[args[0]] = (c::mem::memoryVal[args[1]]) ? 0 : 1;
			break;
		}
		else switch (args[3] & 31)
		{
#define one _val(1)
#define two _val(2)
			numer_case(1, 1, == , one, two);
			numer_case(2, 1, != , one, two);
			numer_case(3, 1, < , one, two);
			numer_case(4, 1, <= , one, two);
			numer_case(5, 1, > , one, two);
			numer_case(6, 1, >= , one, two);
		case 7:
			c::mem::memoryVal[args[0]] = (c::mem::memoryVal[args[1]]) ? 0 : 1;
			break;
		}
#undef one
#undef two
		
	}

	void savepoint()
	{
		_val(0) = c::ms.oder;
	}

	void jump( bool st )
	{
		if (args[2] & 64) c::mem::memoryVal[args[2] & 31] = c::ms.oder;
		if ( st ) return;
		if (args[0] & 32) if (_val(0) <= 0); else return;
		else if (_val(0) > 0); else return;
		
		if ((args[2] & 32) || args[1] > 31)
			if(args[2] & 128) c::ms.oder += (unsigned char)args[1];
			else c::ms.oder = (unsigned char)args[1];
		else 
			if(args[2] & 128) c::ms.oder += _val(1);
			else c::ms.oder = _val(1);
		
	}

	void defineval()
	{
		if (args[1] != 0) _val(0) = (unsigned int)(args[1] << 24) | (args[2] & 255 << 16) | (args[3] & 255 << 8) | (args[4] & 255);
		else if (args[2] != 0) _val(0) = (unsigned int)(args[2] << 16) | (args[3] & 255 << 8) | (args[4] & 255);
		else if (args[3] != 0) _val(0) = (unsigned int)(args[3] << 8) | (args[4] & 255);
		else if (args[4] != 0) _val(0) = (unsigned char)(args[4]);
	}

  void funs( bool first )
  {
    if( args[0] & 64 )
    {
      if( first )
      {
        if( args[0] & 32) c::mem::shortval[args[0]&3] = c::ms.oder; else _val(0) = c::ms.oder;
      }
      else
      {
        c::ms.oder = c::mem::shortval[3];
      }
    }
    else
    {
      if( first )
      {

      }
      else
      {
        c::mem::shortval[3] = c::ms.oder;
        if( args[0] & 32) c::ms.oder = c::mem::shortval[args[0]&3]; else c::ms.oder = _val(0);
      }
    }
  }
}

#undef numer_case
#undef val
#define _val(index) c::mem::memoryVal[core::args[index] & 31]

namespace Main
{

	namespace c = ArduInterpreter;

	void mainSwitch();
	void firstSwitch();

	void setup()
	{
    Serial.begin(9600);
		c::Ser.begin(9600);
	}

	void loop()
	{

		while (!c::Ser.available() && (c::ms.sequence == 0)) 
    {
      delay(16);
    }
		if (c::Ser.available() && (c::ms.sequence == 0))
		{
			c::ms.sequence = 1;
			c::input_::input__(c::mem::codeBuffer);
      //c::debug();
		}
		else if ((MAX1 - 1 <= c::ms.oder))
		{
			if ((c::ms.sequence == 2 ) && (c::ms.sequence = 3) && !(c::ms.oder = 0));
			if ((c::ms.sequence == 1) && (c::ms.sequence = 2) && !(c::ms.oder = 0));
		}
    if( (c::ms.sequence == 3) && (!c::Ser.available()) )
    {
      for(int i = 0; i < MAX1; i++) c::mem::codeBuffer[i] = 0;
      c::ms.sequence = 0;
    }
    
		if (c::ms.sequence == 2) mainSwitch();
		if (c::ms.sequence == 1) firstSwitch();
    if (c::ms.sequence == 2) delay(1);
		
	}

	void mainSwitch()
	{
		switch (Cod())
		{
		case 1:
			delay( c::mem::memoryVal[Cod()] - 1 );
			break;
		case 2:
			core::Args(4);
			core::numer();
			break;
		case 3:
			core::Args(4);
			core::comper();
			break;
		case 4:
			core::Args(1);
			core::savepoint();
			break;
		case 5:
			core::Args(3);
			core::jump(0);
			break;
		case 6:
			core::Args(5);
			core::defineval();
			break;
    case 7:
      core::Args(1);
      core::funs(0);
      break;
		case 9:
			c::ms.oder = 512;
			break;
    case 17:
      core::Args(2);
      pinMode( _val(0), _val(1) );
      break;
    case 18:
      core::Args(2);
      _val(1) = digitalRead( _val(0) );
      break;
    case 19:
      core::Args(2);
      digitalWrite( _val(0), _val(1) );
      break;
    case 20:
      core::Args(2);
      _val(1) = analogRead( _val(0) );
      break;
    case 21:
      core::Args(2);
      analogWrite( _val(0), _val(1) );
      break;
    case 22:
      core::Args(3);
      tone( _val(0), _val(1), _val(2));
      break;
    case 23:
      core::Args(1);
      noTone( _val(0) );
		case 111:
      core::Args(1);
      Serial.println( (int)c::mem::memoryVal[ core::args[0] ] );
			break;
		default:
			break;
		}
	}

	void firstSwitch()
	{
		switch (Cod())
		{
		case 1: Cod(); break;
		case 2: core::Args(4);
			break;
		case 3: core::Args(4);
			break;
		case 4:
			core::Args(1);
			core::savepoint();
			break;
		case 5:
			core::Args(3);
			core::jump(1);
			break;
		case 6: 
    core::Args(5);
			break;
    case 7:
    core::Args(1);
    core::funs(1);
      break;
    case 17:
      core::Args(2);
      break;
    case 18:
      core::Args(2);
      break;
    case 19:
      core::Args(2);
      break;
    case 20:
      core::Args(2);
      break;
    case 21:
      core::Args(2);
      break;
    case 22:
      core::Args(3);
      break;
    case 23:
      core::Args(2);
      break;
		case 111: core::Args(1);
			break;
		default:
			break;
		}
	}

}

void setup()
{
  Main::setup();
}

void loop()
{
  Main::loop();
  delay(1);
}